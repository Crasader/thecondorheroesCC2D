#include "MenuScene.h"
#include "GameScene.h"

cocos2d::Scene * MenuLayer::createScene()
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MenuLayer::init()
{
	auto screenSize = Director::getInstance()->getVisibleSize();

	auto tmxMap = TMXTiledMap::create("config_menu_scene.tmx");
	tmxMap->setPosition(0,0);
	auto scalex = screenSize.width / tmxMap->getContentSize().width;
	auto scaley = screenSize.height / tmxMap->getContentSize().height;
	tmxMap->setScaleX(scalex);
	tmxMap->setScaleY(scaley);
	this->addChild(tmxMap);

	// create hero
	auto grouphero = tmxMap->getObjectGroup("hero");
	for (auto child : grouphero->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scalex, mObject["y"].asFloat()* scaley);
	
		auto skeleton = SkeletonAnimation::createWithFile("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas", screenSize.height / 5 / 340);
		skeleton->setPosition(origin);
		skeleton->setVisible(true);
		this->addChild(skeleton);
		skeleton->setAnimation(0,"idle",true);
	}

	auto menuplay = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(MenuLayer::gotoPlay, this));
	//createPalybutton
	auto groupplaybutton = tmxMap->getObjectGroup("play");
	for (auto child : groupplaybutton->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scalex, mObject["y"].asFloat()* scaley);

		menuplay->setPosition(origin);
		auto scalePlay = screenSize.width / 8 / menuplay->getContentSize().width;
		menuplay->setScale(scalePlay);
		menuplay->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5f,scalePlay*1.1f), ScaleTo::create(0.5f,scalePlay))));
		
		auto menu = Menu::create(menuplay, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu);
	}

	return true;
}

void MenuLayer::gotoPlay(Ref * pSender)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}
