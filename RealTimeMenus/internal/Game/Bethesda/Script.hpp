#pragma once

#include "TESForm.hpp"
#include "BSStringT.hpp"

class ExtraDataList;
class ScriptLocals;
class ScriptVariable;
class TESQuest;
class SCRIPT_REFERENCED_OBJECT;
class ScriptCompiler;
class ScriptCompileData;
struct ScriptOperator;

enum SCRIPT_VARIABLE_TYPE : uint8_t {
	eVarType_Float = 0, // ref is also zero
	eVarType_Integer,

	// NVSE, return values only
	eVarType_String,
	eVarType_Array,
	eVarType_Ref,

	eVarType_Invalid
};

enum SCRIPT_PARAMETER_TYPE : uint32_t {
	SCRIPT_PARAMETER_STRING =				0x00,
	SCRIPT_PARAMETER_INTEGER =				0x01,
	SCRIPT_PARAMETER_FLOAT =				0x02,
	SCRIPT_PARAMETER_OBJECTID =				0x03,	// GetItemCount				TESForm *, must pass IsInventoryObjectType and TESForm::Unk_3A
	SCRIPT_PARAMETER_OBJECTREF =			0x04,	// Activate					TESObjectREFR *, REFR-PFLA
	SCRIPT_PARAMETER_ACTORVALUE =			0x05,	// ModActorValue			uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_ACTOR =				0x06,	// ToggleAI					TESObjectREFR *, must pass IsActor (ACHR-ACRE)
	SCRIPT_PARAMETER_SPELLITEM =			0x07,	// AddSpell					TESForm *, must be either SpellItem or book
	SCRIPT_PARAMETER_AXIS =					0x08,	// Rotate					char *, immediate char, X Y Z
	SCRIPT_PARAMETER_CELL =					0x09,	// GetInCell				TESObjectCELL *, must be cell
	SCRIPT_PARAMETER_ANIMATIONGROUP =		0x0A,	// PlayGroup				uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_MAGICITEM =			0x0B,	// Cast						MagicItem *
	SCRIPT_PARAMETER_SOUND =				0x0C,	// Sound					TESForm *, FORM_TYPE::TESSound
	SCRIPT_PARAMETER_TOPIC =				0x0D,	// Say						TESForm *, FORM_TYPE::TESTopicog
	SCRIPT_PARAMETER_QUEST =				0x0E,	// ShowQuestVars			TESForm *, FORM_TYPE::TESQuest
	SCRIPT_PARAMETER_RACE =					0x0F,	// GetIsRace				TESForm *, FORM_TYPE::TESRace
	SCRIPT_PARAMETER_CLASS =				0x10,	// GetIsClass				TESForm *, FORM_TYPE::TESClass
	SCRIPT_PARAMETER_FACTION =				0x11,	// Faction					TESForm *, FORM_TYPE::TESFaction
	SCRIPT_PARAMETER_SEX =					0x12,	// GetIsSex					uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_GLOBAL =				0x13,	// GetGlobalValue			TESForm *, FORM_TYPE::TESGlobal
	SCRIPT_PARAMETER_FURNITURE =			0x14,	// IsCurrentFurnitureObj	TESForm *, FORM_TYPE::TESFurniture or FORM_TYPE::BGSListForm
	SCRIPT_PARAMETER_TESOBJECT =			0x15,	// PlaceAtMe				TESObject *, must pass TESForm::Unk_3A
	SCRIPT_PARAMETER_VARIABLENAME =			0x16,	// GetQuestVariable			only works in conditionals
	SCRIPT_PARAMETER_QUESTSTAGE =			0x17,	// SetStage					handled like integer
	SCRIPT_PARAMETER_MAPMARKER =			0x18,	// ShowMap					TESObjectREFR *, see ObjectRef
	SCRIPT_PARAMETER_ACTORBASE =			0x19,	// SetEssential				TESActorBase * (NPC / creature)
	SCRIPT_PARAMETER_CONTAINER =			0x1A,	// RemoveMe					TESObjectREFR *, see ObjectRef
	SCRIPT_PARAMETER_WORLDSPACE =			0x1B,	// CenterOnWorld			TESWorldSpace *, FORM_TYPE::TESWorldSpace
	SCRIPT_PARAMETER_CRIMETYPE =			0x1C,	// GetCrimeKnown			uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_AIPACKAGE =			0x1D,	// GetIsCurrentPackage		TESPackage *, FORM_TYPE::TESPackage
	SCRIPT_PARAMETER_COMBATSTYLE =			0x1E,	// SetCombatStyle			TESCombatStyle *, FORM_TYPE::TESCombatStyle
	SCRIPT_PARAMETER_MAGICEFFECT =			0x1F,	// HasMagicEffect			EffectSetting *
	SCRIPT_PARAMETER_FORMTYPE =				0x20,	// GetIsUsedItemType		uint8_t *, immediate uint16_t
	SCRIPT_PARAMETER_WEATHERID =			0x21,	// GetIsCurrentWeather		TESForm *, FORM_TYPE::TESWeather
	SCRIPT_PARAMETER_NPC =					0x22,	// unused					TESNPC *, FORM_TYPE::TESNPC
	SCRIPT_PARAMETER_OWNER =				0x23,	// IsOwner					TESForm *, FORM_TYPE::TESNPC or FORM_TYPE::TESFaction
	SCRIPT_PARAMETER_EFFECTSHADER =			0x24,	// PlayMagicShaderVisuals	TESForm *, FORM_TYPE::TESEffectShader
	SCRIPT_PARAMETER_FORMLIST	=			0x25,	// IsInList					FORM_TYPE::BGSListForm
	SCRIPT_PARAMETER_MENUICON =				0x26,	// unused					FORM_TYPE::BGSMenuIcon
	SCRIPT_PARAMETER_PERK =					0x27,	// Add Perk					FORM_TYPE::BGSPerk
	SCRIPT_PARAMETER_NOTE =					0x28,	// Add Note					FORM_TYPE::BGSNote
	SCRIPT_PARAMETER_MISCELLANEOUSSTAT =	0x29,	// ModPCMiscStat			uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_IMAGESPACEMODIFIER =	0x2A,	//							FORM_TYPE::TESImageSpaceModifier
	SCRIPT_PARAMETER_IMAGESPACE =			0x2B,	//							FORM_TYPE::TESImageSpace
	SCRIPT_PARAMETER_DOUBLE =				0x2C,	// 
	SCRIPT_PARAMETER_SCRIPTVARIABLE =		0x2D,	// 
	SCRIPT_PARAMETER_UNHANDLED2E =			0x2E,	// 
	SCRIPT_PARAMETER_ENCOUNTERZONE =		0x2F,	//							FORM_TYPE::BGSEncounterZone
	SCRIPT_PARAMETER_IDLEFORM =				0x30,	// 
	SCRIPT_PARAMETER_MESSAGE =				0x31,	//							FORM_TYPE::BGSMessage
	SCRIPT_PARAMETER_INVOBJORFORMLIST =		0x32,	// AddItem					IsInventoryObjectType or FORM_TYPE::BGSListForm
	SCRIPT_PARAMETER_ALIGNMENT =			0x33,	// GetIsAlignment			uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_EQUIPTYPE =			0x34,	// GetIsUsedEquipType		uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_NONFORMLIST =			0x35,	// GetIsUsedItem			TESForm::Unk_3A and not FORM_TYPE::BGSListForm
	SCRIPT_PARAMETER_SOUNDFILE =			0x36,	// PlayMusic				FORM_TYPE::BGSMusicType
	SCRIPT_PARAMETER_CRITICALSTAGE =		0x37,	// SetCriticalStage			uint32_t *, immediate uint16_t
	SCRIPT_PARAMETER_LEVELEDORBASECHAR =	0x38,	// AddNPCToLeveledList		NPC / LeveledCharacter
	SCRIPT_PARAMETER_LEVELEDORBASECREATURE =0x39,	// AddCreatureToLeveledList	Creature / LeveledCreature
	SCRIPT_PARAMETER_LEVELEDCHAR =			0x3A,	// AddNPCToLeveledList		FORM_TYPE::TESLevCharacter
	SCRIPT_PARAMETER_LEVELEDCREATURE =		0x3B,	// AddCreatureToLeveledList	FORM_TYPE::TESLevCreature
	SCRIPT_PARAMETER_LEVELEDITEM =			0x3C,	// AddItemToLeveledList		FORM_TYPE::TESLevItem
	SCRIPT_PARAMETER_ANYFORM =				0x3D,	// AddFormToFormList		any form
	SCRIPT_PARAMETER_REPUTATION =			0x3E,	//							FORM_TYPE::TESReputation
	SCRIPT_PARAMETER_CASINO =				0x3F,	//							FORM_TYPE::TESCasino
	SCRIPT_PARAMETER_CASINOCHIP =			0x40,	//							FORM_TYPE::TESCasinoChips
	SCRIPT_PARAMETER_CHALLENGE =			0x41,	//							FORM_TYPE::TESChallenge
	SCRIPT_PARAMETER_CARAVANMONEY =			0x42,	//							FORM_TYPE::TESCaravanMoney
	SCRIPT_PARAMETER_CARAVANCARD =			0x43,	//							FORM_TYPE::TESCaravanCard
	SCRIPT_PARAMETER_CARAVANDECK =			0x44,	//							FORM_TYPE::TESCaravanDeck
	SCRIPT_PARAMETER_REGION =				0x45,	//							FORM_TYPE::TESRegion

