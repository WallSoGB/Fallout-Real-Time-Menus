#include "RTM-PipBoy.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/ActorValue.hpp"
#include "Bethesda/Calendar.hpp"
#include "Bethesda/GameSettingCollection.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/InterfaceManager.hpp"
#include "Bethesda/InventoryMenu.hpp"
#include "Bethesda/MapMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/ItemChange.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace PipBoy {

		class Hook {
		public:
			void UpdatePipBoyEffectList() {
				ThisCall(0x7DDB00, this); // StatsMenu::UpdateRadLevel

				// Update effect list if it's open
				if (ThisCall<uint32_t>(0x7DCC00, this) == 0 && ThisCall<int32_t>(0x7DCC40, this) == 2) { // GetCurrentTab and GetStatusPage
					ThisCall(0x7DDF00, this); // UpdateActiveEffects
				}
			}

			static bool UpdateMenuMapTime(Interface::Menus aeMenuCode) {
				bool bTop = Interface::IsTopMenuID(aeMenuCode);
				if (bTop) {
					static int32_t iLastMinuteUpdate = -1;
					static int32_t iLastHourUpdate = -1;
					MapMenu* pMapMenu = MapMenu::GetSingleton();

					Calendar* pCalendar = Calendar::GetSingleton();
					const float fHour = pCalendar->GetHour();
					int32_t iHour = int32_t(fHour);
					int32_t iMinutes = (fHour - iHour) * 60.f;

					if (iLastMinuteUpdate != iMinutes || iLastHourUpdate != iHour) {
						bool b12HourClock = *reinterpret_cast<uint8_t*>(0x79AC55) != 0x68;
						char cBuffer[256];
						iLastMinuteUpdate = iMinutes;
						iLastHourUpdate = iHour;

						BSString strDate = pCalendar->GetDateString();
						if (b12HourClock) {
							const char* pAM = GameSettingCollection::sAMTime->String();
							const char* pPM = GameSettingCollection::sPMTime->String();

							if (fHour >= 0.0 && fHour < 1.0)
								iHour = 12;
							else if (fHour >= 13.0)
								iHour -= 12;

							const char* pSuffix = (iHour >= 12) ? pPM : pAM;

							our_snprintf(cBuffer, sizeof(cBuffer), "%s, %d:%02d %s", strDate.c_str(), iHour, iMinutes, pSuffix);
						}
						else {
							our_snprintf(cBuffer, sizeof(cBuffer), "%s, %d:%02d", strDate.c_str(), iHour, iMinutes);
						}
						pMapMenu->pTimeDateInfo->SetValue(Tile::kTileValue_string, cBuffer, true);
					}

					const uint32_t uiCurrentTab = pMapMenu->uiCurrentTab;
					if (pMapMenu->pTileWorldMap && (uiCurrentTab == 32 || uiCurrentTab == 33)) {
						static uint32_t uiLastUpdateTime = 0;
						const uint32_t uiCurrentTime = GetTickCount();
						if (uiCurrentTime - uiLastUpdateTime >= 100) {
							pMapMenu->AddQuestMarkers();
							pMapMenu->AddCompanionMarkers();
							uiLastUpdateTime = uiCurrentTime;
						}
					}

					return true;
				}
				return false;
			}

			static uint8_t __fastcall UpdateInventoryActorValue(ActorValue::Index aeActorValue) {
				InterfaceManager* pManager = InterfaceManager::GetSingleton();
				uint8_t ucFlags = pManager->uiFlags;
				if (aeActorValue == ActorValue::HEALTH || aeActorValue == ActorValue::DAMAGE_RESISTANCE || aeActorValue == ActorValue::DAMAGE_THRESHOLD || aeActorValue == ActorValue::INVENTORY_WEIGHT) {
					if (ucFlags && Interface::GetPipboyManager() && Interface::GetPipboyManager()->bIsInitialized) {
						if (InventoryMenu::GetSingleton() && Interface::IsMenuIDVisible(Interface::Inventory, 0xB)) {
							PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();
							char cBuf[64];
							switch (aeActorValue) {
								case ActorValue::HEALTH:
									{
										our_snprintf(cBuf, sizeof(cBuf), "%d/%d", pPlayer->GetActorValueI(aeActorValue), pPlayer->GetPermanentActorValueI(aeActorValue));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerHPInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								case ActorValue::DAMAGE_RESISTANCE:
									{
										our_snprintf(cBuf, sizeof(cBuf), "%d", int32_t(std::min(pPlayer->GetActorValueF(aeActorValue), GameSettingCollection::fMaxArmorRating->Float())));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerDRInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								case ActorValue::DAMAGE_THRESHOLD:
									{
										int32_t iBaseDR = pPlayer->GetActorValueI(aeActorValue);
										ItemChange* pWeapon = pPlayer->GetCurrentProcess()->GetCurrentWeapon();
										TESForm* pWeaponForm = pWeapon ? pWeapon->pObject : TESForm::GetFormByNumericID(0x1F4); // Fists
										float fBonus = 0.f;
										CdeclCall(0x5E58F0, 0x38, pPlayer, pPlayer, pWeaponForm, &fBonus); // BGSEntryPoint::HandleEntryPoint
										our_snprintf(cBuf, sizeof(cBuf), "%1.1f", iBaseDR + fBonus);
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerDTInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								case ActorValue::INVENTORY_WEIGHT:
									{
										our_snprintf(cBuf, sizeof(cBuf), "%d/%d", pPlayer->GetActorValueI(aeActorValue), int32_t(pPlayer->GetMaxCarryWeight()));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerWGInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								default:
									break;
							}
						}
					}
				}
				return ucFlags;
			}
		};

		__declspec(naked) void UpdateInventoryActorValue_Asm() {
			static constexpr uint32_t uiJumpTargetExit			= 0x704EBD;
			static constexpr uint32_t uiJumpTargetUpdateStats	= 0x704E3A;
			__asm {
				mov		ecx, [ebp + 8]
				call	Hook::UpdateInventoryActorValue
				test	al, al
				jz		FAIL

				jmp		uiJumpTargetUpdateStats
				
				FAIL:
				jmp		uiJumpTargetExit
			}
		}

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace PipBoy {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			if (!Settings::bPausePipBoy) {
				// Update actor values in the inventory menu
				WriteRelJump(0x704E28, uint32_t(UpdateInventoryActorValue_Asm));

				// Update effects' info in the stats menu
				ReplaceCallEx(0x7DC544, &Hook::UpdatePipBoyEffectList);

				// Update time in map menu
				ReplaceCall(0x79A23E, Hook::UpdateMenuMapTime);

				PatchMemoryNop(0x86E7B4, 2); // Don't report PipBoy opening as menumode
				PatchMemoryNop(0x70F042, 5); // Don't mute sounds when opening PipBoy.
			}

			if (Settings::bHitEffectInPipBoy) {
				PatchMemoryNop(0x70F01B, 5); // Don't clear hit effects when opening PipBoy
				PatchMemoryNopRange(0x7F8126, 0x7F813B); // Don't clear, nor block hit effects on FOPipboyManager open
			}
		}

	}

}

#pragma endregion