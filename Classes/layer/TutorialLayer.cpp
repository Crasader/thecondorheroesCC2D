#include "TutorialLayer.h"
#include "GameScene.h"
#include "manager/RefManager.h"




bool TutorialLayer::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	parentLayer->resumeAfterTut(2);
	return false;
}

bool TutorialLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->schedule([&](float dt) {
		counter++;
		if (counter >= 5) {
			listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
			unschedule("tut_key");
		}
	}, 0.1f, "tut_key");

	

	return true;
}

TutorialLayer* TutorialLayer::create()
{
	TutorialLayer* tut = new TutorialLayer();

	if (tut && tut->init())
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

bool TutorialJump::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");

	auto location = touch->getLocation();
	if (location.x < SCREEN_SIZE.width / 2) {
		//log("Done Jump Tutorial");
		parentLayer->jump();
	}

	parentLayer->resumeAfterTut(1);

	return false;
}

bool TutorialJump::init(int type)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto origin = Director::getInstance()->getVisibleOrigin();
	
	if (type == 1) {
		auto guideImage = Sprite::create("UI/UI_tutorial/imgtut1.png");
		guideImage->setScale(SCREEN_SIZE.height * 0.75f / guideImage->getContentSize().height);
		guideImage->setPosition(origin + SCREEN_SIZE / 2);
		addChild(guideImage);
	}
	else {
		auto lbText = Label::createWithTTF("Don't let hero fall down into abyss", "fonts/Marker Felt.ttf", 32);
		lbText->setScale(SCREEN_SIZE.height * 0.06f / lbText->getContentSize().height);

		lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
		addChild(lbText);
	}

	this->schedule([&](float dt) {
		counter++;
		if (counter >= 5) {
			listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(TutorialJump::onTouchBegan, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
			unschedule("tut_key");
		}
	}, 0.1f, "tut_key");

	return true;
}

TutorialJump * TutorialJump::create(int type)
{
	TutorialJump* tut = new TutorialJump();

	if (tut && tut->init(type))
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

bool TutorialAttack::init()
{
	TutorialLayer::init();

	auto origin = Director::getInstance()->getVisibleOrigin();
	/*auto lbText = Label::createWithTTF("Press Attack Button to hit Enemy nearby", "fonts/Marker Felt.ttf", 32);
	lbText->setScale(SCREEN_SIZE.height * 0.06f / lbText->getContentSize().height);
	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
	addChild(lbText);*/

	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut2.png");
	guideImage->setScale(SCREEN_SIZE.height * 0.75f / guideImage->getContentSize().height);
	guideImage->setPosition(origin + SCREEN_SIZE / 2);
	addChild(guideImage);

	return true;
}

TutorialAttack * TutorialAttack::create()
{
	TutorialAttack* tut = new TutorialAttack();

	if (tut && tut->init())
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

bool TutorialSkill::init()
{
	TutorialLayer::init();
	auto origin = Director::getInstance()->getVisibleOrigin();

	/*auto lbText = Label::createWithTTF("Use Skill Button to active special hero's skill", "fonts/Marker Felt.ttf", 32);
	lbText->setScale(SCREEN_SIZE.height * 0.06f / lbText->getContentSize().height);
	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
	addChild(lbText);*/

	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut3.png");
	guideImage->setScale(SCREEN_SIZE.height * 0.75f / guideImage->getContentSize().height);
	guideImage->setPosition(origin + SCREEN_SIZE / 2);
	addChild(guideImage);

	return true;
}

TutorialSkill * TutorialSkill::create()
{
	TutorialSkill* tut = new TutorialSkill();

	if (tut && tut->init())
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

bool TutorialIntroBird::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	
	auto origin = Director::getInstance()->getVisibleOrigin();

	/*auto lbText = Label::createWithTTF("Call your Cordon and you will fly with him.\nYou will be protected", "fonts/Marker Felt.ttf", 32);
	lbText->setAlignment(TextHAlignment::CENTER);
	lbText->setScale(SCREEN_SIZE.height * 0.12f / lbText->getContentSize().height);
	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
	addChild(lbText);*/

	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut4.png");
	guideImage->setScale(SCREEN_SIZE.height * 0.75f / guideImage->getContentSize().height);
	guideImage->setPosition(origin + SCREEN_SIZE / 2);
	addChild(guideImage);

	this->schedule([&](float dt) {
		counter++;
		if (counter >= 5) {
			listener = EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(TutorialIntroBird::onTouchBegan, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
			unschedule("tut_key");
		}
	}, 0.1f, "tut_key");
	return true;
}

TutorialIntroBird * TutorialIntroBird::create()
{
	TutorialIntroBird* tut = new TutorialIntroBird();

	if (tut && tut->init())
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

bool TutorialIntroBird::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	parentLayer->resumeAfterTut(4);
	return false;
}