	// Custom NVSE types
	SCRIPT_PARAMETER_STRINGVAR =			0x01,
	SCRIPT_PARAMETER_ARRAY =				0x100,	// only usable with compiler override; StandardCompile() will report unrecognized param type
};

struct SCRIPT_HEADER {
	uint32_t	uiUnusedVariableCount;
	uint32_t	uiNumRefs;
	uint32_t	uiDataLength;
	uint32_t	uiVarCount;
	uint16_t	usType;
	bool		bCompiled;
	uint8_t		unk13;
};

class Script : public TESForm {
public:
	Script();
	virtual ~Script();

	enum Type {
		OBJECT	= 0,
		QUEST	= 1,
		MAGIC	= 0x100,
		UNK		= 0x10000,
	};

	SCRIPT_HEADER								kHeader;
	char*										pText;
	uint8_t*									pData;
	float										fProfilerTimer;
	float										fQuestScriptDelay;
	float										fQuestScriptGetSecondsBuffer;
	TESQuest*									pQuest;
	BSSimpleList<SCRIPT_REFERENCED_OBJECT*>		kReferences;
	BSSimpleList<ScriptVariable*>				kVariables;
};

ASSERT_SIZE(Script, 0x54);

class SCRIPT_LOCAL {
public:
	uint32_t				uiVarID;
	SCRIPT_LOCAL*			pNext;
	union {
		double				dValue;
		uint32_t			uiFormID;
	};
	SCRIPT_VARIABLE_TYPE	eType; // Originally bool bIsInteger
};

