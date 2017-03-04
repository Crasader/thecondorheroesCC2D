#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


Hud *hud;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	hud = Hud::create();

    // add layer as a child to scene
    scene->addChild(layer);
	scene->addChild(hud);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("map_1/bg.jpg");
	bg->setScaleX(visibleSize.width / bg->getContentSize().width);
	bg->setScaleY(visibleSize.height / bg->getContentSize().height);
	bg->setPosition(origin + visibleSize / 2);
	addChild(bg);

	this->scheduleUpdate();

    return true;
}


void HelloWorld::listener()
{
	if (hud->getBtnAttack()->getIsActive()) {
		log("Ahihi");
		hud->getBtnAttack()->setIsActive(false);
	}
}

void HelloWorld::update(float dt)
{
	listener();
}
