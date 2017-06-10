#ifndef __SDKBOXPLAY_HELPER_H__
#define __SDKBOXPLAY_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "PluginSdkboxPlay/PluginSdkboxPlay.h"
#endif
#include <string>
using namespace std;


class SPHelper {
public:
	//static GAHelper *INSTANCE;
	SPHelper() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginSdkboxPlay::init();
#endif
	}

	
	bool isSigned() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		return sdkbox::PluginSdkboxPlay::isSignedIn();
#endif
		return false;
	}
	void signIn() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginSdkboxPlay::signin();
#endif

	}

	void submitScore(string name, int score) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginSdkboxPlay::submitScore(name,  score);
#endif
	}

	void showBoard(string name) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
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

