#ifndef __VUNGLE_HELPER_H__
#define __VUNGLE_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "PluginVungle/PluginVungle.h"
#endif
#include <string>
using namespace std;
//
////#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginVungle::init();
#endif
	}
	/*bool showBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginVungle::show("video");
#endif
	}

	void showReward() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		
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

