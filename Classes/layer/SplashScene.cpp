#include "SplashScene.h"
#include "IntroScene.h"


Scene* SplashScene::createScene()
{

    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add Splash sprite
	auto splashSprite = Sprite::create("splash.png");
	splashSprite->setScale(visibleSize.height / splashSprite->getContentSize().height);
	splashSprite->setPosition(origin + visibleSize / 2);
	addChild(splashSprite);


	auto nextScene = CallFunc::create([]() {
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, SceneIntro::createScene()));
	});

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), nextScene));

    return true;
}


