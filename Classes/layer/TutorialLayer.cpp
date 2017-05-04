#include "layer/TutorialLayer.h"
#include "layer/GameScene.h"
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

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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
		log("Done Jump Tutorial");
		parentLayer->jump();
	}

	parentLayer->resumeAfterTut(1);

	return false;
}

bool TutorialJump::init(string image_path)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TutorialJump::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

TutorialJump * TutorialJump::create(string image_path)
{
	TutorialJump* tut = new TutorialJump();

	if (tut && tut->init(image_path))
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

bool TutorialIntroBird::init(string image_path)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TutorialIntroBird::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

TutorialIntroBird * TutorialIntroBird::create(string image_path)
{
	TutorialIntroBird* tut = new TutorialIntroBird();

	if (tut && tut->init(image_path))
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
