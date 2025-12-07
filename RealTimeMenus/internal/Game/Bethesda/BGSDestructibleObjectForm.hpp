#pragma once

#include "BaseFormComponent.hpp"

class NiAVObject;
class BGSExplosion;
class BGSDebris;
class TESModelTextureSwap;
class TESFile;
class TESForm;

struct DestructibleObjectStage {
	struct ALIGN2 _DestructibleObjectStageFlags {
		enum Flags : uint16_t {
			CAP_DAMAGE		= 1u << 0,
			DISABLE_OBJECT	= 1u << 1,
			DESTROY_OBJECT	= 1u << 2,
		};

		bool bCapDamage		: 1;
		bool bDisableObject : 1;
		bool bDestroyObject : 1;
	};
	using DestructibleObjectStageFlags = _DestructibleObjectStageFlags::Flags;

	uint8_t									ucModelDamageStage;
	uint8_t									ucHealthPercentage;
	Bitfield<_DestructibleObjectStageFlags>	usFlags;
	uint32_t								uiSelfDamagePerSecond;
	BGSExplosion*							pExplosion;
	BGSDebris*								pDebris;
	uint32_t								uiDebrisCount;
	TESModelTextureSwap*					pReplacementModel;
};

ASSERT_SIZE(DestructibleObjectStage, 0x18);

struct DestructibleObjectData {
	uint32_t					uiHealth;
	uint8_t						ucNumStages;
	bool						bTargetable;
	DestructibleObjectStage**	pStagesArray;
};

class BGSDestructibleObjectForm : public BaseFormComponent {
public:
	BGSDestructibleObjectForm();
	~BGSDestructibleObjectForm();

	DestructibleObjectData* pData;
};

ASSERT_SIZE(BGSDestructibleObjectForm, 0x8);