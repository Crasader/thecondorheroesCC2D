#ifndef __FACEBOOK_HELPER_H__
#define __FACEBOOK_HELPER_H__
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

class FacebookHelper {
public:
	//static GAHelper *INSTANCE;
	FacebookHelper() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginFacebook::init();
#endif
	}

private:
	void onCaptured(bool succeed, const std::string& outputFile) {};
	std::string takeScreenShot() {
		utils::captureScreen(CC_CALLBACK_2(FacebookHelper::onCaptured, this), "scrcap.png");
		return FileUtils::getInstance()->fullPathForFilename("scrcap.png");
	};
public:
	
	void login() {
#ifdef SDKBOX_ENABLED
		if (!sdkbox::PluginFacebook::isLoggedIn())
			sdkbox::PluginFacebook::login();
#endif
	};
	void logout() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginFacebook::logout();
#endif
	};
	void isLoggedIn() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginFacebook::isLoggedIn();
#endif
};
	void requestPostPermission() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginFacebook::requestPublishPermissions({ sdkbox::FB_PERM_PUBLISH_POST });
#endif
	};
	void requestReadProfilePermisson() {
#ifdef SDKBOX_ENABLED
		sdkbox::PluginFacebook::requestReadPermissions({ sdkbox::FB_PERM_READ_PUBLIC_PROFILE, sdkbox::FB_PERM_READ_USER_FRIENDS });
#endif
	};
	void scrShotAndShare(std::string title) {
#ifdef SDKBOX_ENABLED
		sdkbox::FBShareInfo info;
		info.type = sdkbox::FB_PHOTO;
		info.title = title;
		info.image = takeScreenShot();
		sdkbox::PluginFacebook::share(info);
#endif
	};
	void scrShotAndDialog(std::string title) {
#ifdef SDKBOX_ENABLED
		sdkbox::FBShareInfo info;
		info.type = sdkbox::FB_PHOTO;
		info.title = title;
		info.image = takeScreenShot();
		sdkbox::PluginFacebook::dialog(info);
#endif
	}

	static FacebookHelper* getInstance() {
		static  FacebookHelper *faInstance;
		if (faInstance == nullptr) {
			faInstance = new FacebookHelper();
		}
		return faInstance;
	}
};

#endif

