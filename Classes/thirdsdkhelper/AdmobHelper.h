#ifndef __ADMOB_HELPER_H__
#define __ADMOB_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "pluginadmob/PluginAdMob.h"
#endif
#include <string>
using namespace std;

#ifdef SDKBOX_ENABLED

class IMListener : public sdkbox::AdMobListener {
public:

	virtual void reward(const std::string &name, const std::string &currency, double amount) {
		
	}
};
#endif

class AdmobHelper {
public:
	//static AdmobHelper *INSTANCE;
	AdmobHelper() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::init();
		sdkbox::PluginAdMob::cache("home");
		sdkbox::PluginAdMob::cache("gameover");
		sdkbox::PluginAdMob::cache("rewarded");
#endif
	}
	bool isAvailable(std::string adname) {
#ifdef SDKBOX_ENABLED
		return sdkbox::PluginAdMob::isAvailable(adname);
#endif
		return false;
	}
	/*bool showBanner() {
#ifdef SDKBOX_ENABLED
		if (isAvailable("home")) {
			sdkbox::PluginAdMob::show("home");
			return true;
		}
		sdkbox::PluginAdMob::cache("home");
		return false;
#endif
		return false;
	}*/
	void showBanner() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("home");
#endif
	}

	void hideBanner() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::hide("home");
#endif
	}

	/*bool showFullAd() {
#ifdef SDKBOX_ENABLED
		if (isAvailable("gameover")) {
			sdkbox::PluginAdMob::show("gameover");
			return true;
		}
		sdkbox::PluginAdMob::cache("gameover");
		return false;
#endif
		return false;
	}*/
	void showFullAd() {
#ifdef SDKBOX_ENABLED
		
		sdkbox::PluginAdMob::show("gameover");
		sdkbox::PluginAdMob::cache("gameover");
#endif
	}

	void showRewardVideoToRevive() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("rewarded");
		sdkbox::PluginAdMob::cache("rewarded");
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

