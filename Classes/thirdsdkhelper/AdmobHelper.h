#ifndef __ADMOB_HELPER_H__
#define __ADMOB_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif // SDKBOX_ENABLED


#include <string>
using namespace std;

class AdmobHelper {
public:
	AdmobHelper() {
#ifdef SDKBOX_ENABLED
		//sdkbox::PluginAdMob::init();
#endif
	}
	bool isAvailable(std::string adname) {
#ifdef SDKBOX_ENABLED
		return sdkbox::PluginAdMob::isAvailable(adname);
#endif
		return false;
	}

	void showAd(std::string name) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show(name);
#endif
	}

	void cacheAd(std::string name) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::cache(name);
#endif
	}


	void hide(std::string name) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::hide(name);
#endif
	}

	static AdmobHelper* getInstance() {
		static  AdmobHelper *admobInstance;
		if (admobInstance == nullptr) {
			admobInstance = new AdmobHelper();
		}
		return admobInstance;
	}
};

#endif

