
#include "DialogPauseGame.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "manager/RefManager.h"
#include "ui_custom/CustomLayerToToast.h"



bool DialogPauseGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	AdmobHelper::getInstance()->showTopBanner();
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

void DialogPauseGame::onExit()
{
	AdmobHelper::getInstance()->hideTopBanner();
}


void DialogPauseGame::resumeGame(Ref * pSender)
{
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->resumeGame();
}

void DialogPauseGame::backHome(Ref * pSender)
{
	AdmobHelper::getInstance()->showFullAd();
	auto gameScene = this->getParent();
	gameScene->removeAllChildrenWithCleanup(true);
	Director::getInstance()->replaceScene(MenuLayer::createScene());
}

void DialogPauseGame::overGame()
{
	this->stopAllActions();
	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	this->removeFromParentAndCleanup(true);

	parentLayer->overGame();
}

void DialogPauseGame::replayGame(Ref * pSender, int goldRevive, bool isWatchVideo)
{
	AdmobHelper::getInstance()->showFullAd();
	//log("%i", goldRevive);
	if (!isWatchVideo) {
		if (REF->setDownGold(goldRevive)) {
			auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
			gameLayer->reviveHero();
		}
		else {
			CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(2), TOAST_SHORT);
			_pToast->setPosition(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 7));
			addChild(_pToast, 10);
		}
	}
	else {
		auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
		gameLayer->reviveHero();
	}
}

void DialogPauseGame::nextState(Ref * pSender)
{
	if (!REF->getIsLockedHero()) {
		auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
		gameLayer->nextGame();
	}
	else {
		auto gameScene = this->getParent();
		gameScene->removeAllChildrenWithCleanup(true);
		Director::getInstance()->replaceScene(MenuLayer::createScene());
	}
}

void DialogPauseGame::restartGame(Ref * pSender)
{
	AdmobHelper::getInstance()->showFullAd();
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	
	if (!REF->getIsLockedHero()) {
		if (REF->getNumberOfLife() > 0) {
			REF->setDownLife(1);
			gameLayer->restartGame();
		}
		else {
			CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(1), TOAST_SHORT);
			_pToast->setPosition(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 4));
			addChild(_pToast, 10);
		}
	}
	else {
		gameLayer->restartGame();
	}
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


void DialogPause::selectedEventMusic(Ref * pSender, ui::CheckBox::EventType type)
{
	switch (type)
	{
	case ui::CheckBox::EventType::SELECTED:
		log("Music enable");
		break;

	case ui::CheckBox::EventType::UNSELECTED:
		log("Music disable");
		break;

	default:
		break;
	}
}

