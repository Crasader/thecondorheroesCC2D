#ifndef __IAP_HELPER_H__
#define __IAP_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "PluginIAP/PluginIAP.h"
#endif
#include <string>
using namespace std;
//
////#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//class IAPHelperListener: public sdkbox::VungleListener{
//
//};
//#else
//class IAPHelperListener {
//	
//};
//#endif

class IAPHelper {
public:
	//static IAPHelper *INSTANCE;
	IAPHelper() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::IAP::init();
#endif
	}
	/*bool showBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (isAvailable("home")) {
			sdkbox::PluginIAP::show("home");
			return true;
		}
		sdkbox::PluginIAP::cache("home");
		return false;
#endif
		return false;
	}*/
	void purchase(std::string name) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::IAP::purchase(name);
#endif
	}

	void setIsDebugMode(bool mode) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::IAP::setDebug(mode);
#endif
	}


	static IAPHelper* getInstance() {
		static  IAPHelper *iapInstance;
		if (iapInstance == nullptr) {
			iapInstance = new IAPHelper();
		}
		return iapInstance;
	}
};

#endif

