#include "TutorialLayer.h"
#include "GameScene.h"


void TutorialLayer::showJump()
{
	this->setVisible(true);
	this->schedule(schedule_selector(TutorialLayer::showTime), 0.5, 1, 0);
}

void TutorialLayer::showAttack()
{
	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut2.png");
	this->schedule(schedule_selector(TutorialLayer::showTime), 0.5, 1, 0);
}

void TutorialLayer::showSkills()
{
	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut3.png");
	this->schedule(schedule_selector(TutorialLayer::showTime), 0.5, 1, 0);
}

void TutorialLayer::showBird()
{
	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut4.png");
	this->schedule(schedule_selector(TutorialLayer::showTime), 0.5, 1, 0);
}

void TutorialLayer::showTime(float dt)
{
	this->unschedule(schedule_selector(TutorialLayer::showTime));
	accept = true;
}


bool TutorialLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	main = Sprite::create("UI/UI_tutorial/imgtut1.png");
	main->setScale(visibleSize.height * 0.75f / main->getContentSize().height);
	main->setPosition(origin + visibleSize / 2);
	addChild(main);

	return true;
}

TutorialLayer* TutorialLayer::create()
{
	TutorialLayer* tut = new(std::nothrow) TutorialLayer();

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

