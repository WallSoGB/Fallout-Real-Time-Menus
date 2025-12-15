#include "RTM-Utils.hpp"
#include "RTM-Settings.hpp"

#include "Bethesda/BarterMenu.hpp"
#include "Bethesda/ComputersMenu.hpp"
#include "Bethesda/ContainerMenu.hpp"
#include "Bethesda/DialogMenu.hpp"
#include "Bethesda/FixedStrings.hpp"
#include "Bethesda/GameSettingCollection.hpp"
#include "Bethesda/HackingMenu.hpp"
#include "Bethesda/Interface.hpp"
#include "Bethesda/InterfaceManager.hpp"
#include "Bethesda/LockPickMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/PlayerMover.hpp"
#include "Bethesda/RepairServicesMenu.hpp"
#include "Bethesda/StartMenu.hpp"
#include "Bethesda/VATSMenu.hpp"
#include "Gamebryo/NiBound.hpp"
#include "Gamebryo/NiControllerManager.hpp"
#include "Obsidian/CompanionWheelMenu.hpp"
#include "Obsidian/RecipeMenu.hpp"

namespace RealTimeMenus {

	namespace Utils {

#define HANLDE_QUANTITY_MENU 1

		TESObjectREFR* pActivatedReference = nullptr;

		bool __fastcall IsMenuActive(Interface::Menus aeMenu, Interface::Menus aeTarget) {
			if (aeMenu == aeTarget)
				return true;

			if (Interface::IsMenuIDVisible(aeTarget, 0xFF))
				return true;

			return false;
		}

		bool IsLiveMenuInstanceAlive(uint32_t aeMenu) {
			switch (aeMenu) {
				case Interface::Menus::MainFour:
				case Interface::Menus::Inventory:
				case Interface::Menus::Stats:
				case Interface::Menus::PipboyData:
				case Interface::Menus::PipboyRepair:
				case Interface::Menus::ItemModMenu:
					return Interface::IsInPipBoy();
				case Interface::Menus::Dialog:
					return DialogMenu::GetSingleton();
				case Interface::Menus::Container:
					return ContainerMenu::GetSingleton();
				case Interface::Menus::Barter:
					return BarterMenu::GetSingleton();
				case Interface::Menus::VendorRepair:
					return RepairServicesMenu::GetSingleton();
				case Interface::Menus::Recipe:
					return RecipeMenu::GetSingleton();
				case Interface::Menus::LockPick:
					return LockPickMenu::GetSingleton();
				case Interface::Menus::VATS:
					return VATSMenu::GetSingleton();
				case Interface::Menus::Hacking:
					return HackingMenu::GetSingleton();
				case Interface::Menus::Computers:
					return ComputersMenu::GetSingleton();
				case Interface::Menus::CompanionWheel:
					return CompanionWheelMenu::GetSingleton();
				default:
					return false;
			}
		}

		bool IsAnyLiveMenuInstanceAlive(bool abGameModeCheck = false) {
			if (!Settings::bPausePipBoy && Interface::IsInPipBoy())
				return true;

			if (!Settings::bPauseDialogue && (!abGameModeCheck || Settings::bGameModeInDialogue)) {
				if (DialogMenu::GetSingleton())
					return true;
			}

			if (!Settings::bPauseContainers) {
				if (ContainerMenu::GetSingleton())
					return true;
				if (BarterMenu::GetSingleton())
					return true;
				if (RepairServicesMenu::GetSingleton())
					return true;
				if (RecipeMenu::GetSingleton())
					return true;
			}

			if (!Settings::bPauseLockPickMenu && LockPickMenu::GetSingleton())
				return true;

			if (!Settings::bPauseVATS && VATSMenu::GetSingleton())
				return true;

			if (!Settings::bPauseComputerMenu) {
				if (HackingMenu::GetSingleton())
					return true;
				if (ComputersMenu::GetSingleton())
					return true;
			}

			if (!Settings::bPauseCompanionWheel && CompanionWheelMenu::GetSingleton())
				return true;

			return false;
		}

