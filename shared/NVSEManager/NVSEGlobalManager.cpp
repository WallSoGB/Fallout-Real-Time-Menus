#include "NVSEGlobalManager.hpp"
#include "Bethesda/Script.hpp"

bool					(*NVSEGlobalManager::ExtractArgsEx)(COMMAND_ARGS_EX, ...) = nullptr;
PluginFormExtraData*	(*NVSEGlobalManager::ExtraData::Get)(const TESForm*, const char*) = nullptr;
bool					(*NVSEGlobalManager::ExtraData::Add)(TESForm*, PluginFormExtraData*) = nullptr;
void					(*NVSEGlobalManager::ExtraData::RemoveByName)(TESForm*, const char*) = nullptr;
void					(*NVSEGlobalManager::ExtraData::RemoveByPtr)(TESForm*, PluginFormExtraData*) = nullptr;

NVSEGlobalManager::NVSEGlobalManager() {
}

NVSEGlobalManager::~NVSEGlobalManager() {
}

void NVSEGlobalManager::Initialize(NVSEInterface* apNVSEInterface) {
	pNVSEInterface		= apNVSEInterface;
	pScriptInterface	= static_cast<NVSEScriptInterface*>(apNVSEInterface->QueryInterface(kInterface_Script));
	pCmdInterface		= static_cast<NVSECommandTableInterface*>(apNVSEInterface->QueryInterface(kInterface_CommandTable));
	pArrayInterface		= static_cast<NVSEArrayVarInterface*>(apNVSEInterface->QueryInterface(kInterface_ArrayVar));
	pMsgInterface		= static_cast<NVSEMessagingInterface*>(apNVSEInterface->QueryInterface(kInterface_Messaging));
	pStrInterface		= static_cast<NVSEStringVarInterface*>(apNVSEInterface->QueryInterface(kInterface_StringVar));
	pEventInterface		= static_cast<NVSEEventManagerInterface*>(apNVSEInterface->QueryInterface(kInterface_EventManager));
	pDataInterface		= static_cast<NVSEDataInterface*>(apNVSEInterface->QueryInterface(kInterface_Data));
	uiPluginHandle		= apNVSEInterface->GetPluginHandle();
	ExtractArgsEx 		= GetScriptInterface()->ExtractArgsEx;

	ExtraData::Get			= reinterpret_cast<PluginFormExtraData * (*)(const TESForm*, const char*)>(pDataInterface->GetFunc(NVSEDataInterface::kNVSEData_FormExtraDataGet));
	ExtraData::Add			= reinterpret_cast<bool(*)(TESForm*, PluginFormExtraData*)>(pDataInterface->GetFunc(NVSEDataInterface::kNVSEData_FormExtraDataAdd));
	ExtraData::RemoveByName = reinterpret_cast<void (*)(TESForm*, const char*)>(pDataInterface->GetFunc(NVSEDataInterface::kNVSEData_FormExtraDataRemoveByName));
	ExtraData::RemoveByPtr	= reinterpret_cast<void (*)(TESForm*, PluginFormExtraData*)>(pDataInterface->GetFunc(NVSEDataInterface::kNVSEData_FormExtraDataRemoveByPtr));

	_MESSAGE("NVSEGlobalManager initialized!");
}

NVSEGlobalManager& NVSEGlobalManager::GetSingleton() {
	static NVSEGlobalManager kSingleton;
	return kSingleton;
}

void NVSEGlobalManager::ReplaceScriptFunctions(const char* apName, Cmd_Execute apCommand, Cmd_Cond apCondition) {
	SCRIPT_FUNCTION* pInfo = const_cast<SCRIPT_FUNCTION*>(pCmdInterface->GetByName(apName));
	if (pInfo) {
		pInfo->pExecuteFunction = apCommand;
		pInfo->pConditionFunction = apCondition;
		_MESSAGE("ReplaceScriptFunctions: Replaced script function for name %s", apName);
	}
	else {
		_MESSAGE("ReplaceScriptFunctions: No script function found for name %s", apName);
	}
}

void NVSEGlobalManager::ReplaceScriptFunctions(uint32_t auiOPCode, Cmd_Execute apCommand, Cmd_Cond apCondition) {
	SCRIPT_FUNCTION* pInfo = const_cast<SCRIPT_FUNCTION*>(pCmdInterface->GetByOpcode(auiOPCode));
	if (pInfo) {
		pInfo->pExecuteFunction = apCommand;
		pInfo->pConditionFunction = apCondition;
		_MESSAGE("ReplaceScriptFunctions: Replaced script function for opcode %u", auiOPCode);
	}
	else {
		_MESSAGE("ReplaceScriptFunctions: No script function found for opcode %u", auiOPCode);
	}
}

bool NVSEGlobalManager::RegisterPluginEventListener(const char* apSender, NVSEMessagingInterface::EventCallback apCallback) const {
	return GetMsgInterface()->RegisterListener(uiPluginHandle, apSender, apCallback);
}

bool NVSEGlobalManager::DispatchPluginEvent(uint32_t auiMessageType, void* apData, uint32_t auiDataLength, const char* apReceiver) const {
	return GetMsgInterface()->Dispatch(uiPluginHandle, auiMessageType, apData, auiDataLength, apReceiver);
}

bool NVSEGlobalManager::RegisterScriptEvent(const char* apName, uint8_t aucParams, NVSEEventManagerInterface::SCRIPT_PARAMETER_TYPE* apParamType, NVSEEventManagerInterface::EventFlags aeFlags) {
	return GetEventInterface()->RegisterEvent(apName, aucParams, apParamType, aeFlags);
}

bool NVSEGlobalManager::RegisterScriptEvent(const char* apName, std::initializer_list<NVSEEventManagerInterface::SCRIPT_PARAMETER_TYPE> akParams, NVSEEventManagerInterface::EventFlags aeFlags) {
	return GetEventInterface()->RegisterEvent(apName, static_cast<uint8_t>(akParams.size()), const_cast<NVSEEventManagerInterface::SCRIPT_PARAMETER_TYPE*>(akParams.begin()), aeFlags);
}

PluginFormExtraData* NVSEGlobalManager::GetExtraData(const TESForm* apForm, const char* apName) {
	ASSUME_ASSERT(ExtraData::Get != nullptr);
	return ExtraData::Get(apForm, apName);
}

bool NVSEGlobalManager::AddExtraData(TESForm* apForm, PluginFormExtraData* apExtraData) {
	ASSUME_ASSERT(ExtraData::Add != nullptr);
	return ExtraData::Add(apForm, apExtraData);
}

void NVSEGlobalManager::RemoveExtraData(TESForm* apForm, const char* apName) {
	ASSUME_ASSERT(ExtraData::RemoveByName != nullptr);
	return ExtraData::RemoveByName(apForm, apName);
}

void NVSEGlobalManager::RemoveExtraData(TESForm* apForm, PluginFormExtraData* apExtraData) {
	ASSUME_ASSERT(ExtraData::RemoveByPtr != nullptr);
	return ExtraData::RemoveByPtr(apForm, apExtraData);
}
