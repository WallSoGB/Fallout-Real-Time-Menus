#include "RTM-Terminal.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BGSOpenCloseForm.hpp"
#include "Bethesda/ComputersMenu.hpp"
#include "Bethesda/HackingMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Terminal {

		CallDetour kDestroyComputersMenuDetour;
		CallDetour kDestroyHackingMenuDetour;
		CallDetour kFixHackingMenuDetour;

		class Hook {
		public:
			void DestroyComputersMenu() {
				ComputersMenu* pMenu = reinterpret_cast<ComputersMenu*>(this);
				TESObjectREFR* pComputerRef = pMenu->pTargetRef;
				if (pComputerRef && !CdeclCall<bool>(0x501990, pComputerRef) && Utils::HasOpenCloseAnims(pComputerRef, false)) {
					Utils::CloseReference(pComputerRef);
				}
				ThisCall(kDestroyComputersMenuDetour.GetOverwrittenAddr(), this); // Menu destructor
			}

			void DestroyHackingMenu() {
				HackingMenu* pMenu = reinterpret_cast<HackingMenu*>(this);
				TESObjectREFR* pComputerRef = pMenu->pTargetRef;
				if (!ComputersMenu::GetSingleton() && pComputerRef && Utils::HasOpenCloseAnims(pComputerRef, false)) {
					Utils::CloseReference(pComputerRef);
				}
				ThisCall(kDestroyHackingMenuDetour.GetOverwrittenAddr(), this); // Menu destructor
			}

			Menu* FixHackingMenu() {
				ThisCall(kFixHackingMenuDetour.GetOverwrittenAddr(), this);
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

		}

		void InitDeferredHooks() {
			// Fix hacking menu not initializing form pointers to 0
			kFixHackingMenuDetour.ReplaceCallEx(0x76551B, &Hook::FixHackingMenu);

			if (!Settings::bPauseComputerMenu) {
				// Play terminal close anim on menu close
				PatchMemoryNop(0x501724, 5);
				kDestroyComputersMenuDetour.ReplaceCallEx(0x757B54, &Hook::DestroyComputersMenu);
				kDestroyHackingMenuDetour.ReplaceCallEx(0x765B2C, &Hook::DestroyHackingMenu);
			}
		}

	}

}

#pragma endregion