		MenuPauseState IsLiveMenu(uint32_t aeMenu, bool abCheckInstances, bool abGameModeCheck) {
			switch (aeMenu) {
				case Interface::Menus::NoMenu:
					if (abCheckInstances)
						return IsAnyLiveMenuInstanceAlive(abGameModeCheck) ? MenuPauseState::MENU_MIXED : MenuPauseState::MENU_PAUSED;
					else
						return MenuPauseState::MENU_PAUSED;
#if HANLDE_QUANTITY_MENU
				case Interface::Menus::Quantity:
					if (abCheckInstances)
						return IsLiveMenu(Interface::Menus::NoMenu, abCheckInstances, abGameModeCheck);
					return MenuPauseState::MENU_PAUSED;
#endif
				case Interface::Menus::MainFour:
				case Interface::Menus::Inventory:
				case Interface::Menus::Stats:
				case Interface::Menus::PipboyData:
				case Interface::Menus::PipboyRepair:
				case Interface::Menus::ItemModMenu:
					return Settings::bPausePipBoy ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				case Interface::Menus::Dialog:
					if (Settings::bPauseDialogue)
						return MenuPauseState::MENU_PAUSED;
					else if (Settings::bGameModeInDialogue)
						return MenuPauseState::MENU_LIVE;
					else
						return MenuPauseState::MENU_MIXED;
				case Interface::Menus::Container:
				case Interface::Menus::Barter:
				case Interface::Menus::VendorRepair:
				case Interface::Menus::Recipe: 
					// Dialogue menu is a special case, because it can enter "Service mode", where another menu is opened from within dialogue
					if (abCheckInstances && IsLiveMenuInstanceAlive(Interface::Menus::Dialog)){
						auto eState = IsLiveMenu(Interface::Menus::Dialog, abCheckInstances, abGameModeCheck);
						// Let service menu override live state if dialogue is live
						if (eState >= MenuPauseState::MENU_LIVE && Settings::bPauseContainers)
							return MenuPauseState::MENU_PAUSED;
						else
							return eState;
					}
					return Settings::bPauseContainers ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				case Interface::Menus::LockPick:
					return Settings::bPauseLockPickMenu ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				case Interface::Menus::VATS:
					return Settings::bPauseVATS ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				case Interface::Menus::Hacking:
				case Interface::Menus::Computers:
					return Settings::bPauseComputerMenu ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				case Interface::Menus::CompanionWheel:
					return Settings::bPauseCompanionWheel ? MenuPauseState::MENU_PAUSED : MenuPauseState::MENU_LIVE;
				default:
					return MenuPauseState::MENU_PAUSED;
			}
		}

		bool IsMenuPausingGame() {
			if (Interface::IsInMenuMode()) {
				if (Interface::IsConsoleVisible())
					return true;

				const Interface::Menus eMenu = Interface::GetTopMenuID();
				MenuPauseState eState = IsLiveMenu(eMenu, true);
				if (eState != MenuPauseState::MENU_PAUSED)
					return false;

				return true;
			}

			return false;
		}

		bool IsMenuBlockingGameMode() {
			if (Interface::IsInMenuMode()) {
				if (Interface::IsConsoleVisible())
					return true;

				const Interface::Menus eMenu = Interface::GetTopMenuID();
				MenuPauseState eState = IsLiveMenu(eMenu, true, true);
				if (eState == MenuPauseState::MENU_LIVE)
					return false;

				return true;
			}

			return false;
		}

		TESForm* GetCurrentMenuBackgroundFX() {
			constexpr uint32_t PAUSE_BACKGROUND_FX		= 0x4EEE8;
			constexpr uint32_t PIPBOY_BACKGROUND_FX		= 0x96389;
			constexpr uint32_t INTERFACE_BACKGROUND_FX	= 0x44F34;
			constexpr uint32_t POPUP_BACKGROUND_FX		= 0x32B38;

			uint32_t eMenu = Interface::GetTopMenuID();
			if (StartMenu::PauseMenuIsActive())
				return TESForm::GetFormByNumericID(PAUSE_BACKGROUND_FX);

			if (PlayerCharacter::GetSingleton()->HasPipBoyOpen())
				return Settings::bBackgroundBlurInPipBoy ? TESForm::GetFormByNumericID(PIPBOY_BACKGROUND_FX) : nullptr;

			if (eMenu == Interface::Menus::LockPick || (eMenu == Interface::Menus::NoMenu && LockPickMenu::GetSingleton()))
				return Settings::bBackgroundBlurInLockPickMenu ? TESForm::GetFormByNumericID(INTERFACE_BACKGROUND_FX) : nullptr;

			InterfaceManager* pUIMgr = InterfaceManager::GetSingleton();
			if (!pUIMgr->pMenuRoot->GetChildByName("Player Name Entry Menu")) {
				if (eMenu == Interface::Menus::Quantity)
					eMenu = Interface::Menus::NoMenu;

				if (eMenu != Interface::Menus::NoMenu) {
					switch (eMenu) {
						case Interface::Menus::Container:
						case Interface::Menus::Barter:
						case Interface::Menus::VendorRepair:
						case Interface::Menus::Recipe:
							if (!Settings::bBackgroundBlurInContainers)
								return nullptr;
							break;
						case Interface::Menus::Hacking:
						case Interface::Menus::Computers:
							if (!Settings::bBackgroundBlurInComputerMenu)
								return nullptr;
							break;
						case Interface::Menus::CompanionWheel:
							if (!Settings::bBackgroundBlurInCompanionWheel)
								return nullptr;
							break;
						default:
							break;
					}
				}
				else {
					if (!Settings::bBackgroundBlurInContainers) {
						if (Utils::IsLiveMenuInstanceAlive(Interface::Menus::Container)
							|| Utils::IsLiveMenuInstanceAlive(Interface::Menus::Barter)
							|| Utils::IsLiveMenuInstanceAlive(Interface::Menus::VendorRepair)
							|| Utils::IsLiveMenuInstanceAlive(Interface::Menus::Recipe))
							return nullptr;
					}

					if (!Settings::bBackgroundBlurInComputerMenu) {
						if (Utils::IsLiveMenuInstanceAlive(Interface::Menus::Hacking)
							|| Utils::IsLiveMenuInstanceAlive(Interface::Menus::Computers))
							return nullptr;
					}

					if (!Settings::bBackgroundBlurInCompanionWheel) {
						if (Utils::IsLiveMenuInstanceAlive(Interface::Menus::CompanionWheel))
							return nullptr;
					}
				}

				return TESForm::GetFormByNumericID(POPUP_BACKGROUND_FX);
			}

			return nullptr;
		}

