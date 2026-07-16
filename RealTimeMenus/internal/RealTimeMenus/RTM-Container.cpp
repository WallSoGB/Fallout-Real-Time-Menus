#include "RTM-Container.hpp"
#include "RTM-Settings.hpp"
#include "RTM-Utils.hpp"

#include "Bethesda/BGSOpenCloseForm.hpp"
#include "Bethesda/ContainerMenu.hpp"
#include "Bethesda/PlayerCharacter.hpp"
#include "Bethesda/Interface.hpp"

#pragma region Code

namespace RealTimeMenus {

	namespace Container {
		
		uint32_t uiHandleActivateAddr = 0x47A560;

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

		HookUtils::CallDetour kDestroyContainerMenuDetour;
		HookUtils::CallDetour kEquipBestArmorOnAddDetour;
		HookUtils::CallDetour kEquipBestArmorOnRemoveDetour;
		class Hook {
		public:
			void DestroyContainerMenu() {
				ContainerMenu* pMenu = reinterpret_cast<ContainerMenu*>(this);
				TESObjectREFR* pContainerRef = pMenu->pContainer;
				if (pContainerRef && Utils::HasOpenCloseAnims(pContainerRef, false)) {
					Utils::CloseReference(pContainerRef, uiHandleActivateAddr);
				}
				ThisCall(kDestroyContainerMenuDetour, this); // Menu destructor
			}

			FORM_TYPE EquipBestArmorOnAdd() {
				TESBoundObject* pObject = reinterpret_cast<TESBoundObject*>(this);
				uint8_t* pEBP = HookUtils::GetParentBasePtr();
				Actor* pActor = *reinterpret_cast<Actor**>(pEBP - 0x44);
				EquipBestArmor(pActor, pObject);
				return ThisCall<FORM_TYPE>(kEquipBestArmorOnAddDetour, pObject);
			}

			FORM_TYPE EquipBestArmorOnRemove() {
				TESBoundObject* pObject = reinterpret_cast<TESBoundObject*>(this);
				uint8_t* pEBP = HookUtils::GetParentBasePtr();
				Actor* pActor = *reinterpret_cast<Actor**>(pEBP - 0x14);
				EquipBestArmor(pActor, pObject);
				return ThisCall<FORM_TYPE>(kEquipBestArmorOnRemoveDetour, pObject);
			}
		};
	}

}

#pragma endregion



#pragma region Hooks

namespace RealTimeMenus {

	namespace Container {

		void InitHooks() {

		}

		void InitDeferredHooks() {
			if (!Settings::IsMenuPaused(Interface::Container)) {
				// Play container close anim on menu close
				uiHandleActivateAddr = HookUtils::GetRelJumpAddr(0x5173B8);
				HookUtils::PatchMemoryNop(0x5173B8, 5);
				kDestroyContainerMenuDetour.ReplaceCall(0x75B2FB, &Hook::DestroyContainerMenu);

				// Equip best armor on add/remove
				kEquipBestArmorOnAddDetour.ReplaceCall(0x892722, &Hook::EquipBestArmorOnAdd);
				kEquipBestArmorOnRemoveDetour.ReplaceCall(0x57526F, &Hook::EquipBestArmorOnRemove);

				HookUtils::PatchMemoryNop(0x887BFA, 2); // Remove Menu check in Actor::UpdateNonRenderSafe
			}
		}

	}

}

#pragma endregion