#include "layer/DialogPauseGame.h"
#include "layer/GameScene.h"
#include "layer/MenuScene.h"
#include "SimpleAudioEngine.h"



bool DialogPauseGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

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


void DialogPauseGame::resumeGame(Ref * pSender)
{
	log("Back to play");
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->resumeGame();
}

void DialogPauseGame::backHome(Ref * pSender)
{
	Director::getInstance()->replaceScene(MenuLayer::createScene());
}

void DialogPauseGame::replayGame(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->reviveHero();
}

void DialogPauseGame::nextState(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->nextGame();
}

void DialogPauseGame::restartGame(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->restartGame();
}

void DialogPauseGame::upgrade(Ref * pSender)
{
}

void DialogPauseGame::effect()
{
	this->setPosition(0, SCREEN_SIZE.height * 1.1f);
	auto actionMove = MoveTo::create(0.4f, Vec2::ZERO);
	auto effect = EaseBackOut::create(actionMove);

	this->runAction(effect);
}


bool DialogPause::init()
{
	DialogPauseGame::init();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("UI/UI_Endgame/paper_pause.png");
	background->setScale(SCREEN_SIZE.width / 2 / background->getContentSize().width);
	background->setPosition(origin + SCREEN_SIZE / 2);
	addChild(background);

	auto checkBoxMusic = ui::CheckBox::create("UI/UI_Endgame/btn_off.png", "UI/UI_Endgame/btn_on.png");
	checkBoxMusic->setPosition(Vec2(background->getContentSize().width*0.25f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxMusic);

	auto checkBoxSound = ui::CheckBox::create("UI/UI_Endgame/btn_off.png", "UI/UI_Endgame/btn_on.png");
	checkBoxSound->setPosition(Vec2(background->getContentSize().width*0.63f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxSound);
	auto homeBtn = MenuItemImage::create("UI/UI_Endgame/btn_home.png", "UI/UI_Endgame/btn_home.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	homeBtn->setAnchorPoint(Vec2(0.7, 0.5));
	homeBtn->setPosition(background->getContentSize() / 3);

	auto resumeBtn = MenuItemImage::create("UI/UI_Endgame/btn_resume.png", "UI/UI_Endgame/btn_resume.png", CC_CALLBACK_1(DialogPauseGame::resumeGame, this));
	resumeBtn->setAnchorPoint(Vec2(0.4, 0.5));
	resumeBtn->setPosition(background->getContentSize().width * 2 / 3, background->getContentSize().height * 1 / 3);

	auto menu = Menu::create(homeBtn, resumeBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);

	effect();

	return true;
}

DialogPause * DialogPause::create()
{
	DialogPause* dialog = new DialogPause();

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
	return dialog;
}

bool DialogRevive::init(int numberOfRevive)
{
	DialogPauseGame::init();
	this->numberOfRevive = numberOfRevive;

	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("UI/UI_Endgame/paper_revive.png");
	background->setScale(SCREEN_SIZE.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3);
	addChild(background);

	auto reviveBtn = MenuItemImage::create("UI/UI_Endgame/btn_revive.png", "UI/UI_Endgame/btn_revive.png", CC_CALLBACK_1(DialogPauseGame::replayGame, this));
	reviveBtn->setAnchorPoint(Vec2(0, 0.5f));
	reviveBtn->setPosition(0, 0);

	loading = ui::LoadingBar::create("UI/UI_Endgame/timeline_revive.png");
	loading->setPosition(Vec2(reviveBtn->getContentSize().width / 2.05f, reviveBtn->getContentSize().height*0.275f));
	loading->setPercent(100);
	reviveBtn->addChild(loading);

	auto countdownFun = CallFunc::create([&]() {
		this->countDown--;
		loading->setPercent(this->countDown * 2);
		if (this->countDown <= 0) {
			this->stopAllActions();
			auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
			this->removeFromParentAndCleanup(true);
			
			parentLayer->overGame();
		}
	});
	this->runAction(RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(0.1f), countdownFun)));

	auto videoBtn = MenuItemImage::create("UI/UI_Endgame/btn_video.png", "UI/UI_Endgame/btn_video.png", CC_CALLBACK_1(DialogPauseGame::replayGame, this));
	videoBtn->setAnchorPoint(Vec2(1, 0.5f));
	videoBtn->setPosition(background->getContentSize().width, 0);

	auto exitBtn = MenuItemImage::create("UI/UI_Endgame/btn_close.png", "UI/UI_Endgame/btn_close.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	exitBtn->setAnchorPoint(Vec2(1, 1));
	exitBtn->setPosition(background->getContentSize().width, background->getContentSize().height * 0.8f);

	auto menu = Menu::create(reviveBtn, videoBtn, exitBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);

	effect();

	return true;
}

DialogRevive * DialogRevive::create(int numberOfRevive)
{
	DialogRevive* dialog = new DialogRevive();

	if (dialog && dialog->init(numberOfRevive))
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
	return dialog;
}

bool DialogStageClear::init(int score, int gold)
{
	DialogPauseGame::init();
	this->score = score;
	this->gold = gold;

	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(SCREEN_SIZE.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height*0.4f);
	addChild(background, 1);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3 - SCREEN_SIZE.height * 0.1f);
	this->addChild(chim, 0);

	auto stateclear = Sprite::create("UI/UI_Endgame/stage_clear.png");
	stateclear->setPosition(chim->getContentSize() / 2);
	chim->addChild(stateclear, 0);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	backBtn->setAnchorPoint(Vec2(0, 1));
	backBtn->setPosition(background->getContentSize().width*0.1, 0);

	auto nextBtn = MenuItemImage::create("UI/UI_Endgame/btn_next.png", "UI/UI_Endgame/btn_next.png", CC_CALLBACK_1(DialogPauseGame::nextState, this));
	nextBtn->setAnchorPoint(Vec2(1, 1));
	nextBtn->setPosition(background->getContentSize().width*0.9, 0);

	auto menu = Menu::create(backBtn, nextBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);

	// LABEL
	goldLb = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", gold));
	goldLb->setBMFontSize(backBtn->getBoundingBox().size.height * 0.55f);
	goldLb->setAnchorPoint(Vec2::ZERO);
	goldLb->setPosition(background->getContentSize().width / 2, background->getContentSize().height * 0.52f);
	background->addChild(goldLb);

	scoreLb = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", score));
	scoreLb->setBMFontSize(goldLb->getBMFontSize());
	scoreLb->setAnchorPoint(Vec2::ZERO);
	scoreLb->setPosition(background->getContentSize().width / 2, background->getContentSize().height * 0.67f);
	background->addChild(scoreLb);

	effect();

	return true;
}

DialogStageClear * DialogStageClear::create(int score, int gold)
{
	DialogStageClear* dialog = new DialogStageClear();

	if (dialog && dialog->init(score, gold))
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
	return dialog;
}

bool DialogOverGame::init(int score, int gold)
{
	DialogPauseGame::init();
	this->score = score;
	this->gold = gold;

	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(SCREEN_SIZE.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.4f);
	addChild(background, 1);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3 - SCREEN_SIZE.height * 0.1f);
	this->addChild(chim, 0);

	auto gameover = Sprite::create("UI/UI_Endgame/gameover.png");
	gameover->setPosition(chim->getContentSize() / 2);
	chim->addChild(gameover, 0);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	backBtn->setAnchorPoint(Vec2(1, 1));
	backBtn->setPosition(background->getContentSize().width*0.1f, 0);

	auto upgradeBtn = MenuItemImage::create("UI/UI_Endgame/btn_upgrade.png", "UI/UI_Endgame/btn_upgrade.png", CC_CALLBACK_1(DialogPauseGame::upgrade, this));
	upgradeBtn->setAnchorPoint(Vec2(0.5f, 1));
	upgradeBtn->setPosition(background->getContentSize().width*0.5f, 0);

	auto restartBtn = MenuItemImage::create("UI/UI_Endgame/btn_restart.png", "UI/UI_Endgame/btn_restart.png", CC_CALLBACK_1(DialogPauseGame::restartGame, this));
	restartBtn->setAnchorPoint(Vec2(0, 1));
	restartBtn->setPosition(background->getContentSize().width*0.9f, 0);

	auto menu = Menu::create(backBtn, upgradeBtn, restartBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);

	// LABEL
	goldLb = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", gold));
	goldLb->setBMFontSize(backBtn->getBoundingBox().size.height * 0.55f);
	goldLb->setAnchorPoint(Vec2::ZERO);
	goldLb->setPosition(background->getContentSize().width / 2, background->getContentSize().height * 0.52f);
	background->addChild(goldLb);

	scoreLb = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", score));
	scoreLb->setBMFontSize(goldLb->getBMFontSize());
	scoreLb->setAnchorPoint(Vec2::ZERO);
	scoreLb->setPosition(background->getContentSize().width / 2, background->getContentSize().height * 0.67f);
	background->addChild(scoreLb);

	effect();

	return true;
}

DialogOverGame * DialogOverGame::create(int score, int gold)
{
	DialogOverGame* dialog = new DialogOverGame();

	if (dialog && dialog->init(score, gold))
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
	return dialog;
}