		void __fastcall UpdatePlayerMovement(PlayerCharacter* apPlayer, float afDelta, bool abUpdateAnims, bool abFullUpdate, bool abUpdateMovement) {
			BaseProcess* pProcess = apPlayer->GetCurrentProcess();
			ThisCall(0xCA1410, apPlayer->pRagdollPenetrationUtil); // bhkRagdollPenetrationUtil::Update

			static_cast<PlayerMover*>(apPlayer->pActorMover)->SetSpeedVector(NiPoint3::ZERO);
			if (abUpdateMovement)
				apPlayer->pActorMover->Update(afDelta);

			pProcess->UpdateKnockState(apPlayer);

			if (abFullUpdate) [[likely]] {
				if (abUpdateAnims) {
					apPlayer->UpdateAnimation();

					if (apPlayer->b3rdPerson)
						apPlayer->PickAnimations(1.f, 1.f);
					apPlayer->b3rdPerson = !apPlayer->b3rdPerson;
					if (apPlayer->b3rdPerson)
						apPlayer->PickAnimations(1.f, 1.f);
				}
				else {
					apPlayer->b3rdPerson = !apPlayer->b3rdPerson;
				}

				ThisCall(0x8D3550, apPlayer, afDelta); // Character::Update
				if (abUpdateAnims)
					apPlayer->UpdateAnimationMovement(apPlayer->GetAnimation(!apPlayer->b3rdPerson));

				apPlayer->b3rdPerson = !apPlayer->b3rdPerson;

				ThisCall(0x8D3550, apPlayer, afDelta); // Character::Update
				if (abUpdateAnims)
					apPlayer->UpdateAnimationMovement(apPlayer->GetAnimation(!apPlayer->b3rdPerson));
			}
		}

		bool __fastcall IsRefOutOfReach(const PlayerCharacter* apPlayer, const TESObjectREFR* apRef, float afReachBonus = 0.f) {
			const NiAVObject* pTargetRoot = apRef->Get3D();
			const NiBound* pTargetBound = pTargetRoot->GetWorldBound();

			NiPoint3 kViewPos;
			if (PlayerCharacter::p1stPersonCamera.Get()) [[likely]] {
				kViewPos = apPlayer->kCamera1stPos;
			}
			else {
				kViewPos = apPlayer->GetLocationOnReference();
				kViewPos.z += apPlayer->GetScale() * apPlayer->fEyeHeight;
			}

			if (apPlayer->bIs3rdPerson)
				kViewPos += apPlayer->kPtD58;

			float fContainerDistance = (pTargetBound->m_kCenter - kViewPos).Length() - pTargetBound->m_fRadius;
			return fContainerDistance > GameSettingCollection::iActivatePickLength->Int() + afReachBonus;
		}

		void CloseContainerMenu() {
			ContainerMenu::Close();
			pActivatedReference = nullptr;
		}

