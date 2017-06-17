#ifndef __IAP_HELPER_H__
#define __IAP_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginIAP/PluginIAP.h"
#endif
#include <string>
using namespace std;
class IAPHelper {
public:
	//static IAPHelper *INSTANCE;
	IAPHelper() {
#ifdef SDKBOX_ENABLED
		//sdkbox::IAP::init();
#endif
	}
	void purchase(std::string name) {
#ifdef SDKBOX_ENABLED
		sdkbox::IAP::purchase(name);
#endif
	}

	void refresh() {
#ifdef SDKBOX_ENABLED
		sdkbox::IAP::refresh();
		//CCLOG("refresh");
#endif
	}

	void restore() {
#ifdef SDKBOX_ENABLED
		sdkbox::IAP::restore();
		//CCLOG("restore");
#endif
	}

	void setIsDebugMode(bool mode) {
#ifdef SDKBOX_ENABLED
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

