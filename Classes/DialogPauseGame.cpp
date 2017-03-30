#include "DialogPauseGame.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"


bool DialogPauseGame::init(int type)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	switch (type)
	{
	case -1: {
		initForPause();
		break;
	}
	case 0: {
		initForDie();
		break;
	}
	case 1: {
		initForWin();
		break;
	}

	default:
		break;
	}
	

	return true;
}

DialogPauseGame* DialogPauseGame::create(int type)
{
	DialogPauseGame* dialog = new DialogPauseGame();

	if (dialog && dialog->init(type))
	{
		dialog->autorelease();
		return dialog;
	}
	else
	{
		delete dialog;
		dialog = nullptr;
		return nullptr;
	}
}

void DialogPauseGame::initForPause()
{
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin + win_size / 2);
	addChild(background);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backToPlay, this));
	backBtn->setAnchorPoint(Vec2(0.5, 1));
	backBtn->setScale(background->getBoundingBox().size.width / 3 / backBtn->getContentSize().width);
	backBtn->setPosition(background->getPositionX() - background->getBoundingBox().size.width / 2,
		background->getPositionY() - background->getBoundingBox().size.height / 2);

	auto exitBtn = MenuItemImage::create("UI/UI_Endgame/btn_restart.png", "UI/UI_Endgame/btn_restart.png", CC_CALLBACK_1(DialogPauseGame::exitToStage, this));
	exitBtn->setAnchorPoint(Vec2(0.5, 1));
	exitBtn->setScale(background->getBoundingBox().size.width / 3 / exitBtn->getContentSize().width);
	exitBtn->setPosition(background->getPositionX() + background->getBoundingBox().size.width / 2,
		background->getPositionY() - background->getBoundingBox().size.height / 2);

	auto menu = Menu::create(backBtn, exitBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);
}

void DialogPauseGame::initForDie()
{
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();

	background = Sprite::create("UI/UI_Endgame/paper_revive.png");
	background->setScale(win_size.width / 1.8f / background->getContentSize().width);
	background->setPosition(origin.x + win_size.width / 2, origin.y + win_size.height*2/3);
	addChild(background);

	auto reviveBtn = MenuItemImage::create("UI/UI_Endgame/btn_revive.png", "UI/UI_Endgame/btn_revive.png", CC_CALLBACK_1(DialogPauseGame::replay, this));
	reviveBtn->setAnchorPoint(Vec2(0,0.5f ));
	//reviveBtn->setScale(background->getBoundingBox().size.width / 3 / reviveBtn->getContentSize().width);
	reviveBtn->setPosition(0,0);

	auto videoBtn = MenuItemImage::create("UI/UI_Endgame/btn_video.png", "UI/UI_Endgame/btn_video.png", CC_CALLBACK_1(DialogPauseGame::replay, this));
	videoBtn->setAnchorPoint(Vec2(1, 0.5f));
	//videoBtn->setScale(background->getBoundingBox().size.width / 3 / videoBtn->getContentSize().width);
	videoBtn->setPosition(background->getContentSize().width, 0);
	auto menu = Menu::create(reviveBtn, videoBtn, nullptr);

	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);
}

void DialogPauseGame::initForWin()
{
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin + win_size / 2);
	addChild(background, 1);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5, 0));
	chim->setPosition(origin.x + win_size.width/2, origin.y + win_size.height*2/3);
	this->addChild(chim, 0);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::exitToStage, this));
	backBtn->setAnchorPoint(Vec2(0, 1));
	//backBtn->setScale(background->getBoundingBox().size.width / 3 / backBtn->getContentSize().width);
	backBtn->setPosition(background->getContentSize().width*0.1,0);

	auto nextBtn = MenuItemImage::create("UI/UI_Endgame/btn_next.png", "UI/UI_Endgame/btn_next.png", CC_CALLBACK_1(DialogPauseGame::nextState, this));
	nextBtn->setAnchorPoint(Vec2(1, 1));
	//nextBtn->setScale(background->getBoundingBox().size.width / 3 / nextBtn->getContentSize().width);
	nextBtn->setPosition(background->getContentSize().width*0.9, 0);

	auto menu = Menu::create(backBtn, nextBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);
}

void DialogPauseGame::backToPlay(Ref * pSender)
{
	log("Back to play");
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->resumeGame();
}

void DialogPauseGame::exitToStage(Ref * pSender)
{
	Director::getInstance()->replaceScene(MenuLayer::createScene());
}

void DialogPauseGame::replay(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->restartGame();
}

void DialogPauseGame::nextState(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->nextGame();
}

