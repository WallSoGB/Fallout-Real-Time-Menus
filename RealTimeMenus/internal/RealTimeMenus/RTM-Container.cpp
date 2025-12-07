#include "RTM-Container.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BGSOpenCloseForm.hpp"
#include "Bethesda/ContainerMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Container {
		
		CallDetour kDestroyContainerMenuDetour;

		class Hook {
		public:
			void DestroyContainerMenu() {
				ContainerMenu* pMenu = reinterpret_cast<ContainerMenu*>(this);
				TESObjectREFR* pContainerRef = pMenu->pContainer;
				if (pContainerRef && Utils::HasOpenCloseAnims(pContainerRef, false)) {
					Utils::CloseReference(pContainerRef);
				}
				ThisCall(kDestroyContainerMenuDetour.GetOverwrittenAddr(), this); // Menu destructor
			}

			static void __fastcall EquipBestArmor(Actor* apActor, TESBoundObject* apObject) {
				if (apObject->GetFormType() != FORM_TYPE::TESObjectARMO)
					return;

				TESBoundObject* pActorBase = apActor->GetObjectReference();
				if (!pActorBase)
					return;

				auto eType = pActorBase->GetFormType();
				if (eType == FORM_TYPE::TESNPC) {
					ThisCall(0x606540, pActorBase, apActor, apActor->GetBiped(), true); // TESNPC::InitParts
				}
				else if (eType == FORM_TYPE::TESCreature) {
					ThisCall<bool>(0x5F9E00, pActorBase, apActor, true, false, true); // TESCreature::InitDefaultWorn
				}
			}
		};

		__declspec(naked) void EquipBestArmorOnAdd_Asm() {
			static constexpr uint32_t uiJumpTarget = 0x89271F;
			__asm {
				mov		edx, [ebp + 0x8]
				mov		ecx, [ebp - 0x44]
				call	Hook::EquipBestArmor
				mov		dword ptr[ebp - 0x20], 0
				jmp		uiJumpTarget;
			}
		}

		__declspec(naked) void EquipBestArmorOnRemove_Asm() {
			static constexpr uint32_t uiJumpTarget = 0x575274;
			__asm {
				mov		edx, [ebp + 0x8]
				mov		ecx, [ebp - 0x14]
				call	Hook::EquipBestArmor
				mov		ecx, [ebp + 0x8]
				call	TESForm::GetFormType
				jmp		uiJumpTarget;
			}
		}

	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace Container {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			if (!Settings::bPauseContainers) {
				// Play container close anim on menu close
				PatchMemoryNop(0x5173B8, 5);
				kDestroyContainerMenuDetour.ReplaceCallEx(0x75B2FB, &Hook::DestroyContainerMenu);

				// Equip best armor on add/remove
				WriteRelJump(0x892718, uint32_t(EquipBestArmorOnAdd_Asm));
				WriteRelJump(0x57526C, uint32_t(EquipBestArmorOnRemove_Asm));

				PatchMemoryNop(0x887BFA, 2); // Remove Menu check in Actor::UpdateNonRenderSafe
			}
		}

	}

}

#pragma endregion