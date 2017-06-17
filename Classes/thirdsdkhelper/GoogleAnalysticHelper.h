#ifndef __GA_HELPER_H__
#define __GA_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"
#endif
#include <string>
using namespace std;


class GAHelper {
public:
	//static GAHelper *INSTANCE;
	GAHelper() {
	}
	
	void stopSession() {
#ifdef SDKBOX_ENABLED
		 sdkbox::PluginGoogleAnalytics::stopSession();
#endif
	}

	void startSession() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginGoogleAnalytics::startSession();
#endif
	}

	void logEvent(string eventcategory, string eventAction, string eventContent, int eventValue) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginGoogleAnalytics::logEvent(eventcategory, eventAction, eventContent, eventValue);
#endif
	}

	void logScreen(string screenName) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginGoogleAnalytics::logScreen(screenName);
#endif
	}

	void sendDataNow() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginGoogleAnalytics::dispatchHits();
#endif
	}

	

	static GAHelper* getInstance() {
		static  GAHelper *gaInstance;
		if (gaInstance == nullptr) {
			gaInstance = new GAHelper();
		}
		return gaInstance;
	}
};

#endif

