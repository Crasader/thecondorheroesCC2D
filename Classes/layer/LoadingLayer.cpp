#include "layer/LoadingLayer.h"
#include "layer/GameScene.h"

bool LoadingLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	addStuff();

	return true;
}


void LoadingLayer::addStuff()
{

	auto origin = Director::getInstance()->getVisibleOrigin();

	leftDoor = Sprite::create("UI/Loading/door.png");
	leftDoor->setScaleX(SCREEN_SIZE.width / 2 / leftDoor->getContentSize().width);
	leftDoor->setScaleY(SCREEN_SIZE.height / leftDoor->getContentSize().height);
	leftDoor->setPosition(origin.x + SCREEN_SIZE.width * 0.25f, origin.y + SCREEN_SIZE.height / 2);
	addChild(leftDoor);

	rightDoor = Sprite::create("UI/Loading/door.png");
	rightDoor->setFlippedX(true);
	rightDoor->setScaleX(SCREEN_SIZE.width / 2 / rightDoor->getContentSize().width);
	rightDoor->setScaleY(SCREEN_SIZE.height / rightDoor->getContentSize().height);
	rightDoor->setPosition(origin.x + SCREEN_SIZE.width * 0.75f, origin.y + SCREEN_SIZE.height / 2);
	addChild(rightDoor);


	boardTime = Sprite::create("UI/Loading/board_time.png");
	boardTime->setAnchorPoint(Vec2::ZERO);
	boardTime->setScale(SCREEN_SIZE.width * 0.55f / boardTime->getContentSize().width);
	boardTime->setPosition(origin.x + SCREEN_SIZE.width * 0.04f, origin.y + SCREEN_SIZE.height * 0.14f);
	addChild(boardTime);



	loading = LoadingBar::create("UI/Loading/time.png");
	loading->setAnchorPoint(Vec2::ZERO);
	loading->setScaleX(boardTime->getBoundingBox().size.width * 0.96f / loading->getContentSize().width);
	loading->setScaleY(boardTime->getBoundingBox().size.height * 0.6f / loading->getContentSize().height);
	loading->setPosition(Vec2(boardTime->getPositionX() + boardTime->getBoundingBox().size.width * 0.02f, 
								boardTime->getPositionY() + boardTime->getBoundingBox().size.height * 0.2f));
	addChild(loading);


	avatarHero = Sprite::create(JSHERO->getAvatarLoadingPath());
	avatarHero->setAnchorPoint(Vec2(1, 0));
	avatarHero->setPosition(rightDoor->getContentSize().width, 0);

	rightDoor->addChild(avatarHero);

	lbGuide = Label::create("Guide here", "fonts/Marker Felt.ttf", 32);
	lbGuide->setAnchorPoint(Vec2::ZERO);
	lbGuide->setScale(boardTime->getBoundingBox().size.height / lbGuide->getContentSize().height);
	lbGuide->setPosition(boardTime->getPositionX(), boardTime->getPositionY() - boardTime->getBoundingBox().size.height * 1.3f);
	addChild(lbGuide);


	doLoading();

}

void LoadingLayer::doLoading()
{
	this->schedule([&](float dt) {
		++percent;
		loading->setPercent(percent);

		if (percent > 100.0f) {
			boardTime->setVisible(false);
			loading->setVisible(false);
			lbGuide->setVisible(false);

			doOpen();
			unschedule("Key_loading");
		}

	}, 0.025f, "Key_loading");
}

void LoadingLayer::doOpen()
{
	auto actionLeftOpen = MoveBy::create(0.5f, Vec2(-leftDoor->getBoundingBox().size.width, 0));
	leftDoor->runAction(actionLeftOpen);
	auto actionRightOpen = MoveBy::create(0.5f, Vec2(rightDoor->getBoundingBox().size.width, 0));
	rightDoor->runAction(actionRightOpen);

	auto removeAndPlay = CallFunc::create([&]() {
		
		auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
		this->removeFromParentAndCleanup(true);
		gameLayer->onBegin();
		
	});


	auto action = Sequence::create(DelayTime::create(0.5f), removeAndPlay, nullptr);
	this->runAction(action);
}

void LoadingLayer::addGuide(string guideWhat)
{
	lbGuide->setString(guideWhat);
}

