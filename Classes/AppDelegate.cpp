#include "AppDelegate.h"
#include "layer/IntroScene.h"
#include "layer/SplashScene.h"
#include "AudioEngine.h"
#include "thirdsdkhelper/AdmobHelper.h"
#include "thirdsdkhelper/GoogleAnalysticHelper.h"
#include "thirdsdkhelper/VungleHelper.h"
#include "thirdsdkhelper\FacebookHelper.h"
#include "thirdsdkhelper\IAPHelper.h"
#include "thirdsdkhelper\SdkboxPlay.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(711, 400);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("thecondorheroesCC2D", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("thecondorheroesCC2D");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();


	// cache frame
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/bg.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/skill.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/Select_Stage/boss_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/QuachTinh/fire_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/QuachTinh/chidori_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/QuachTinh/rock_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/Select_Stage/select_stage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/UI_info_ingame/info_hud.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/Btn_attack/button_attack.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/Btn_skill/button_skill.plist");

    // create a scene. it's an autorelease object

	Scene* scene;

	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		scene = SceneIntro::createScene();
	else
		scene = SplashScene::createScene();

	AdmobHelper::getInstance();
	GAHelper::getInstance();
	VungleHelper::getInstance();
	FacebookHelper::getInstance();
	IAPHelper::getInstance();
	SPHelper::getInstance();


    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	experimental::AudioEngine::pauseAll();
	GAHelper::getInstance()->sendDataNow();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	experimental::AudioEngine::resumeAll();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
