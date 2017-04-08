#include "layer/MenuScene.h"
#include "layer/GameScene.h"

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
	
		skeleton = SkeletonAnimation::createWithFile("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas", screenSize.height / 5 / 340);
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
		auto label = Label::create(StringUtils::format("%d", name),"fonts/arial.ttf",24);
		label->setPosition(menuplay->getContentSize() / 2);
		menuplay->addChild(label);
	}

	//Thinhnv edited for select character
	auto groupCharacter = tmxMap->getObjectGroup("character");
	for (auto child : groupCharacter->getObjects()) {
		auto mObject = child.asValueMap();
		auto name = mObject["name"].asString();
		Point origin = Point(mObject["x"].asFloat() *scalex, mObject["y"].asFloat()* scaley);
		if (name.compare("dq") == 0) {
			lblDQ = Label::create("Duong Qua", "fonts/arial.ttf", 24);
			lblDQ->setPosition(origin);
			lblDQ->setColor(ccc3(255, 0, 0));
			this->addChild(lblDQ);
		}
		else if (name.compare("cc") == 0) {
			lblCC = Label::create("Co Co", "fonts/arial.ttf", 24);
			lblCC->setPosition(origin);
			lblCC->setColor(ccc3(255, 255, 255));
			this->addChild(lblCC);
		}
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

	//Thinhnv edited for select character
	if (lblCC->getBoundingBox().containsPoint(touch->getLocation())) {
		this->charId = 1;

		lblCC->setColor(ccc3(255, 0, 0));
		lblDQ->setColor(ccc3(255, 255, 255));

		auto screenSize = Director::getInstance()->getVisibleSize();
		auto skeletonPos = skeleton->getPosition();
		this->removeChild(skeleton);
		skeleton = SkeletonAnimation::createWithFile("Animation/CoLong/CoLong.json", "Animation/CoLong/CoLong.atlas", screenSize.height / 5 / 340);
		skeleton->setPosition(skeletonPos);
		skeleton->setVisible(true);
		this->addChild(skeleton);
		skeleton->setAnimation(0, "idle", true);
	}
	if (lblDQ->getBoundingBox().containsPoint(touch->getLocation())) {
		this->charId = 0;

		lblDQ->setColor(ccc3(255, 0, 0));
		lblCC->setColor(ccc3(255, 255, 255));

		auto screenSize = Director::getInstance()->getVisibleSize();
		auto skeletonPos = skeleton->getPosition();
		this->removeChild(skeleton);
		skeleton = SkeletonAnimation::createWithFile("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas", screenSize.height / 5 / 340);
		skeleton->setPosition(skeletonPos);
		skeleton->setVisible(true);
		this->addChild(skeleton);
		skeleton->setAnimation(0, "idle", true);
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
	Director::getInstance()->replaceScene(GameScene::createScene(map, haveboss, charId));
	//log("map%d", map);
}
