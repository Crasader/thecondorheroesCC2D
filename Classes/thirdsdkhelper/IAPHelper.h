#ifndef __IAP_HELPER_H__
#define __IAP_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginIAP/PluginIAP.h"
#endif
#include <string>
using namespace std;
//
////#ifdef SDKBOX_ENABLED
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
#ifdef SDKBOX_ENABLED
		sdkbox::IAP::init();
#endif
	}
	/*bool showBanner() {
#ifdef SDKBOX_ENABLED
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
#ifdef SDKBOX_ENABLED
		sdkbox::IAP::purchase(name);
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

