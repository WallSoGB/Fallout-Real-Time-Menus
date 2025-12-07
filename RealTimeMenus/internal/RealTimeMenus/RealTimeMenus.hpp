#pragma once

class NVSEGlobalManager;

namespace RealTimeMenus {

	void ReadSettings();
	void InitHooks();
	void InitDeferredHooks();
	void RegisterMessageHandler(NVSEGlobalManager& arNVSEMgr);

}