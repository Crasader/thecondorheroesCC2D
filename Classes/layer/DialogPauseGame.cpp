#include "layer/DialogPauseGame.h"
#include "layer/GameScene.h"
#include "layer/MenuScene.h"
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
	case 2: {
		initForGameover();
		break;
	}
	default:
		initForPause();
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

	
	auto background = Sprite::create("UI/UI_Endgame/paper_pause.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin + win_size / 2);
	addChild(background);

	auto checkBoxMusic = ui::CheckBox::create("UI/UI_Endgame/btn_off.png", "UI/UI_Endgame/btn_on.png");
	checkBoxMusic->setPosition(Vec2(background->getContentSize().width*0.25f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxMusic);

	auto checkBoxSound = ui::CheckBox::create("UI/UI_Endgame/btn_off.png", "UI/UI_Endgame/btn_on.png");
	checkBoxSound->setPosition(Vec2(background->getContentSize().width*0.63f, background->getContentSize().height*0.7f));
	background->addChild(checkBoxSound);
	auto homeBtn = MenuItemImage::create("UI/UI_Endgame/btn_home.png", "UI/UI_Endgame/btn_home.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	homeBtn->setAnchorPoint(Vec2(0.7, 0.5));
	//homeBtn->setScale(background->getBoundingBox().size.width / 3 / homeBtn->getContentSize().width);
	homeBtn->setPosition(background->getContentSize()/3);

	auto resumeBtn = MenuItemImage::create("UI/UI_Endgame/btn_resume.png", "UI/UI_Endgame/btn_resume.png", CC_CALLBACK_1(DialogPauseGame::resumeGame, this));
	resumeBtn->setAnchorPoint(Vec2(0.4, 0.5));
	//resumeBtn->setScale(background->getBoundingBox().size.width / 3 / resumeBtn->getContentSize().width);
	resumeBtn->setPosition(background->getContentSize().width*2/3, background->getContentSize().height*1/3);

	auto menu = Menu::create(homeBtn, resumeBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);
}

void DialogPauseGame::initForDie()
{
	//countDown = 5;
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("UI/UI_Endgame/paper_revive.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + win_size.width / 2, origin.y + win_size.height*2/3);
	addChild(background);

	auto reviveBtn = MenuItemImage::create("UI/UI_Endgame/btn_revive.png", "UI/UI_Endgame/btn_revive.png", CC_CALLBACK_1(DialogPauseGame::replayGame, this));
	reviveBtn->setAnchorPoint(Vec2(0,0.5f ));
	//reviveBtn->setScale(background->getBoundingBox().size.width / 3 / reviveBtn->getContentSize().width);
	reviveBtn->setPosition(0, 0);

	 loading = ui::LoadingBar::create("UI/UI_Endgame/timeline_revive.png");
	//loading->setAnchorPoint(Vec2::ZERO);
	//loading->setScaleX(boardTime->getBoundingBox().size.width * 0.96f / loading->getContentSize().width);
	//loading->setScaleY(boardTime->getBoundingBox().size.height * 0.6f / loading->getContentSize().height);
	loading->setPosition(Vec2( reviveBtn->getContentSize().width / 2.05f, reviveBtn->getContentSize().height*0.275f));
	loading->setPercent(100);
	reviveBtn->addChild(loading);

	auto countdownFun = CallFunc::create([&]() {
		this->countDown--;
		loading->setPercent(this->countDown*2);
		if (this->countDown <= 0) {
			this->stopAllActions();
			this->removeAllChildren();
			this->initForGameover();
		}
	});
	this->runAction(RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(0.1f),countdownFun)));

	auto videoBtn = MenuItemImage::create("UI/UI_Endgame/btn_video.png", "UI/UI_Endgame/btn_video.png", CC_CALLBACK_1(DialogPauseGame::replayGame, this));
	videoBtn->setAnchorPoint(Vec2(1, 0.5f));
	//videoBtn->setScale(background->getBoundingBox().size.width / 3 / videoBtn->getContentSize().width);
	videoBtn->setPosition(background->getContentSize().width, 0);

	auto exitBtn = MenuItemImage::create("UI/UI_Endgame/btn_close.png", "UI/UI_Endgame/btn_close.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	exitBtn->setAnchorPoint(Vec2(1, 1));
	exitBtn->setPosition(background->getContentSize().width, background->getContentSize().height * 0.8f);

	auto menu = Menu::create(reviveBtn, videoBtn, exitBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);
}

void DialogPauseGame::initForGameover()
{
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + win_size.width / 2, origin.y + win_size.height*0.4f);
	addChild(background, 1);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + win_size.width / 2, origin.y + win_size.height * 2 / 3 - win_size.height * 0.1f);
	this->addChild(chim, 0);

	auto gameover = Sprite::create("UI/UI_Endgame/gameover.png");
	//gameover->setScale(background->getScale());
	//gameover->setAnchorPoint(Point(0.5f, 0.1f));
	gameover->setPosition(chim->getContentSize()/2);
	chim->addChild(gameover, 0);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
	backBtn->setAnchorPoint(Vec2(1, 1));
	//backBtn->setScale(background->getBoundingBox().size.width / 3 / backBtn->getContentSize().width);
	backBtn->setPosition(background->getContentSize().width*0.1f, 0);

	auto upgradeBtn = MenuItemImage::create("UI/UI_Endgame/btn_upgrade.png", "UI/UI_Endgame/btn_upgrade.png", CC_CALLBACK_1(DialogPauseGame::nextState, this));
	upgradeBtn->setAnchorPoint(Vec2(0.5f, 1));
	//nextBtn->setScale(background->getBoundingBox().size.width / 3 / nextBtn->getContentSize().width);
	upgradeBtn->setPosition(background->getContentSize().width*0.5f, 0);

	auto restartBtn = MenuItemImage::create("UI/UI_Endgame/btn_restart.png", "UI/UI_Endgame/btn_restart.png", CC_CALLBACK_1(DialogPauseGame::nextState, this));
	restartBtn->setAnchorPoint(Vec2(0, 1));
	//nextBtn->setScale(background->getBoundingBox().size.width / 3 / nextBtn->getContentSize().width);
	restartBtn->setPosition(background->getContentSize().width*0.9f, 0);

	auto menu = Menu::create(backBtn, upgradeBtn,restartBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	background->addChild(menu);
}

void DialogPauseGame::initForWin()
{
	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	auto background = Sprite::create("UI/UI_Endgame/paper.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin.x + win_size.width / 2, origin.y+win_size.height*0.4f);
	addChild(background, 1);

	auto chim = Sprite::create("UI/UI_Endgame/background_ending.png");
	chim->setScale(background->getScale());
	chim->setAnchorPoint(Point(0.5f, 0.1f));
	chim->setPosition(origin.x + win_size.width/2, origin.y + win_size.height*2/3- win_size.height * 0.1f);
	this->addChild(chim, 0);

	auto stateclear = Sprite::create("UI/UI_Endgame/stage_clear.png");
	//stateclear->setScale(background->getScale());
	//gameover->setAnchorPoint(Point(0.5f, 0.1f));
	stateclear->setPosition(chim->getContentSize() / 2);
	chim->addChild(stateclear, 0);

	auto backBtn = MenuItemImage::create("UI/UI_Endgame/btn_back.png", "UI/UI_Endgame/btn_back.png", CC_CALLBACK_1(DialogPauseGame::backHome, this));
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

