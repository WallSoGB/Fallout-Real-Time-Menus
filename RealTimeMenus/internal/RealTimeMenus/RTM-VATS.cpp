#include "RTM-VATS.hpp"
#include "RTM-Settings.hpp"

#include "Bethesda/BSShaderProperty.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/TimeGlobal.hpp"
#include "Bethesda/VATSEffect.hpp"
#include "Bethesda/VATSMenu.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace VATS {

		class FakeGameSetting {
		public:
			union Info {
				const char* str;
				int				i;
				unsigned int	u;
				float			f;
				bool			b;
				char			c;
				char			h;
			};

			FakeGameSetting() {
				memset(this, 0, sizeof(FakeGameSetting));
			}

			~FakeGameSetting() {
			}

			void*		__vftable;
			Info		uValue;
			const char* pKey;

			void Initialize(const char* apName, float afValue) {
				ThisCall(0x40E0B0, this, apName, afValue);
			}

			void Initialize(const char* apName, int32_t aiValue) {
				ThisCall(0x40C150, this, apName, aiValue);
			}

			float Float() const {
				return uValue.f;
			}
		};

		float fOriginalTimeScale = 1.f;
		bool bTimeScaleChanged = false;
		bool bExiting = false;

		FakeGameSetting fVATSTimeMultSetting;

		uint32_t uiLastChanceUpdateTime = 0;

		int32_t eNextLimbToRender = BODY_PART_TYPE::NONE;

		float fTileSafeZoneX = 60.f;
		float fTileSafeZoneY = 50.f;

		class Hook {
		public:
			// Runs twice per frame
			void RenderQueriesAndMask(BSRenderedTexture* apCurrentTexture, uint32_t aeQueryType, bool abForce, bool abWholeBody, NiRenderer::ClearFlags uiClearMode, bool abEndFrame) {
				VATSEffect* pThis = reinterpret_cast<VATSEffect*>(this);

				const BODY_PART_TYPE eOriginalLimb = pThis->eSelectedLimb;
				if (pThis->eMode == VATSEffect::Mode::TARGET && pThis->bIsOcclusionEnabled) {
					if (eNextLimbToRender != BODY_PART_TYPE::NONE) {
						pThis->eSelectedLimb = BODY_PART_TYPE(eNextLimbToRender);
					}
				}

				ThisCall(0x800F30, pThis, apCurrentTexture, aeQueryType, abForce, abWholeBody, uiClearMode, abEndFrame);

				pThis->eSelectedLimb = eOriginalLimb;
			}

			bool VATSUpdate() {
				VATSMenu* pThis = reinterpret_cast<VATSMenu*>(this);
				bool bResult = ThisCall<bool>(0x7F3E00, this);
				if (bResult && ::VATS::GetSingleton()->eMode != VATS_PLAYBACK) {
					VATSEffect* pEffect = Interface::GetVATSEffect();
					TESObjectREFR* pTargetRef = VATSMenu::GetTargetReference();
					if (pTargetRef && pEffect->eSelectedLimb != BODY_PART_TYPE::NONE) {
						auto pTargetEntry = pThis->FindTarget(pTargetRef);
						if (pTargetEntry) {
							VATSTarget* pTarget = pTargetEntry->GetItem();
							if (pTarget->eType == VATSTarget::Type::PROJECTILE) {
								pEffect->eSelectedLimb = BODY_PART_TYPE::TORSO;
							}

							ThisCall(0x7F1840, this); // VATSMenu::InitBodyPartTiles

							{
								auto pIter = pTarget->kBodyParts.GetHead();
								while (pIter && !pIter->IsEmpty()) {
									VATSBodyPart* pPart = pIter->GetItem();
									if (pTarget->eType == VATSTarget::Type::PROJECTILE && pPart->eBodyPart == BODY_PART_TYPE::NONE) {
										pPart->eBodyPart = BODY_PART_TYPE::TORSO;
									}

									pPart->bChanceCalculated = false;
									pPart->bNeedsRecalc = false;
									if (pEffect->GetBodyPartVisibilityPercent(pPart->eBodyPart, pPart->fPercentVisible)) {
										pPart->fHitChance = 0.f;
										if (ThisCall<bool>(0x7F06D0, this, pPart)) { // VATSMenu::IsBodyPartVisible
											pPart->bChanceCalculated = true;
										}
										else {
											pPart->bNeedsRecalc = true;
										}
										ThisCall(0x7F1290, this, pIter); // VATSMenu::UpdateHitChanceForBodyPartData
									}

									pIter = pIter->GetNext();
								}
							}

							{
								bool bRenderedLimbs = false;
								for (uint32_t i = 0; i < 16; i++) {
									bRenderedLimbs |= pEffect->bHasExecutedQuery[i];
									pEffect->bHasExecutedQuery[i] = false;
								}

								if (bRenderedLimbs) {
									eNextLimbToRender = (eNextLimbToRender + 1) % BODY_PART_TYPE::COUNT;
									bool bValidPart = false;
									while (!bValidPart) {
										auto pIter = pTarget->kBodyParts.GetHead();
										while (pIter && !pIter->IsEmpty()) {
											VATSBodyPart* pPart = pIter->GetItem();
											pIter = pIter->GetNext();
											if (pPart && pPart->eBodyPart == eNextLimbToRender) {
												bValidPart = true;
												break;
											}
										}

										if (!bValidPart)
											eNextLimbToRender = (eNextLimbToRender + 1) % BODY_PART_TYPE::COUNT;
									}
								}
								else {
									eNextLimbToRender = BODY_PART_TYPE::NONE;
								}

								pEffect->bIsOcclusionEnabled = true;
							}

							uint32_t uiCurrentTime = GetTickCount();
							if (uiCurrentTime - uiLastChanceUpdateTime >= 500) {
								uiLastChanceUpdateTime = uiCurrentTime;

								ThisCall(0x7F0EA0, this); // VATSMenu::UpdateHitChanceTiles
							}
						}
					}
				}
				return bResult;
			}

			float GetRealTimeDelta() {
				return reinterpret_cast<BSTimer*>(this)->fRealTimeDelta;
			}

			void ClearTargetRef() {
				TESObjectREFR* pThis = reinterpret_cast<TESObjectREFR*>(this);
				VATSEffect* pEffect = Interface::GetVATSEffect();
				if (pEffect) {
					if (pEffect->kCurrentTarget.pRef == pThis) {
						pEffect->SetTarget(nullptr, BODY_PART_TYPE::TORSO, false);
					}
					else {
						for (int32_t i = 0; i < 32; i++) {
							if (pEffect->kAllTargets[i].pRef == pThis) {
								NiNode* pNode = pEffect->kAllTargets[i].spNode;
								CdeclCall(0xBA91A0, pNode, BSSP_VATS_SELECTION, false); // BSShaderProperty::SetFlagRecurse
								pEffect->kAllTargets[i].pRef = nullptr;
								pEffect->kAllTargets[i].spNode = nullptr;
								pEffect->iTargetCount--;

								if (pEffect->iHighlightedTarget == i) {
									pEffect->iHighlightedTarget = -1;
								}

								break;
							}
						}
					}
				}

				ThisCall(0x57ACC0, this);
			}

			void RestoreGlobalTimeMultiplier(float afMult, bool abNow) {
				if (bTimeScaleChanged) {
					afMult = fOriginalTimeScale;
					if (bExiting)
						bTimeScaleChanged = false;
				}
				ThisCall(0xAA4DB0, this, afMult, abNow);
			}

			void MuteSelected(uint32_t aeSoundTypes, bool abExact) {
			}

			void UnmuteSelected(uint32_t aeSoundTypes, bool abExact) {
			}
		};

		void __fastcall OnVATSSetMode(uint32_t aeMode) {
			*reinterpret_cast<Actor**>(0x11F21F0) = PlayerCharacter::GetSingleton();
			float fCurrentTimeScale = BSTimer::GetGlobalTimeMultiplierTarget();
			switch (aeMode) {
				case VATS_MODE_NONE:
					bExiting = true;
					eNextLimbToRender = BODY_PART_TYPE::NONE;
					// Time is restored elsewhere
					break;
				case VATS_TARGET_SELECT:
				case VATS_MENU_MODE:
					if (!bTimeScaleChanged) {
						bTimeScaleChanged = true;
						fOriginalTimeScale = fCurrentTimeScale;
					}
					break;
				case VATS_SCAN_MODE:
				case VATS_PLAYBACK:
					break;
				default:
					break;
			}

			if (bTimeScaleChanged)
				TimeGlobal::GetSingleton()->SetGlobalTimeMultiplier(fVATSTimeMultSetting.Float(), true);
		}

		void __declspec(naked) __fastcall OnVATSSetMode_Asm() {
			static constexpr uint32_t uiJumpTarget = 0x9C6C79;
			__asm {
				mov     ecx, [ebp + 8]
				call	OnVATSSetMode
				jmp 	uiJumpTarget
			}
		}

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace VATS {

		void InitHooks() {
			fVATSTimeMultSetting.Initialize("fVATSTimeMult", Settings::fVATSTimeMultiplier);
		}

		void InitDeferredHooks() {
			if (!Settings::IsMenuPaused(Interface::VATS)) {
				PatchMemoryNop(0x944965, 6); // Always track target

				// Render VATS UI even if we are not "on the target" (since it's moving)
				PatchMemoryNop(0x9454CC, 2);
				PatchMemoryNop(0x9454DE, 2);

				ReplaceCallEx(0x7ED349, &Hook::VATSUpdate);

				ReplaceCallEx(0x7EB8B1, &Hook::MuteSelected);
				ReplaceCallEx(0x7EC391, &Hook::UnmuteSelected);

				ReplaceCallEx(0x55A47D, &Hook::ClearTargetRef); // Clear target ref on target delete

				WriteRelJump(0x9C6C6F, OnVATSSetMode_Asm); // Set/control timemult

				ReplaceCallEx(0x9C6F3A, &Hook::RestoreGlobalTimeMultiplier); // Restore time multiplier
				ReplaceCallEx(0x9C7F27, &Hook::RestoreGlobalTimeMultiplier);
				ReplaceCallEx(0x9C83A0, &Hook::RestoreGlobalTimeMultiplier);
				ReplaceCallEx(0x9C8579, &Hook::RestoreGlobalTimeMultiplier);
				ReplaceCallEx(0x9C8A82, &Hook::RestoreGlobalTimeMultiplier);

				// Update camera with real time delta instead of scaled
				ReplaceCallEx(0x945444, &Hook::GetRealTimeDelta);
				ReplaceCallEx(0x94545D, &Hook::GetRealTimeDelta);
				ReplaceCallEx(0x95E3AE, &Hook::GetRealTimeDelta);
				ReplaceCallEx(0x95E3C7, &Hook::GetRealTimeDelta);
				ReplaceCallEx(0x7ED24D, &Hook::GetRealTimeDelta);

				ReplaceCallEx(0x8741AF, &Hook::RenderQueriesAndMask);
				ReplaceCallEx(0x874A82, &Hook::RenderQueriesAndMask);

				// Stewie patches this
				if (*reinterpret_cast<uint8_t*>(0x71634F) == 0x75)
					SafeWriteBuf(0x71634C + 2, "\x00\x74", 2); // Fix UI timemult in VATS

				// Visual tweaks
				{
					WriteRelJump(0x7F6F44, 0x7F6F0E); // Make all % tiles boxes, like the torso (except head)
					WriteRelJump(0x7F6F5F, 0x7F6F0E);

					WriteRelJump(0x7F2040, 0x7F20E9); // Treat every % tile as visible

					WriteRelJump(0x7F218E, 0x7F226F); // Center tiles on body parts (except head)

					WriteRelJump(0x7F2290, 0x7F22DA); // Skip screen bounds check

					WriteRelJump(0x7F231E, 0x7F236B); // Don't direction tiles based on their screen position

					WriteRelJump(0x7F26F0, 0x7F27AD); // Skip screen bound checks

					// Reduce tile safezones to prevent them from jumping around too much
					SafeWrite32(0x7F23A6 + 2, uint32_t(&fTileSafeZoneX));
					SafeWrite32(0x7F23AF + 2, uint32_t(&fTileSafeZoneY));
				}
			}
		}

	}

}

#pragma endregion