#include "LoadingLayer.h"
#include "manager/RefManager.h"
#include "manager/JSonHeroManager.h"

Scene * LoadingLayer::createScene(int stage, int map, int charId)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object

	auto loadingLayer = LoadingLayer::create(stage, map, charId);
	loadingLayer->setName("loading");

	// add layer as a child to scene
	scene->addChild(loadingLayer);

	// return the scene
	return scene;
}

bool LoadingLayer::init(int stage, int map, int charId)
{

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	AdmobHelper::getInstance()->showAd("top_banner");
	//AdmobHelper::getInstance()->showRewardVideoToRevive();
	this->stage = stage;
	this->map = map;
	this->charId = charId;

	addStuff();

	return true;
}

LoadingLayer * LoadingLayer::create(int stage, int map, int charId)
{
	LoadingLayer *pRet = new(std::nothrow) LoadingLayer();
	if (pRet && pRet->init(stage, map, charId))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


void LoadingLayer::addStuff()
{
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto leftDoor = Sprite::create("UI/Loading/door.png");
	leftDoor->setScaleX(SCREEN_SIZE.width / 2 / leftDoor->getContentSize().width);
	leftDoor->setScaleY(SCREEN_SIZE.height / leftDoor->getContentSize().height);
	leftDoor->setPosition(origin.x + SCREEN_SIZE.width * 0.25f, origin.y + SCREEN_SIZE.height / 2);
	addChild(leftDoor);

	auto rightDoor = Sprite::create("UI/Loading/door.png");
	rightDoor->setFlippedX(true);
	rightDoor->setScaleX(SCREEN_SIZE.width / 2 / rightDoor->getContentSize().width);
	rightDoor->setScaleY(SCREEN_SIZE.height / rightDoor->getContentSize().height);
	rightDoor->setPosition(origin.x + SCREEN_SIZE.width * 0.75f, origin.y + SCREEN_SIZE.height / 2);
	addChild(rightDoor);


	auto avatarHero = Sprite::create(JSHERO->getAvatarLoadingPath());
	avatarHero->setAnchorPoint(Vec2(1, 0));
	avatarHero->setScale(SCREEN_SIZE.height * 0.98f / avatarHero->getContentSize().height);
	avatarHero->setPosition(origin.x + SCREEN_SIZE.width, 0);

	addChild(avatarHero);

	auto boardTime = Sprite::create("UI/Loading/board_time.png");
	boardTime->setAnchorPoint(Vec2::ZERO);
	boardTime->setScale(SCREEN_SIZE.width * 0.55f / boardTime->getContentSize().width);
	boardTime->setPosition(origin.x + SCREEN_SIZE.width * 0.04f, origin.y + SCREEN_SIZE.height * 0.14f);
	addChild(boardTime);


	auto lbLoading = Sprite::create("UI/Loading/text_loading.png");
	lbLoading->setAnchorPoint(Vec2::ZERO);
	lbLoading->setPosition(leftDoor->getContentSize().width * 0.1f, leftDoor->getContentSize().height * 0.25f);
	leftDoor->addChild(lbLoading);

	loading = LoadingBar::create("UI/Loading/time.png");
	loading->setAnchorPoint(Vec2::ZERO);
	loading->setScaleX(boardTime->getBoundingBox().size.width * 0.96f / loading->getContentSize().width);
	loading->setScaleY(boardTime->getBoundingBox().size.height * 0.6f / loading->getContentSize().height);
	loading->setPosition(Vec2(boardTime->getPositionX() + boardTime->getBoundingBox().size.width * 0.02f,
		boardTime->getPositionY() + boardTime->getBoundingBox().size.height * 0.2f));
	addChild(loading);

	auto lbGuide = Label::createWithTTF("Guide here", "fontsDPM/UTM_BRUSHSCI.ttf", 32);
	lbGuide->setAnchorPoint(Vec2::ZERO);
	lbGuide->setScale(boardTime->getBoundingBox().size.height * 0.8f / lbGuide->getContentSize().height);
	lbGuide->setPosition(boardTime->getPositionX(), boardTime->getPositionY() - boardTime->getBoundingBox().size.height * 1.3f);
	addChild(lbGuide);

	int index_tip = rand() % 10 + 1;
	lbGuide->setString(JSHERO->getTipAtX(index_tip));
	
	doLoading();

}

void LoadingLayer::doLoading()
{
	this->schedule([&](float dt) {
		++percent;
		loading->setPercent(percent);

		timer += 5;

		if (timer >= 200) {
			//start = chrono::system_clock::now();
			timer = 0;
			doProcess();
			unschedule("Key_loading");
		}

	}, 0.01f, "Key_loading");
}

//void LoadingLayer::doOpen()
//{
//	/*auto actionLeftOpen = MoveBy::create(0.5f, Vec2(-leftDoor->getBoundingBox().size.width, 0));
//	leftDoor->runAction(actionLeftOpen);
//	auto actionRightOpen = MoveBy::create(0.5f, Vec2(rightDoor->getBoundingBox().size.width, 0));
//	rightDoor->runAction(actionRightOpen);
//
//	auto removeAndPlay = CallFunc::create([&]() {
//		
//	});
//
//
//	auto action = Sequence::createWithTwoActions(DelayTime::create(0.5f), removeAndPlay);
//	this->runAction(action);*/
//}

void LoadingLayer::doProcess()
{
	mainScene = GameScene::create(stage, map, charId);
	mainScene->setName("gameLayer");
	mainScene->retain();
	hud = Hud::create();
	hud->retain();

	mainScene->setHud(hud);
	
	if (charId != REF->getLastPickHero()) {	// try
		hud->tryHud();
	}
	

	//end = chrono::system_clock::now();
	
	//chrono::duration<double> elapsed_seconds = end - start;
	//log("%f", elapsed_seconds.count());

	this->schedule([&](float dt) {
		++percent;
		loading->setPercent(percent);

		if (percent >= 100.0f) {
			percent = 0.0f;
			loading->setVisible(false);

			unschedule("key");

			AdmobHelper::getInstance()->hide("top_banner");
			Director::getInstance()->replaceScene(TransitionFade::create(0.43f, GameScene::createScene(mainScene, hud)));
		}

	}, 1.33f / (100.0f - percent), "key");
}