		void __fastcall ShouldCloseContainerMenu(PlayerCharacter* apPlayer, ContainerMenu* apContainerMenu) {
			const TESObjectREFR* pContainerRef = apContainerMenu->pContainer;

			if (apPlayer->IsKnockedOut() || apPlayer->IsUnconscious() || pContainerRef->GetDisabled() || pContainerRef->GetDeleted()) {
				CloseContainerMenu();
				return;
			}

			if (pContainerRef->IsMobileObject()) [[unlikely]] {
				const MobileObject* pObject = static_cast<const MobileObject*>(pContainerRef);
				if (pObject->GetNeedtoChangeProcess() || (pObject->GetCurrentProcess() && pObject->GetCurrentProcess()->eProcessLevel != PROCESS_TYPE::HIGH)) {
					CloseContainerMenu();
					return;
				}
			}

			float fReachBonus = 0.f;
			if (Settings::uiCompanionMenuHandling && pContainerRef->IsActor() && static_cast<const Actor*>(pContainerRef)->IsPlayerTeammate()) {
				switch (Settings::uiCompanionMenuHandling) {
					case COMPANION_MENU_EXTEND_CHECK:
						fReachBonus = 256.0f;
						break;
					case COMPANION_MENU_SKIP_CHECK:
						return;
					default:
						break;
				}
			}


			if (IsRefOutOfReach(apPlayer, pContainerRef, fReachBonus))
				CloseContainerMenu();
		}

		void CloseLockPickMenu() {
			LockPickMenu::Close();
			pActivatedReference = nullptr;
		}

		void __fastcall ShouldCloseLockPickMenu(PlayerCharacter* apPlayer, LockPickMenu* apLockPickMenu) {
			const TESObjectREFR* pLockRef = apLockPickMenu->pLockRef;
			if (apPlayer->IsKnockedOut() || apPlayer->IsUnconscious() || pLockRef->GetDisabled() || pLockRef->GetDeleted()) {
				CloseLockPickMenu();
				return;
			}

			if (IsRefOutOfReach(apPlayer, pLockRef))
				CloseLockPickMenu();
		}

		void __fastcall ShouldCloseMenus(PlayerCharacter* apPlayer) {
			ContainerMenu* pContainerMenu = ContainerMenu::GetSingleton();
			if (InterfaceManager::GetSingleton()->IsMenuInStack(Interface::Container) && pContainerMenu && pContainerMenu->pContainer && pContainerMenu->pContainer == pActivatedReference) {
				ShouldCloseContainerMenu(apPlayer, pContainerMenu);
			}

			LockPickMenu* pLockPickMenu = LockPickMenu::GetSingleton();
			if (InterfaceManager::GetSingleton()->IsMenuInStack(Interface::LockPick) && pLockPickMenu && pLockPickMenu->pLockRef && pLockPickMenu->pLockRef == pActivatedReference) {
				ShouldCloseLockPickMenu(apPlayer, pLockPickMenu);
			}
		}

		void __fastcall DeactivateOpenAnim(TESObjectREFR* apReference) {
			if (!apReference)
				return;

			NiAVObject* pRoot = apReference->Get3D();
			if (pRoot && pRoot->GetControllers()) {
				NiControllerManager* pCtrlMgr = pRoot->GetController<NiControllerManager>();
				NiControllerSequence* pOpenSequence = pCtrlMgr ? pCtrlMgr->GetSequenceByName(FixedStrings::GetOpen()) : nullptr;
				if (pOpenSequence)
					pOpenSequence->Deactivate(0.f, false);
			}
		}

		bool __fastcall HasOpenCloseAnims(const TESObjectREFR* apReference, bool abClose) {
			if (!apReference)
				return false;

			NiAVObject* pRoot = apReference->Get3D();
			if (pRoot && pRoot->GetControllers()) {
				NiControllerManager* pCtrlMgr = pRoot->GetController<NiControllerManager>();
				if (!pCtrlMgr)
					return false;

				return pCtrlMgr->GetSequenceByName(abClose ? FixedStrings::GetClose() : FixedStrings::GetOpen());
			}

			return false;
		}

		void __fastcall CloseReference(TESObjectREFR* apReference) {
			auto eOpenState = BGSOpenCloseForm::GetOpenState(apReference);
			if (eOpenState == BGSOpenCloseForm::OpenCloseState::OPEN || eOpenState == BGSOpenCloseForm::OpenCloseState::OPENING)
				Utils::DeactivateOpenAnim(apReference);
			BGSOpenCloseForm::HandleActivate(apReference, PlayerCharacter::GetSingleton(), nullptr);
		}
	}

}