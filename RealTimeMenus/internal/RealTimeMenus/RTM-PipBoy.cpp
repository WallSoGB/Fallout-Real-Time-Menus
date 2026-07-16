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

		HookUtils::CallDetour kGetFirstInitDoneDetour;
		HookUtils::CallDetour kUpdateRadLevelDetour;
		HookUtils::CallDetour kIsTopMenuIDDetour;
		class Hook {
		public:
			void UpdatePipBoyEffectList() {
				ThisCall(kUpdateRadLevelDetour, this); // StatsMenu::UpdateRadLevel

				// Update effect list if it's open
				if (ThisCall<uint32_t>(0x7DCC00, this) == 0 && ThisCall<int32_t>(0x7DCC40, this) == 2) { // GetCurrentTab and GetStatusPage
					ThisCall(0x7DDF00, this); // UpdateActiveEffects
				}
			}

			static bool UpdateMenuMapTime(Interface::Menus aeMenuCode) {
				bool bTop = CdeclCall<bool>(kIsTopMenuIDDetour, aeMenuCode);
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
					bool bCanUpdateMap = Interface::GetPipboyManager()->bMenuRendering && !MapMenu::ShouldRenderLocalMap();
					if (bCanUpdateMap && pMapMenu->pTileWorldMap && (uiCurrentTab == 32 || uiCurrentTab == 33)) {
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

			bool UpdateInventoryActorValueHook() {
				bool bFirstInit = ThisCall<bool>(kGetFirstInitDoneDetour, this);
				if (bFirstInit) {
					uint8_t* pEBP = HookUtils::GetParentBasePtr();
					const ActorValue::Index eActorValue = *reinterpret_cast<ActorValue::Index*>(pEBP + 0x8);
					if (eActorValue == ActorValue::HEALTH || eActorValue == ActorValue::DAMAGE_RESISTANCE || eActorValue == ActorValue::DAMAGE_THRESHOLD || eActorValue == ActorValue::INVENTORY_WEIGHT) {
						if (Interface::GetPipboyManager() && Interface::GetPipboyManager()->bIsInitialized && InventoryMenu::GetSingleton() && Interface::IsMenuIDVisible(Interface::Inventory, 0xB)) {
							PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();
							char cBuf[128];
							switch (eActorValue) {
								case ActorValue::HEALTH:
									{
										our_snprintf(cBuf, sizeof(cBuf), "%d/%d", pPlayer->GetActorValueI(eActorValue), pPlayer->GetPermanentActorValueI(eActorValue));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerHPInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								case ActorValue::DAMAGE_RESISTANCE:
									{
										our_snprintf(cBuf, sizeof(cBuf), "%d", int32_t(std::min(pPlayer->GetActorValueF(eActorValue), GameSettingCollection::fMaxArmorRating->Float())));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerDRInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								case ActorValue::DAMAGE_THRESHOLD:
									{
										int32_t iBaseDR = pPlayer->GetActorValueI(eActorValue);
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
										our_snprintf(cBuf, sizeof(cBuf), "%d/%d", pPlayer->GetActorValueI(eActorValue), int32_t(pPlayer->GetMaxCarryWeight()));
										reinterpret_cast<Tile*>(InventoryMenu::GetSingleton()->pTilePlayerWGInfo)->SetValue(InventoryMenu::uiValueTrait, cBuf, true);
									}
									break;
								default:
									break;
							}
						}
					}
				}
				return bFirstInit;
			}
		};

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace PipBoy {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			if (!Settings::IsMenuPaused(Interface::PipBoy)) {
				// Update actor values in the inventory menu
				kGetFirstInitDoneDetour.ReplaceCall(0x704E2A, &Hook::UpdateInventoryActorValueHook);

				// Update effects' info in the stats menu
				kUpdateRadLevelDetour.ReplaceCall(0x7DC544, &Hook::UpdatePipBoyEffectList);

				// Update time in map menu
				kIsTopMenuIDDetour.ReplaceCall(0x79A23E, Hook::UpdateMenuMapTime);

				HookUtils::PatchMemoryNop(0x86E7B4, 2); // Don't report PipBoy opening as menumode
				HookUtils::PatchMemoryNop(0x70F042, 5); // Don't mute sounds when opening PipBoy.
			}

			if (Settings::bHitEffectInPipBoy) {
				HookUtils::PatchMemoryNop(0x70F01B, 5); // Don't clear hit effects when opening PipBoy
				HookUtils::PatchMemoryNopRange(0x7F8126, 0x7F813B); // Don't clear, nor block hit effects on FOPipboyManager open
			}
		}

	}

}

#pragma endregion