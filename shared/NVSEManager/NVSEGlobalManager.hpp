#pragma once

#include "nvse/PluginAPI.h"

class NVSEGlobalManager {
private:
	NVSEInterface*				pNVSEInterface		= nullptr;
	NVSEScriptInterface*		pScriptInterface	= nullptr;
	NVSEArrayVarInterface*		pArrayInterface		= nullptr;
	NVSEMessagingInterface*		pMsgInterface		= nullptr;
	NVSECommandTableInterface*	pCmdInterface		= nullptr;
	NVSEStringVarInterface*		pStrInterface		= nullptr;
	NVSEEventManagerInterface*	pEventInterface		= nullptr;
	NVSEDataInterface*			pDataInterface		= nullptr;
	uint32_t					uiPluginHandle		= 0;

	struct ExtraData {
		static PluginFormExtraData* (*Get)(const TESForm*, const char*);
		static bool					(*Add)(TESForm*, PluginFormExtraData*);
		static void					(*RemoveByName)(TESForm*, const char*);
		static void					(*RemoveByPtr)(TESForm*, PluginFormExtraData*);
	};
public:
	NVSEGlobalManager();
	~NVSEGlobalManager();

	static bool (*ExtractArgsEx)(COMMAND_ARGS_EX, ...);

	void Initialize(NVSEInterface* apNVSEInterface);

	bool IsInitialized() const{
		return pNVSEInterface != nullptr;
	}

	static NVSEGlobalManager& GetSingleton();

	NVSEInterface*				GetNVSEInterface() const	{ return pNVSEInterface;	}
	NVSEScriptInterface*		GetScriptInterface() const	{ return pScriptInterface;	}
	NVSEArrayVarInterface*		GetArrayInterface() const	{ return pArrayInterface;	}
	NVSEMessagingInterface*		GetMsgInterface() const		{ return pMsgInterface;		}
	NVSECommandTableInterface*	GetCmdInterface() const		{ return pCmdInterface;		}
	NVSEStringVarInterface*		GetStrInterface() const		{ return pStrInterface;		}
	NVSEEventManagerInterface*	GetEventInterface() const	{ return pEventInterface;	}
	NVSEDataInterface*			GetDataInterface() const	{ return pDataInterface; }
	uint32_t					GetPluginHandle() const		{ return uiPluginHandle;	}


	// Command Table

	void ReplaceScriptFunctions(const char* apName, Cmd_Execute apCommand, Cmd_Cond apCondition);
	void ReplaceCommandFunction(const char* apName, Cmd_Execute apFunc);
	void ReplaceConditionFunction(const char* apName, Cmd_Cond apFunc);

	void ReplaceScriptFunctions(uint32_t auiOPCode, Cmd_Execute apCommand, Cmd_Cond apCondition);
	void ReplaceCommandFunction(uint32_t auiOPCode, Cmd_Execute apFunc);
	void ReplaceConditionFunction(uint32_t auiOPCode, Cmd_Cond apFunc);
	

	// Messaging

	bool RegisterPluginEventListener(const char* apSender, NVSEMessagingInterface::EventCallback apCallback) const;
	bool DispatchPluginEvent(uint32_t auiMessageType, void* apData, uint32_t auiDataLength, const char* apReceiver) const;


	// Event Manager
	
	bool RegisterScriptEvent(const char* apName, uint8_t aucParams, NVSEEventManagerInterface::SCRIPT_PARAMETER_TYPE* apParamType, NVSEEventManagerInterface::EventFlags aeFlags);
	bool RegisterScriptEvent(const char* apName, std::initializer_list<NVSEEventManagerInterface::SCRIPT_PARAMETER_TYPE> akParams, NVSEEventManagerInterface::EventFlags aeFlags);

	template <typename... Args>
	bool DispatchScriptEvent(const char* apName, class TESObjectREFR* apRef, Args...args) const {
		return GetEventInterface()->DispatchEventThreadSafe(apName, nullptr, apRef, std::forward<Args>(args)...);
	}

	// Data

	static PluginFormExtraData* GetExtraData(const TESForm* apForm, const char* apName);
	
	template <typename T>
	static T* GetExtraData(const TESForm* apForm) {
		static_assert(std::is_base_of<PluginFormExtraData, T>::value, "T must be derived from PluginFormExtraData");
		return static_cast<T*>(GetExtraData(apForm, T::GetName()));
	}

	static bool AddExtraData(TESForm* apForm, PluginFormExtraData* apExtraData);

	static void RemoveExtraData(TESForm* apForm, const char* apName);

	static void RemoveExtraData(TESForm* apForm, PluginFormExtraData* apExtraData);
};