#pragma once

#include "RTM-Utils.hpp"

namespace RealTimeMenus {

	namespace Rendering {

		extern bool						bCanDoAsyncUI;
		extern bool						bUIUpdated;
		extern Utils::ImageSpaceStage	eImageSpaceStage;
	
		void InitHooks();
		void InitDeferredHooks();
	
	}

}