#ifndef __GA_HELPER_H__
#define __GA_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#endif
#include <string>
using namespace std;


class GAHelper {
public:
	//static GAHelper *INSTANCE;
	GAHelper() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginGoogleAnalytics::init();
		sdkbox::PluginGoogleAnalytics::startSession();
#endif
	}

	void stopSession(std::string adname) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		 sdkbox::PluginGoogleAnalytics::stopSession();
#endif
	}

	void startSession(std::string adname) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginGoogleAnalytics::startSession();
#endif
	}

	void logEvent(string eventcategory, string eventAction, string eventContent, int eventValue) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginGoogleAnalytics::logEvent(eventcategory, eventAction, eventContent, eventValue);
#endif
	}

	void logScreen(string screenName) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginGoogleAnalytics::logScreen(screenName);
#endif
	}

	void sendDataNow() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginGoogleAnalytics::dispatchHits();
#endif
	}

	/*bool showFullAd() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (isAvailable("gameover")) {
			sdkbox::PluginGoogleAnalytics::show("gameover");
			return true;
		}
		sdkbox::PluginGoogleAnalytics::cache("gameover");
		return false;
#endif
		return false;
	}*/
	

	static GAHelper* getInstance() {
		static  GAHelper *gaInstance;
		if (gaInstance == nullptr) {
			gaInstance = new GAHelper();
		}
		return gaInstance;
	}
};

#endif

