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

	//auto menuplay = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(MenuLayer::gotoPlay, this));
	//createPalybutton
	auto groupplaybutton = tmxMap->getObjectGroup("play");
	for (auto child : groupplaybutton->getObjects()) {
		auto mObject = child.asValueMap();
		auto name = mObject["name"].asInt();
		Point origin = Point(mObject["x"].asFloat() *scalex, mObject["y"].asFloat()* scaley);
		auto menuplay = Sprite::create("play.png");
		menuplay->setPosition(origin);
		menuplay->setTag(name);
		auto scalePlay = screenSize.width / 8 / menuplay->getContentSize().width;
		menuplay->setScale(scalePlay);
		menuplay->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.5f,scalePlay*1.1f), ScaleTo::create(0.5f,scalePlay))));
		listMap.push_back(menuplay);
		this->addChild(menuplay);
		auto label = Label::create(StringUtils::format("%d", name),"Arial",24);
		label->setPosition(menuplay->getContentSize() / 2);
		menuplay->addChild(label);
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MenuLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

bool MenuLayer::onTouchBegan(Touch * touch, Event * event)
{
	for (auto a : listMap) {
		if (a->getBoundingBox().containsPoint(touch->getLocation()))
		{
			gotoPlay(a->getTag());
		}
	}
	return false;
}

void MenuLayer::gotoPlay( int map)
{
	int haveboss;
	switch (map)
	{
	case 1: {
		haveboss = 0;
		break;
	}
	case 2: {
		haveboss = 0;
		break;
	}
	case 3: {
		haveboss = 1;
		break;
	}
	default:
		break;
	}
	Director::getInstance()->replaceScene(GameScene::createScene(map, haveboss));
	log("map%d", map);
}
