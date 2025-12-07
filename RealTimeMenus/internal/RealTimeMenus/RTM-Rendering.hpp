#pragma once

namespace RealTimeMenus {

	namespace Rendering {

		extern bool bCanDoAsyncUI;
		extern bool bUIUpdated;
	
		void InitHooks();
		void InitDeferredHooks();
	
	}

}