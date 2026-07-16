#include "RTM-Terminal.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BGSOpenCloseForm.hpp"
#include "Bethesda/ComputersMenu.hpp"
#include "Bethesda/HackingMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/Interface.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Terminal {

		HookUtils::CallDetour kDestroyComputersMenuDetour;
		HookUtils::CallDetour kDestroyHackingMenuDetour;
		HookUtils::CallDetour kFixHackingMenuDetour;

		class Hook {
		public:
			void DestroyComputersMenu() {
				ComputersMenu* pMenu = reinterpret_cast<ComputersMenu*>(this);
				TESObjectREFR* pComputerRef = pMenu->pTargetRef;
				if (pComputerRef && !CdeclCall<bool>(0x501990, pComputerRef) && Utils::HasOpenCloseAnims(pComputerRef, false)) {
					Utils::CloseReference(pComputerRef);
				}
				ThisCall(kDestroyComputersMenuDetour, this); // Menu destructor
			}

			void DestroyHackingMenu() {
				HackingMenu* pMenu = reinterpret_cast<HackingMenu*>(this);
				TESObjectREFR* pComputerRef = pMenu->pTargetRef;
				if (!ComputersMenu::GetSingleton() && pComputerRef && Utils::HasOpenCloseAnims(pComputerRef, false)) {
					Utils::CloseReference(pComputerRef);
				}
				ThisCall(kDestroyHackingMenuDetour, this); // Menu destructor
			}

			Menu* FixHackingMenu() {
				ThisCall(kFixHackingMenuDetour, this);
				HackingMenu* pMenu = reinterpret_cast<HackingMenu*>(this);
				pMenu->pTargetRef = nullptr;
				pMenu->pTargetBaseRef = nullptr;
				return pMenu;
			}
		};

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace Terminal {

		void InitHooks() {
			// Fix hacking menu not initializing form pointers to 0
			kFixHackingMenuDetour.ReplaceCall(0x76551B, &Hook::FixHackingMenu);
		}

		void InitDeferredHooks() {
			if (!Settings::IsMenuPaused(Interface::Computers)) {
				// Play terminal close anim on menu close
				HookUtils::PatchMemoryNop(0x501724, 5);
				kDestroyComputersMenuDetour.ReplaceCall(0x757B54, &Hook::DestroyComputersMenu);
				kDestroyHackingMenuDetour.ReplaceCall(0x765B2C, &Hook::DestroyHackingMenu);
			}
		}

	}

}

#pragma endregion