#include "DialogPauseGame.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"


bool DialogPauseGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin + win_size / 2);
	addChild(background);

	backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backToPlay, this));
	backBtn->setAnchorPoint(Vec2(0.5, 1));
	backBtn->setScale(background->getBoundingBox().size.width / 3 / backBtn->getContentSize().width);
	backBtn->setPosition(background->getPositionX() - background->getBoundingBox().size.width / 2,
						background->getPositionY() - background->getBoundingBox().size.height / 2);
	
	exitBtn = MenuItemImage::create("UI/UI_Endgame/btn_restart.png", "UI/UI_Endgame/btn_restart.png", CC_CALLBACK_1(DialogPauseGame::exitToStage, this));
	exitBtn->setAnchorPoint(Vec2(0.5, 1));
	exitBtn->setScale(background->getBoundingBox().size.width / 3 / exitBtn->getContentSize().width);
	exitBtn->setPosition(background->getPositionX() + background->getBoundingBox().size.width / 2,
						background->getPositionY() - background->getBoundingBox().size.height / 2);

	auto menu = Menu::create(backBtn, exitBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return true;
}

DialogPauseGame* DialogPauseGame::create()
{
	DialogPauseGame* dialog = new DialogPauseGame();

	if (dialog && dialog->init())
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

void DialogPauseGame::backToPlay(Ref * pSender)
{
	log("Back to play");
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->resumeGame();
}

void DialogPauseGame::exitToStage(Ref * pSender)
{
	log("revive");
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->resumeGame();
	gameLayer->reviveHero();
}