ASSERT_SIZE(SCRIPT_LOCAL, 0x18);

class ACTION_OBJECT {
public:
	TESForm*	pObject;
	uint32_t		uiFlags;
};

ASSERT_SIZE(ACTION_OBJECT, 0x8);

class SCRIPT_EFFECT_DATA {
public:
	bool	bScriptEffectStart;
	bool	bScriptEffectFinish;
	float	fSecondsElapsed;
};

ASSERT_SIZE(SCRIPT_EFFECT_DATA, 0x8);

class SCRIPT_REFERENCED_OBJECT {
public:
	BSString	strName;
	TESForm*	pForm;
	uint32_t	uiVarID;
};

ASSERT_SIZE(SCRIPT_REFERENCED_OBJECT, 0x10);

class ScriptVariable {
public:
	SCRIPT_LOCAL	kData;
	BSString		strName;
};

ASSERT_SIZE(ScriptVariable, 0x20);

class SCRIPT_LINE {
public:
	uint32_t	uiLineNumber;
	char		cLine[512];
	uint32_t	uiSize;
	uint32_t	uiOffset;
	char		cOutput[512];
	uint32_t	uiOutputSize;
	uint32_t	uiOPCode;
	uint32_t	uiRefObjectIndex;
	uint32_t	eScriptError;
};

ASSERT_SIZE(SCRIPT_LINE, 0x41C);

class SCRIPT_PARAMETER {
public:
	const char* pParamName;
	uint32_t	eParamType;
	bool		bIsOptional;
};

ASSERT_SIZE(SCRIPT_PARAMETER, 0xC);

typedef bool (*Cmd_Execute)(SCRIPT_PARAMETER* apParameters, char* apCompiledParams, TESObjectREFR* apRef, TESObjectREFR* apContainer, Script* apScript, ScriptLocals* apScriptLocals, double* adResult, uint32_t* auiOffset);
typedef bool (*Cmd_Parse)(uint32_t auiParamCount, SCRIPT_PARAMETER* apParameters, SCRIPT_LINE* apScriptLine, ScriptCompileData* apCompileData);
typedef bool (*Cmd_Cond)(TESObjectREFR* apRef, void* apParam1, void* apParam2, double* adResult);

class SCRIPT_FUNCTION {
public:
	const char*			pFunctionName;
	const char*			pShortName;
	uint32_t			eOutput;
	const char*			pHelpString;
	bool				bReferenceFunction;
	uint16_t			usParamCount;
	SCRIPT_PARAMETER*	pParameters;
	Cmd_Execute			pExecuteFunction;
	Cmd_Parse			pCompileFunction;
	Cmd_Cond			pConditionFunction;
	uint32_t			uiFlags;
};

ASSERT_SIZE(SCRIPT_FUNCTION, 0x28);