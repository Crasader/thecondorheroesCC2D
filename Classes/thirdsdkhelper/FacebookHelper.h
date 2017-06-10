#ifndef __FACEBOOK_HELPER_H__
#define __FACEBOOK_HELPER_H__
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "PluginFacebook/PluginFacebook.h"
#endif
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

class FacebookHelper {
private :
	std::string _captureFilename;
public:
	//static GAHelper *INSTANCE;
	FacebookHelper() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginFacebook::init();
#endif
	}

public:
	
	void login() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (!sdkbox::PluginFacebook::isLoggedIn())
			sdkbox::PluginFacebook::login();
#endif
	};
	void logout() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginFacebook::logout();
#endif
	};
	void isLoggedIn() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginFacebook::isLoggedIn();
#endif
};
	void requestPostPermission() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginFacebook::requestPublishPermissions({ sdkbox::FB_PERM_PUBLISH_POST });
#endif
	};
	void requestReadProfilePermisson() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		sdkbox::PluginFacebook::requestReadPermissions({ sdkbox::FB_PERM_READ_PUBLIC_PROFILE, sdkbox::FB_PERM_READ_USER_FRIENDS });
#endif
	};
	void dialogPhoto(string title)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

		if (!_captureFilename.empty() && FileUtils::getInstance()->isFileExist(_captureFilename))
		{
			//CCLOG("dialog photo: %s", _captureFilename.c_str());
			sdkbox::FBShareInfo info;
			info.type = sdkbox::FB_PHOTO;
			info.title = title;
			info.image = _captureFilename;
			sdkbox::PluginFacebook::dialog(info);
		}
		else
		{
			//CCLOG("##FB capture screen first");
		}
#endif
	}
	void captureScreen()
	{
		utils::captureScreen(CC_CALLBACK_2(FacebookHelper::afterCaptureScreen, this), "screen.png");
	}

	void afterCaptureScreen(bool yes, const std::string &outputFilename)
	{
		
		if (yes)
		{
			_captureFilename = outputFilename;
		}
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

