#ifndef __ADMOB_HELPER_H__
#define __ADMOB_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "pluginadmob/PluginAdMob.h"
#endif



class AdmobHelper{
public:
	static AdmobHelper *INSTANCE;
	AdmobHelper(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::init();
		sdkbox::PluginAdMob::cache("home");
		sdkbox::PluginAdMob::cache("gameover");
#endif
	}
	void showBanner(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("home");
#endif
	}
	void hideBanner(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::hide("home");
#endif
	}
	void showFullAd(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("gameover");
#endif
	}

	static AdmobHelper* getInstance() {
		if (!INSTANCE) {
			INSTANCE = new AdmobHelper();
		}
		return INSTANCE;
	}
};

#endif