void DialogPause::selectedEventSound(Ref * pSender, ui::CheckBox::EventType type)
{
	switch (type)
	{
	case ui::CheckBox::EventType::SELECTED:
		log("Sound enable");
		break;

	case ui::CheckBox::EventType::UNSELECTED:
		log("Sound disable");
		break;

	default:
		break;
	}
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
	checkBoxMusic->addEventListener(CC_CALLBACK_2(DialogPause::selectedEventMusic, this));
	checkBoxMusic->setPosition(Vec2(background->getContentSize().width*0.25f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxMusic);

	auto checkBoxSound = ui::CheckBox::create("UI/UI_Endgame/btn_off.png", "UI/UI_Endgame/btn_on.png");
	checkBoxSound->addEventListener(CC_CALLBACK_2(DialogPause::selectedEventSound, this));
	checkBoxSound->setPosition(Vec2(background->getContentSize().width*0.63f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxSound);

	auto homeBtnNormal = Sprite::create("UI/UI_Endgame/btn_home.png");
	auto homeBtnActive = Sprite::create("UI/UI_Endgame/btn_home.png");
	homeBtnActive->setColor(Color3B(128, 128, 128));
	auto homeBtn = MenuItemSprite::create(homeBtnNormal, homeBtnActive, CC_CALLBACK_1(DialogPauseGame::backHome, this));
	homeBtn->setAnchorPoint(Vec2(0.7, 0.5));
	homeBtn->setPosition(background->getContentSize() / 3);

	auto resumeBtnNormal = Sprite::create("UI/UI_Endgame/btn_resume.png");
	auto resumeBtnActive = Sprite::create("UI/UI_Endgame/btn_resume.png");
	resumeBtnActive->setColor(Color3B(128, 128, 128));
	auto resumeBtn = MenuItemSprite::create(resumeBtnNormal, resumeBtnActive, CC_CALLBACK_1(DialogPauseGame::resumeGame, this));
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

	int gold = calGoldRevive(numberOfRevive);

	auto origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("UI/UI_Endgame/paper_revive.png");
	background->setScale(SCREEN_SIZE.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3);
	addChild(background);

	auto reviveBtnNormal = Sprite::create("UI/UI_Endgame/btn_revive.png");
	auto reviveBtnActive = Sprite::create("UI/UI_Endgame/btn_revive.png");
	reviveBtnActive->setColor(Color3B(128, 128, 128));
	auto reviveBtn = MenuItemSprite::create(reviveBtnNormal, reviveBtnActive, CC_CALLBACK_1(DialogPauseGame::replayGame, this, gold, false));
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
			overGame();
		}
	});
	this->runAction(RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(0.1f), countdownFun)));

	auto videoBtnNormal = Sprite::create("UI/UI_Endgame/btn_video.png");
	auto videoBtnActive = Sprite::create("UI/UI_Endgame/btn_video.png");
	videoBtnActive->setColor(Color3B(128, 128, 128));
	auto videoBtn = MenuItemSprite::create(videoBtnNormal, videoBtnActive, CC_CALLBACK_1(DialogPauseGame::replayGame, this, gold, true));
	videoBtn->setAnchorPoint(Vec2(1, 0.5f));
	videoBtn->setPosition(background->getContentSize().width, 0);

	auto exitBtnNormal = Sprite::create("UI/UI_Endgame/btn_close.png");
	auto exitBtnActive = Sprite::create("UI/UI_Endgame/btn_close.png");
	exitBtnActive->setColor(Color3B(128, 128, 128));
	auto exitBtn = MenuItemSprite::create(exitBtnNormal, exitBtnActive, CC_CALLBACK_0(DialogPauseGame::overGame, this));
	exitBtn->setAnchorPoint(Vec2(1, 1));
	exitBtn->setPosition(background->getContentSize().width, background->getContentSize().height * 0.8f);

	// show gold revive
	goldReviveLb = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", gold));
	goldReviveLb->setBMFontSize(exitBtnNormal->getBoundingBox().size.height * 0.75f);
	goldReviveLb->setAnchorPoint(Vec2::ZERO);
	goldReviveLb->setPosition(reviveBtn->getContentSize().width * 0.43f, reviveBtn->getContentSize().height * 0.43f);
	reviveBtn->addChild(goldReviveLb);


	auto menu = Menu::create(reviveBtn, videoBtn, exitBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);

	effect();

	return true;
}

int DialogRevive::calGoldRevive(int number)
{
	int goldRevive = 0;
	if (number == 1 || number == 0) return 300;
	goldRevive = (number - 1) * 100 + calGoldRevive(number - 1);
	return goldRevive;
}

