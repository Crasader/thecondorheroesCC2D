#ifndef __ADMOB_HELPER_H__
#define __ADMOB_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "pluginadmob/PluginAdMob.h"
#endif



class AdmobHelper{
public:
	//static AdmobHelper *INSTANCE;
	AdmobHelper(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::init();
		sdkbox::PluginAdMob::cache("top_home");
		sdkbox::PluginAdMob::cache("bottom_home");
		sdkbox::PluginAdMob::cache("gameover");
#endif
	}
	void showTopBanner(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("top_home");
#endif
	}
	void showBottomBanner() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("bottom_home");
#endif
	}
	void hideTopBanner(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::hide("top_home");
#endif
	}
	void hideBottomBanner() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::hide("bottom_home");
#endif
	}
	void showFullAd(){
#ifdef SDKBOX_ENABLED
		sdkbox::PluginAdMob::show("gameover");
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

