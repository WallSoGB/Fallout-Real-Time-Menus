#include "nvse/PluginAPI.h"
#include "RealTimeMenus/RealTimeMenus.hpp"
#include "shared/NVSEManager/NVSEGlobalManager.hpp"

constexpr char const* PLUGIN_NAME = "Real Time Menus";

#include "shared/BSMemory/BSMemory.hpp"
BS_ALLOCATORS

IDebugLog	   gLog("logs\\RealTimeMenus.log");

EXTERN_DLL_EXPORT bool NVSEPlugin_Query(const NVSEInterface* apNVSE, PluginInfo* apInfo) {
	apInfo->infoVersion = PluginInfo::kInfoVersion;
	apInfo->name = PLUGIN_NAME;
	apInfo->version = 100;

	return !apNVSE->isEditor;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Load(NVSEInterface* apNVSE) {
	NVSEGlobalManager& rNVSEManager = NVSEGlobalManager::GetSingleton();

	rNVSEManager.Initialize(apNVSE);

	RealTimeMenus::ReadSettings();
	RealTimeMenus::InitHooks();
	RealTimeMenus::RegisterMessageHandler(rNVSEManager);

	return true;
}

BOOL WINAPI DllMain(
	HANDLE  hDllHandle,
	DWORD   dwReason,
	LPVOID  lpreserved
)
{
	return TRUE;
}