DialogRevive * DialogRevive::create(int numberOfRevive)
{
	DialogRevive* dialog = new DialogRevive();

	if (dialog && dialog->init(numberOfRevive)) {
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

	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setName("bg");
	background->setScale(SCREEN_SIZE.width / 1.8f / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height*0.4f);
	addChild(background, 2);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3 - SCREEN_SIZE.height * 0.1f);
	this->addChild(chim, 1);


	auto star_backbroad = SkeletonAnimation::createWithFile("Effect/star_backbroad.json", "Effect/star_backbroad.atlas", chim->getBoundingBox().size.width / 500);
	star_backbroad->setPosition(chim->getPositionX(), chim->getPositionY() + chim->getBoundingBox().size.height * 0.5f);
	star_backbroad->addAnimation(0, "star_backbroad", true);
	this->addChild(star_backbroad, 0);

	auto stateclear = Sprite::create("UI/UI_Endgame/stage_clear.png");
	stateclear->setPosition(chim->getContentSize() / 2);
	chim->addChild(stateclear, 0);

	auto backBtnNormal = Sprite::create("UI/UI_Endgame/btn_back.png");
	auto backBtnActive = Sprite::create("UI/UI_Endgame/btn_back.png");
	backBtnActive->setColor(Color3B(128, 128, 128));
	auto backBtn = MenuItemSprite::create(backBtnNormal, backBtnActive, CC_CALLBACK_1(DialogPauseGame::backHome, this));
	backBtn->setAnchorPoint(Vec2(0, 1));
	backBtn->setPosition(background->getContentSize().width*0.1, 0);

	auto nextBtnNormal = Sprite::create("UI/UI_Endgame/btn_next.png");
	auto nextBtnActive = Sprite::create("UI/UI_Endgame/btn_next.png");
	nextBtnActive->setColor(Color3B(128, 128, 128));
	auto nextBtn = MenuItemSprite::create(nextBtnNormal, nextBtnActive, CC_CALLBACK_1(DialogPauseGame::nextState, this));
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

	int bonusGold = REF->getBonusGold() / 100.0f * gold;
	bonusGoldLb = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", bonusGold));
	bonusGoldLb->setBMFontSize(goldLb->getBMFontSize() * 0.75f);
	bonusGoldLb->setAnchorPoint(Vec2::ZERO);
	bonusGoldLb->setPosition(background->getContentSize().width * 0.55f, background->getContentSize().height * 0.22f);
	background->addChild(bonusGoldLb);

	int bonusScore = REF->getBonusScore() / 100.0f * score;
	bonusScoreLb = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", bonusScore));
	bonusScoreLb->setBMFontSize(bonusGoldLb->getBMFontSize());
	bonusScoreLb->setAnchorPoint(Vec2::ZERO);
	bonusScoreLb->setPosition(background->getContentSize().width * 0.55f, background->getContentSize().height * 0.33f);
	background->addChild(bonusScoreLb);

	if (!REF->getIsLockedHero()) {	// if hero is unlocked
		REF->setUpScore(score + bonusScore);
		REF->setUpGoldExplored(gold + bonusGold);
	}
	
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

void DialogStageClear::effect()
{
	auto mainEffect = CallFunc::create([&]() {
		DialogPauseGame::effect();
	});

	auto runParticle = CallFunc::create([&]() {

		auto actionSpawn = CallFunc::create([&]() {
			/*auto fw = Sprite::create("Effect/firework_circle.png");
			fw->setScale(bonusScoreLb->getScale() / 10);
			fw->setPosition(CCRANDOM_0_1() * this->getChildByName("bg")->getContentSize().width,
				this->getChildByName("bg")->getContentSize().height +
				CCRANDOM_0_1() * this->getChildByName("bg")->getContentSize().height * 0.4f);
			this->getChildByName("bg")->addChild(fw, 2);
			fw->runAction(Sequence::create(ScaleBy::create(1.7f, 5 * Director::getInstance()->getContentScaleFactor()),
				CallFunc::create([&]() {fw->setVisible(false); }), nullptr));*/

			auto particlePaper = ParticleSystemQuad::create("Effect/firework_paper.plist");
			particlePaper->setDuration(0.2f);
			particlePaper->setScale(bonusGoldLb->getScale() / 3);
			particlePaper->setPosition(CCRANDOM_0_1() * this->getChildByName("bg")->getContentSize().width,
				this->getChildByName("bg")->getContentSize().height +
				CCRANDOM_0_1() * this->getChildByName("bg")->getContentSize().height * 0.4f);
			this->getChildByName("bg")->addChild(particlePaper, 2);

			auto particleStar = ParticleSystemQuad::create("Effect/firework_star.plist");
			particlePaper->setDuration(0.1f);
			particleStar->setScale(bonusGoldLb->getScale() / 3);
			particleStar->setPosition(particlePaper->getPosition());
			this->getChildByName("bg")->addChild(particleStar, 2);
		});

		auto seq = Sequence::create(actionSpawn, DelayTime::create(CCRANDOM_0_1() + 0.75f), nullptr);

		auto forever = RepeatForever::create(seq);
		this->runAction(forever);
	});

	this->runAction(Sequence::create(mainEffect, DelayTime::create(0.5f), runParticle, nullptr));
}


bool DialogOverGame::init(int score, int gold)
{
	DialogPauseGame::init();

	auto origin = Director::getInstance()->getVisibleOrigin();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setName("bg");
	background->setScale(SCREEN_SIZE.width / 1.8f / background->getContentSize().width);
	background->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.4f);
	addChild(background, 2);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 2 / 3 - SCREEN_SIZE.height * 0.1f);
	this->addChild(chim, 1);

	auto star_backbroad = SkeletonAnimation::createWithFile("Effect/star_backbroad.json", "Effect/star_backbroad.atlas", chim->getBoundingBox().size.width / 500);
	star_backbroad->setPosition(chim->getPositionX(), chim->getPositionY() + chim->getBoundingBox().size.height * 0.5f);
	star_backbroad->addAnimation(0, "star_backbroad", true);
	this->addChild(star_backbroad, 0);

	auto gameover = Sprite::create("UI/UI_Endgame/gameover.png");
	gameover->setPosition(chim->getContentSize() / 2);
	chim->addChild(gameover, 0);

	auto backBtnNormal = Sprite::create("UI/UI_Endgame/btn_back.png");
	auto backBtnActive = Sprite::create("UI/UI_Endgame/btn_back.png");
	backBtnActive->setColor(Color3B(128, 128, 128));
	auto backBtn = MenuItemSprite::create(backBtnNormal, backBtnActive, CC_CALLBACK_1(DialogPauseGame::backHome, this));
	backBtn->setAnchorPoint(Vec2(1, 1));
	backBtn->setPosition(background->getContentSize().width*0.1f, 0);

	auto upgradeBtnNormal = Sprite::create("UI/UI_Endgame/btn_upgrade.png");
	auto upgradeBtnActive = Sprite::create("UI/UI_Endgame/btn_upgrade.png");
	upgradeBtnActive->setColor(Color3B(128, 128, 128));
	auto upgradeBtn = MenuItemSprite::create(upgradeBtnNormal, upgradeBtnActive, CC_CALLBACK_1(DialogPauseGame::upgrade, this));
	upgradeBtn->setAnchorPoint(Vec2(0.5f, 1));
	upgradeBtn->setPosition(background->getContentSize().width*0.5f, 0);

	auto restartBtnNormal = Sprite::create("UI/UI_Endgame/btn_restart.png");
	auto restartBtnActive = Sprite::create("UI/UI_Endgame/btn_restart.png");
	restartBtnActive->setColor(Color3B(128, 128, 128));
	auto restartBtn = MenuItemSprite::create(restartBtnNormal, restartBtnActive, CC_CALLBACK_1(DialogPauseGame::restartGame, this));
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

	int bonusGold = REF->getBonusGold() / 100.0f * gold;
	bonusGoldLb = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", bonusGold));
	bonusGoldLb->setBMFontSize(goldLb->getBMFontSize() * 0.75f);
	bonusGoldLb->setAnchorPoint(Vec2::ZERO);
	bonusGoldLb->setPosition(background->getContentSize().width * 0.55f, background->getContentSize().height * 0.22f);
	background->addChild(bonusGoldLb);

	int bonusScore = REF->getBonusScore() / 100.0f * score;
	bonusScoreLb = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", bonusScore));
	bonusScoreLb->setBMFontSize(bonusGoldLb->getBMFontSize());
	bonusScoreLb->setAnchorPoint(Vec2::ZERO);
	bonusScoreLb->setPosition(background->getContentSize().width * 0.55f, background->getContentSize().height * 0.33f);
	background->addChild(bonusScoreLb);

	if (!REF->getIsLockedHero()) {		// if hero is unlocked
		REF->setUpScore(score + bonusScore);
		REF->setUpGoldExplored(gold + bonusGold);
	}


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

void DialogOverGame::effect()
{
	DialogPauseGame::effect();
}
