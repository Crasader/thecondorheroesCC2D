#ifndef __SDKBOXPLAY_HELPER_H__
#define __SDKBOXPLAY_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#endif
#include <string>
using namespace std;


class SPHelper {
public:
	//static GAHelper *INSTANCE;
	SPHelper() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginSdkboxPlay::init();
#endif
	}

	
	bool isSigned() {
#ifdef SDKBOX_ENABLED
		return sdkbox::PluginSdkboxPlay::isSignedIn();
#endif
		return false;
	}
	void signIn() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginSdkboxPlay::signin();
#endif

	}

	void submitScore(string name, int score) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginSdkboxPlay::submitScore(name,  score);
#endif
	}

	void showBoard(string name) {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginSdkboxPlay::showLeaderboard(name);
#endif
	}
	

	static SPHelper* getInstance() {
		static  SPHelper *spInstance;
		if (spInstance == nullptr) {
			spInstance = new SPHelper();
		}
		return spInstance;
	}
};


#endif

