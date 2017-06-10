#ifndef __ADMOB_HELPER_H__
#define __ADMOB_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "pluginadmob/PluginAdMob.h"
#endif
#include <string>
using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginAdMob::init();
		sdkbox::PluginAdMob::cache("home");
		sdkbox::PluginAdMob::cache("gameover");
		sdkbox::PluginAdMob::cache("rewarded");
#endif
	}
	bool isAvailable(std::string adname) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		return sdkbox::PluginAdMob::isAvailable(adname);
#endif
//		return false;
	}
	/*bool showBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginAdMob::show("home");
#endif
	}

	void hideBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginAdMob::hide("home");
#endif
	}

	/*bool showFullAd() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		
		sdkbox::PluginAdMob::show("gameover");
		sdkbox::PluginAdMob::cache("gameover");
#endif
	}

	void showRewardVideoToRevive() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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

