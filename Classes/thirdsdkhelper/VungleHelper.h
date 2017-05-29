#ifndef __VUNGLE_HELPER_H__
#define __VUNGLE_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginVungle/PluginVungle.h"
#endif
#include <string>
using namespace std;
//
////#ifdef SDKBOX_ENABLED
//class VungleHelperListener: public sdkbox::VungleListener{
//
//};
//#else
//class VungleHelperListener {
//	
//};
//#endif

class VungleHelper {
public:
	//static VungleHelper *INSTANCE;
	VungleHelper() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginVungle::init();
#endif
	}
	/*bool showBanner() {
#ifdef SDKBOX_ENABLED
		if (isAvailable("home")) {
			sdkbox::PluginVungle::show("home");
			return true;
		}
		sdkbox::PluginVungle::cache("home");
		return false;
#endif
		return false;
	}*/
	void showVideo() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginVungle::show("video");
#endif
	}

	void showReward() {
#ifdef SDKBOX_ENABLED
		
		sdkbox::PluginVungle::show("reward");
#endif
	}

	static VungleHelper* getInstance() {
		static  VungleHelper *vungleInstance;
		if (vungleInstance == nullptr) {
			vungleInstance = new VungleHelper();
		}
		return vungleInstance;
	}
};

#endif

