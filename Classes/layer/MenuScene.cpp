#include "MenuScene.h"
#include "LoadingLayer.h"
#include "IntroScene.h"
#include "ui_custom\Twinkle.h"
#include "manager\AudioManager.h"
#include "manager\JSonHeroManager.h"
#include "manager\JSonMenuManager.h"
#include "manager\JSonQuestManager.h"
#include "manager\RefManager.h"

//#include "MenuScene.h"
//#include "LoadingLayer.h"
//#include "IntroScene.h"
//#include "Twinkle.h"
//#include "AudioManager.h"
//#include "JSonHeroManager.h"
//#include "JSonMenuManager.h"
//#include "JSonQuestManager.h"
//#include "RefManager.h"

MenuLayer * MenuLayer::create(bool p_bOnlySelectStage) {
	MenuLayer *pRet = new(std::nothrow) MenuLayer();
	if (pRet && pRet->init(p_bOnlySelectStage)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool MenuLayer::init(bool p_bOnlySelectStage) {
	if (!Layer::init()) {
		return false;
	}
	initInputData();
	Vec2 _v2Origin = Director::getInstance()->getVisibleOrigin();
	this->setPosition(_v2Origin);

	m_pBuyPackConfirmBackground = LayerColor::create(Color4B(0, 0, 0, 200));
	m_pBuyPackConfirmBackground->setVisible(false);
	this->addChild(m_pBuyPackConfirmBackground, 10);

	m_pShopBlurBackground = LayerColor::create(Color4B(0, 0, 0, 220));
	m_pShopBlurBackground->setVisible(false);
	this->addChild(m_pShopBlurBackground, 5);

	// shop
	m_pShopBoardLayer = Layer::create();
	this->addChild(m_pShopBoardLayer, 6);

	m_pBlurScreen = Layer::create();
	this->addChild(m_pBlurScreen, 8);
	m_pBlurScreen->setVisible(false);

	key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(MenuLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

	if (p_bOnlySelectStage) {
		m_nMenuStatus = 4;
		m_pTopMainMenu = Layer::create();
		m_pTopMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f));
		m_pTopMainMenu->setPosition(m_szVisibleSize.width / 2, m_szVisibleSize.height + m_pTopMainMenu->getContentSize().height / 2);
		this->addChild(m_pTopMainMenu, 7);
		initTopMainMenu();
		moveLayerViaDirection(m_pTopMainMenu, 2);

		// select stage layer
		m_pSelectStageLayer = SelectStageLayer::create(m_nIndexHeroSelected);
		m_pSelectStageLayer->moveAva();
		this->addChild(m_pSelectStageLayer, 3);

		this->scheduleUpdate();

		return true;
	}
	createRequestToGoogle();

	m_pGameBackground = Layer::create(); // layer 1 : background
	this->addChild(m_pGameBackground, 1);
	initBackgroundLayer();

	m_pGameScene = Layer::create(); // layer 2 : scene
	this->addChild(m_pGameScene, 2);
	m_arPreviewHero[0] = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas");
	m_arPreviewHero[1] = new SkeletonAnimation("UI/UI_main_menu/PreviewCoLong/s_CoCo.json",
		"UI/UI_main_menu/PreviewCoLong/s_CoCo.atlas");
	m_arPreviewHero[2] = new SkeletonAnimation("UI/UI_main_menu/PreviewHoangDung/s_HoangDung.json",
		"UI/UI_main_menu/PreviewHoangDung/s_HoangDung.atlas");
	m_arPreviewHero[3] = new SkeletonAnimation("UI/UI_main_menu/PreviewHoangDuocSu/s_HoangDuocSu.json",
		"UI/UI_main_menu/PreviewHoangDuocSu/s_HoangDuocSu.atlas");
	m_arPreviewHero[4] = new SkeletonAnimation("UI/UI_main_menu/PreviewQuachTinh/s_QuachTinh.json",
		"UI/UI_main_menu/PreviewQuachTinh/s_QuachTinh.atlas");

	float _arScaleHero[5] = { 0.7f, 0.6f, 0.65f, 0.6f, 0.75f };
	float _arPositionXHero[5] = { 0.28f, 0.3f, 0.3f, 0.29f, 0.27f };
	float _arPositionYHero[5] = { 0.25f, 0.25f, 0.27f, 0.29f, 0.27f };
	for (int i = 0; i < 5; i++) {
		m_arPreviewHero[i]->update(0.0f);
		m_arPreviewHero[i]->setScale(m_szVisibleSize.height / m_arPreviewHero[i]->getBoundingBox().size.height * _arScaleHero[i]);
		m_arPreviewHero[i]->setPosition(Vec2(m_szVisibleSize.width * _arPositionXHero[i], m_szVisibleSize.height * _arPositionYHero[i]));
	}
	initSceneLayer();

	m_pGameControl = Layer::create(); // layer 3 : control
	this->addChild(m_pGameControl, 3);
	initControlLayer();

	this->scheduleUpdate();
#ifdef SDKBOX_ENABLED
	sdkbox::PluginVungle::setListener(this);
	sdkbox::IAP::setListener(this);
#endif

	createRequestToGoogle();
	AudioManager::stopSoundandMusic();
	AudioManager::playMusic(MUSIC_MENU);
	return true;
}

void MenuLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		if (m_nMenuStatus == 0) {
			if (m_pShopBlurBackground->isVisible()) {
				buttonCloseShopHandle();
				return;
			}

			if (backNumber == 0) {
				backNumber++;
				this->schedule(schedule_selector(MenuLayer::singlePress), 2.5f, 1, 0); // interval: 2s, repeate once, delay 0
				CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(12), TOAST_SHORT);
				_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
				this->addChild(_pToast, 10);
			}
			else {
				Director::getInstance()->end();
			}
		}
		else {
			backNumber = 0;
			buttonBackHandle();
		}
	}
}

bool MenuLayer::downLife()
{
	if (m_nLifeNumber > 0) {
		m_nLifeNumber--;
		REF->setDownLife(1);
		REF->setAnchorTime(time(0));

		initTopMainMenu();

		m_iconLife = Sprite::create("UI/UI_main_menu/TopMenu/icon_life.png");
		m_iconLife->setScale(m_pTopMainMenu->getBoundingBox().size.height / m_iconLife->getBoundingBox().size.height);
		m_iconLife->setAnchorPoint(Vec2(0, 0.5f));
		m_iconLife->setPosition(m_szVisibleSize.width * 0.128f, m_pTopMainMenu->getPositionY());
		addChild(m_iconLife, 10);

		auto moveDown = MoveBy::create(0.75f, Vec2(0.0f, -m_szVisibleSize.height * 0.3f));

		m_iconLife->schedule([&](float dt) {
			int _opacity = (int)m_iconLife->getOpacity();
			m_iconLife->setOpacity(_opacity - 1);
			if (_opacity <= 10) m_iconLife->unschedule("key");
		}, 0.003f, "key");

		m_iconLife->runAction(moveDown);

		return true;
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(1), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		addChild(_pToast, 10);

		return false;
	}
}

void MenuLayer::disableListener()
{
	m_pTopMenu->setEnabled(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(key_listener);
}

void MenuLayer::update(float p_fDelta) {
	time_t _nCurrentTime = time(0);
	if (m_nLifeNumber < 5) {
		int _arCooldownLife[5] = { 60, 120, 180, 240, 300 };
		int _nCooldownLife = _arCooldownLife[m_nLifeNumber];
		m_pTimeCounter->setVisible(true);
		int _nDeltaTime = _nCurrentTime - REF->getAnchorTime();
		if (_nDeltaTime >= _nCooldownLife) {
			m_nLifeNumber++;
			REF->setLife(m_nLifeNumber);
			REF->setAnchorTime(time(0));
			initTopMainMenu();
		}
		int _nMinute = (_nCooldownLife - _nDeltaTime) / 60;
		int _nSecond = (_nCooldownLife - _nDeltaTime) % 60;
		m_pTimeCounter->setString(StringUtils::format(_nSecond < 10 ? "%i:0%i" : "%i:%i", _nMinute, _nSecond));
	}
	else {
		m_pTimeCounter->setVisible(false);
	}
}

void MenuLayer::initInputData() {
	m_nMenuStatus = 0;
	m_nIndexHeroSelected = m_nIndexHeroPicked = REF->getLastPickHero();
	m_nLifeNumber = REF->getNumberOfLife();
	m_nCurrentGold = REF->getGoldExplored();
	m_nCurrentDiamond = REF->getDiamondBuy();
	m_arNumberItemOwning[0] = REF->getNumberItemHealth();
	m_arNumberItemOwning[1] = REF->getNumberItemBird();
	m_arNumberItemOwning[2] = REF->getNumberItemMagnet();
	m_arNumberItemOwning[3] = REF->getNumberItemDoubleGold();
	m_arNumberItemOwning[4] = REF->getNumberItemCoolDown();
	// m_arItemPrice will be loaded in initItemBoard, just once

	int _nDeltaTime = time(0) - REF->getAnchorTime();
	int _nLifeToAdd = (int)(_nDeltaTime / 300);
	int _nTimeToNextLife = (int)(_nDeltaTime % 300);
	if (m_nLifeNumber < 5) {
		m_nLifeNumber += _nLifeToAdd;
		if (m_nLifeNumber >= 5) {
			m_nLifeNumber = 5;
			REF->setAnchorTime(time(0));
		}
		else {
			REF->setAnchorTime(REF->getAnchorTime() + _nLifeToAdd * 300);
		}
		REF->setLife(m_nLifeNumber);
	}

	m_nLanguage = REF->getLanguage();
}

void MenuLayer::initBackgroundLayer() {
	m_pGameBackground->removeAllChildrenWithCleanup(true);

	auto _aMainMenuBackground = Sprite::create("UI/UI_main_menu/bg_1.png");
	float _fTemp = _aMainMenuBackground->getContentSize().height * m_szVisibleSize.width / _aMainMenuBackground->getContentSize().width;
	if (_fTemp > m_szVisibleSize.height) {
		_aMainMenuBackground->setScale(m_szVisibleSize.width / _aMainMenuBackground->getContentSize().width); // full screen size width
	}
	else {
		_aMainMenuBackground->setScale(m_szVisibleSize.height / _aMainMenuBackground->getContentSize().height); // full screen size height
	}
	_aMainMenuBackground->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2)); // center screen
	m_pGameBackground->addChild(_aMainMenuBackground, 1);

	string _arLanguages[5] = {
		"UI/UI_main_menu/PreviewDuongQua/DQ_effect%d.plist",
		"UI/UI_main_menu/PreviewCoLong/CL_effect%d.plist",
		"UI/UI_main_menu/PreviewHoangDung/HD_effect%d.plist",
		"UI/UI_main_menu/PreviewHoangDuocSu/HDS_effect%d.plist",
		"UI/UI_main_menu/PreviewQuachTinh/QT_effect%d.plist"
	};

	auto _aParticleEffect1 = ParticleSystemQuad::create(StringUtils::format(_arLanguages[m_nIndexHeroPicked].c_str(), 1));
	_aParticleEffect1->setDuration(-1);
	_aParticleEffect1->setScale(1.0f);
	_aParticleEffect1->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	m_pGameBackground->addChild(_aParticleEffect1, 2);

	auto _aParticleEffect2 = ParticleSystemQuad::create(StringUtils::format(_arLanguages[m_nIndexHeroPicked].c_str(), 2));
	_aParticleEffect2->setDuration(-1);
	_aParticleEffect2->setScale(1.0f);
	_aParticleEffect2->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	m_pGameBackground->addChild(_aParticleEffect2, 2);
}

void MenuLayer::initSceneLayer() {
	m_pGameScene->removeAllChildrenWithCleanup(true);
	m_pGameScene->addChild(m_arPreviewHero[m_nIndexHeroPicked], 1);
	m_arPreviewHero[m_nIndexHeroPicked]->setAnimation(0, "appear", false);
	m_arPreviewHero[m_nIndexHeroPicked]->addAnimation(0, "idle", true);
}

void MenuLayer::initControlLayer() {
	// top menu
	m_pTopMainMenu = Layer::create();
	m_pTopMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMainMenu->setPosition(m_szVisibleSize.width / 2, m_szVisibleSize.height + m_pTopMainMenu->getContentSize().height / 2);
	this->addChild(m_pTopMainMenu, 7);
	initTopMainMenu();

	// bottom main menu
	m_pBottomMainLayer = Layer::create();
	m_pBottomMainLayer->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 25% screen height
	m_pBottomMainLayer->setPosition(0.0f, -m_pBottomMainLayer->getContentSize().height);
	m_pGameControl->addChild(m_pBottomMainLayer, 1);
	initBottomMainMenu();

	// item board
	m_pItemBoard = Layer::create();
	m_pItemBoard->setContentSize(Size(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	m_pItemBoard->setPosition(m_szVisibleSize.width, m_szVisibleSize.height * 0.0f);
	m_pGameControl->addChild(m_pItemBoard, 1);
	initItemBoard();

	// upgrade board
	m_pUpgradeBoard = Layer::create();
	m_pUpgradeBoard->setContentSize(Size(m_szVisibleSize.width * 0.4f, m_szVisibleSize.height));
	m_pUpgradeBoard->setPosition(m_szVisibleSize.width, 0.0f);
	m_pGameControl->addChild(m_pUpgradeBoard, 1);
	initUpgradeBoard();

	// quest board
	m_pQuestBoard = Layer::create();
	m_pQuestBoard->setContentSize(Size(m_szVisibleSize.width * 0.7f, m_szVisibleSize.height));
	m_pQuestBoard->setPosition(-m_pQuestBoard->getContentSize().width, 0.0f);
	m_pGameControl->addChild(m_pQuestBoard, 1);
	initQuestBoard(0);

	// hero info board
	m_pHeroInfoBoard = Layer::create();
	m_pHeroInfoBoard->setContentSize(Size(m_szVisibleSize.width * 0.35f, m_szVisibleSize.height));
	m_pHeroInfoBoard->setPosition(-m_pHeroInfoBoard->getContentSize().width, 0.0f);
	m_pGameControl->addChild(m_pHeroInfoBoard, 1);
	initHeroInfoBoard();

	// bottom hero menu
	m_pBottomHeroLayer = Layer::create();
	m_pBottomHeroLayer->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 25% screen height
	m_pBottomHeroLayer->setPosition(0.0f, -m_pBottomHeroLayer->getContentSize().height / 2);
	m_pGameControl->addChild(m_pBottomHeroLayer, 1);
	initBottomHeroMenu();

	moveLayerViaDirection(m_pTopMainMenu, 2);
	showMainMenu();
}

void MenuLayer::initTopMainMenu() {
	m_pTopMainMenu->removeAllChildrenWithCleanup(true);

	float _fPadding = m_pTopMainMenu->getContentSize().width / 50;
	float _fXPositionCounter = -m_pTopMainMenu->getContentSize().width / 2 + _fPadding / 2;

	// button back
	MenuItemSprite *_pBackButton = createButtonOnParent(m_pTopMainMenu, NULL, "UI/UI_main_menu/TopMenu/btn_back.png",
		CC_CALLBACK_0(MenuLayer::buttonBackHandle, this), 0.1f, 0.8f, false, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pBackButton->getContentSize().width * _pBackButton->getScaleX() + _fPadding;

	// life frame
	Sprite *_pLifeFrame = createSpriteOnParent(m_pTopMainMenu, NULL, 1, "UI/UI_main_menu/TopMenu/frame_life.png", 0.35f, 0.35f, true,
		Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));

	// icons life
	for (int i = 0; i < (m_nLifeNumber > 5 ? 5 : m_nLifeNumber); i++) {
		Sprite *_pLifeIcon = Sprite::create("UI/UI_main_menu/TopMenu/icon_life.png");
		_pLifeIcon = createSpriteOnParent(NULL, _pLifeFrame, 1, "UI/UI_main_menu/TopMenu/icon_life.png", 1.0f, 1.0f, false, Vec2(0.0f, 0.5f),
			Vec2(_pLifeIcon->getContentSize().width * i * 0.9f, _pLifeFrame->getContentSize().height * 0.5f));
	}

	// label number of extra lifes
	if (m_nLifeNumber > 5) {
		Label *_pLabelExtraLife = createLabelBMOnParent(NULL, _pLifeFrame, 1, "fontsDPM/font_life-export.fnt",
			StringUtils::format("+%i", m_nLifeNumber - 5), 0.6f, TextHAlignment::RIGHT, TextVAlignment::CENTER, Vec2(1.0f, 0.5f),
			Vec2(_pLifeFrame->getContentSize().width * 0.8f, _pLifeFrame->getContentSize().height * 0.5f));
	}

	// label time for next life
	m_pTimeCounter = createLabelBMOnParent(NULL, _pLifeFrame, 1, "fontsDPM/font_life-export.fnt", StringUtils::format("5:00"), 0.6f, TextHAlignment::RIGHT,
		TextVAlignment::CENTER, Vec2(1.0f, 0.5f), Vec2(_pLifeFrame->getContentSize().width * 0.8f, _pLifeFrame->getContentSize().height * 0.5f));
	m_pTimeCounter->setVisible(false);

	_fXPositionCounter += 0.0f;

	// button add life
	MenuItemSprite *_pAddLifeButton = createButtonOnParent(m_pTopMainMenu, NULL, "UI/UI_main_menu/TopMenu/btn_add_life.png",
		CC_CALLBACK_0(MenuLayer::buttonAddLifeHandle, this), 0.35f, 0.35f, true, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pAddLifeButton->getContentSize().width * _pAddLifeButton->getScaleX() + _fPadding;

	// money frame
	Sprite *_pMoneyFrame = createSpriteOnParent(m_pTopMainMenu, NULL, 1, "UI/UI_main_menu/TopMenu/frame_money.png", 0.23f, 0.23f,
		true, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));
	Twinkle *_pTwinkleGold = Twinkle::create();
	_pTwinkleGold->setScale(_pMoneyFrame->getContentSize().height / _pTwinkleGold->getContentSize().height);
	_pTwinkleGold->setPosition(Vec2(_pMoneyFrame->getContentSize().width * 0.1f, _pMoneyFrame->getContentSize().height * 0.6f));
	_pMoneyFrame->addChild(_pTwinkleGold, 1);

	// label number of gold
	Label *_pLabelNumberGold = createLabelBMOnParent(NULL, _pMoneyFrame, 1, "fontsDPM/font_coin-export.fnt",
		StringUtils::format("%i", m_nCurrentGold), 0.6f, TextHAlignment::RIGHT, TextVAlignment::CENTER, Vec2(1.0f, 0.5f),
		Vec2(_pMoneyFrame->getContentSize().width * 0.73f, _pMoneyFrame->getContentSize().height * 0.5f));

	_fXPositionCounter += 0.0f;

	// button add gold
	MenuItemSprite *_pAddMoneyButton = createButtonOnParent(m_pTopMainMenu, NULL, "UI/UI_main_menu/TopMenu/btn_add_money_diamond.png",
		CC_CALLBACK_0(MenuLayer::buttonAddGoldHandle, this), 0.23f, 0.23f, true, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pAddMoneyButton->getContentSize().width * _pAddMoneyButton->getScaleX() + _fPadding;

	// diamond frame
	Sprite *_pDiamondFrame = createSpriteOnParent(m_pTopMainMenu, NULL, 1, "UI/UI_main_menu/TopMenu/frame_diamond.png",
		0.23f, 0.23f, true, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));
	Twinkle *_pTwinkleDiamond = Twinkle::create();
	_pTwinkleDiamond->setScale(_pDiamondFrame->getContentSize().height / _pTwinkleDiamond->getContentSize().height);
	_pTwinkleDiamond->setPosition(Vec2(_pDiamondFrame->getContentSize().width * 0.1f, _pDiamondFrame->getContentSize().height * 0.6f));
	_pDiamondFrame->addChild(_pTwinkleDiamond, 1);

	// label number of diamond
	Label *_pLabelNumberDiamond = createLabelBMOnParent(NULL, _pDiamondFrame, 1, "fontsDPM/font_diamond-export.fnt",
		StringUtils::format("%i", m_nCurrentDiamond), 0.6f, TextHAlignment::RIGHT, TextVAlignment::CENTER, Vec2(1.0f, 0.5f),
		Vec2(_pDiamondFrame->getContentSize().width * 0.73f, _pDiamondFrame->getContentSize().height * 0.5f));

	_fXPositionCounter += 0.0f;

	// add diamond button
	MenuItemSprite *_pAddDiamondButton = createButtonOnParent(m_pTopMainMenu, NULL, "UI/UI_main_menu/TopMenu/btn_add_money_diamond.png",
		CC_CALLBACK_0(MenuLayer::buttonAddDiamondHandle, this), 0.23f, 0.23f, true, Vec2(0.0f, 0.5f), Vec2(_fXPositionCounter, 0.0f));

	m_pTopMenu = Menu::create(_pBackButton, _pAddLifeButton, _pAddMoneyButton, _pAddDiamondButton, NULL);
	m_pTopMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMenu->setPosition(0.0f, 0.0f);
	m_pTopMainMenu->addChild(m_pTopMenu, 2);
}

void MenuLayer::initBottomMainMenu() {
	m_pBottomMainLayer->removeAllChildrenWithCleanup(true);

	float _fXPositionCounter = 0.0f;

	// button quest
	auto _aQuestButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_quest.png",
		CC_CALLBACK_0(MenuLayer::buttonQuestHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aQuestButton->getContentSize().width * _aQuestButton->getScaleX();

	// quest attention
	m_pSpriteQuestAttention = SkeletonAnimation::createWithFile("UI/UI_main_menu/noti/noti.json", "UI/UI_main_menu/noti/noti.atlas",
		m_pBottomMainLayer->getContentSize().height / 120.0f);
	m_pSpriteQuestAttention->setAnchorPoint(Vec2(1.0f, 1.0f));
	m_pSpriteQuestAttention->setPosition(_fXPositionCounter - m_pBottomMainLayer->getContentSize().height * 0.1f,
		m_pBottomMainLayer->getContentSize().height * 0.7f);
	m_pBottomMainLayer->addChild(m_pSpriteQuestAttention, 2);
	m_pSpriteQuestAttention->setAnimation(0, "idle", true);
	m_pSpriteQuestAttention->setVisible(false);

	// button hero
	auto _aHeroButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_heroes.png",
		CC_CALLBACK_0(MenuLayer::buttonHeroesHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aHeroButton->getContentSize().width * _aHeroButton->getScaleX();

	// button shop
	auto _aShopButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_shop.png",
		CC_CALLBACK_0(MenuLayer::buttonShopHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aShopButton->getContentSize().width * _aShopButton->getScaleX();

	// button leaderboard
	auto _aLeaderBoardButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_leaderboard.png",
		CC_CALLBACK_0(MenuLayer::buttonLeaderBoardHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aLeaderBoardButton->getContentSize().width * _aLeaderBoardButton->getScaleX();

	// button free coin
	auto _aFreeCoinButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_free_coin.png",
		CC_CALLBACK_0(MenuLayer::buttonFreeCoinHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aFreeCoinButton->getContentSize().width * _aFreeCoinButton->getScaleX();

	// free coin attention
	m_pSpriteFreeCoinAttention = SkeletonAnimation::createWithFile("UI/UI_main_menu/noti/noti.json", "UI/UI_main_menu/noti/noti.atlas",
		m_pBottomMainLayer->getContentSize().height / 120.0f);
	m_pSpriteFreeCoinAttention->setAnchorPoint(Vec2(1.0f, 1.0f));
	m_pSpriteFreeCoinAttention->setPosition(_fXPositionCounter - m_pBottomMainLayer->getContentSize().height * 0.1f,
		m_pBottomMainLayer->getContentSize().height * 0.7f);
	m_pBottomMainLayer->addChild(m_pSpriteFreeCoinAttention, 2);
	m_pSpriteFreeCoinAttention->setAnimation(0, "idle", true);
	if (REF->getFreeCoin() <= 0) {
		m_pSpriteFreeCoinAttention->setVisible(false);
	}

	// button setting
	auto _aSettingButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_setting.png",
		CC_CALLBACK_0(MenuLayer::buttonSettingHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aSettingButton->getContentSize().width * _aSettingButton->getScaleX();

	// button more game
	auto _aMoreGameButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_more_games.png",
		CC_CALLBACK_0(MenuLayer::buttonMoreGameHandle, this), 0.1f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _aMoreGameButton->getContentSize().width * _aMoreGameButton->getScaleX() + m_pBottomMainLayer->getContentSize().width * 0.02f;

	// button start
	auto _aStartButton = createButtonOnParent(m_pBottomMainLayer, NULL, "UI/UI_main_menu/BottomMenu/btn_start.png",
		CC_CALLBACK_0(MenuLayer::buttonStartHandle, this), 0.26f, 0.8f, false, Vec2(0.0f, 0.0f),
		Vec2(_fXPositionCounter, m_pBottomMainLayer->getContentSize().height * 0.1f));

	m_fButtonStartPosition = _fXPositionCounter;

	m_pBottomMainMenu = Menu::create(_aQuestButton, _aHeroButton, _aShopButton, _aLeaderBoardButton, _aFreeCoinButton,
		_aSettingButton, _aMoreGameButton, _aStartButton, NULL);
	m_pBottomMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.27f)); // fill screen width, 27% screen height
	m_pBottomMainMenu->setPosition(0.0f, 0.0f);
	m_pBottomMainLayer->addChild(m_pBottomMainMenu, 1);
}

void MenuLayer::initItemBoard() {
	m_pItemBoard->removeAllChildrenWithCleanup(true);

	// board item
	Sprite *_pItemBoard = Sprite::create("UI/UI_main_menu/ItemBoard/board_item.png");
	float _fTemp = _pItemBoard->getContentSize().height * m_szVisibleSize.width / _pItemBoard->getContentSize().width * 0.5f;
	if (_fTemp > m_szVisibleSize.height * 0.65f) {
		_pItemBoard->setScale(m_szVisibleSize.height / _pItemBoard->getContentSize().height * 0.65f);
	}
	else {
		_pItemBoard->setScale(m_szVisibleSize.width / _pItemBoard->getContentSize().width * 0.5f);
	}
	_pItemBoard->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pItemBoard->setPosition(Vec2(m_pItemBoard->getContentSize().width * 0.5f, m_szVisibleSize.height * 0.575f));
	m_pItemBoard->addChild(_pItemBoard, 0);

	int _nNumberItems = 5;
	int _nNumberItemsVisiable = 3;
	// scroll view
	m_pItemScrollView = ScrollView::create();
	m_pItemScrollView->setContentSize(Size(_pItemBoard->getContentSize().width * 0.8f, _pItemBoard->getContentSize().height * 0.7f));
	m_pItemScrollView->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_pItemScrollView->setPosition(Vec2(_pItemBoard->getContentSize().width * 0.1f, _pItemBoard->getContentSize().height * 0.1f));
	m_pItemScrollView->setDirection(ScrollView::Direction::VERTICAL);
	m_pItemScrollView->setBounceEnabled(true);
	m_pItemScrollView->setTouchEnabled(true);
	m_pItemScrollView->setScrollBarOpacity(0.0f);
	m_pItemScrollView->setInnerContainerSize(Size(m_pItemScrollView->getContentSize().width,
		m_pItemScrollView->getContentSize().height * _nNumberItems / _nNumberItemsVisiable));
	_pItemBoard->addChild(m_pItemScrollView, 1);
	m_pItemScrollView->addEventListener(CC_CALLBACK_2(MenuLayer::scrollSlideHandle, this));

	// int _nItemIndexCounter = 0;
	float _fItemWidth = m_pItemScrollView->getContentSize().width;
	float _fItemHeight = m_pItemScrollView->getContentSize().height / _nNumberItemsVisiable;
	float _fScrollViewPadding = _fItemHeight / 20;

	for (int i = 0; i < _nNumberItems; i++) {
		Layer *_pItem = Layer::create();
		_pItem->setContentSize(Size(_fItemWidth, _fItemHeight));
		_pItem->setPosition(0.0f, _fItemHeight * (_nNumberItems - i - 1));
		m_pItemScrollView->addChild(_pItem, 1);
		JSMENU->readItem(m_nLanguage, i);

		Sprite *_pItemSprite = createSpriteOnParent(_pItem, NULL, 1, JSMENU->getItemIconFilePath(), 0.9f, 0.9f, false,
			Vec2(0.0f, 0.5f), Vec2(0.0f, _fItemHeight * 0.5f));
		Sprite *_pItemNumberSprite = createSpriteOnParent(NULL, _pItemSprite, 1, "UI/UI_main_menu/ItemBoard/red_circle.png",
			0.4f, 0.4f, false, Vec2(1.0f, 0.0f), Vec2(_pItemSprite->getContentSize().width, 0.0f));

		m_arLabelNumberItemOwning[i] = createLabelBMOnParent(NULL, _pItemNumberSprite, 1, "fontsDPM/font_life-export.fnt",
			StringUtils::format("%d", m_arNumberItemOwning[i]), 0.8f, TextHAlignment::CENTER, TextVAlignment::CENTER, Vec2(0.5f, 0.5f),
			Vec2(_pItemNumberSprite->getContentSize().width / 2, _pItemNumberSprite->getContentSize().height * 0.5f));

		string _sTempName = JSMENU->getItemName();
		Label *_pLabelNameItem = createLabelTTFOnParent(_pItem, NULL, 1, "fontsDPM/UTM_DK_Drop_Dead.ttf",
			StringUtils::format("%s", _sTempName.c_str()), 0.25f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 0.0f), Vec2(_fItemHeight * 0.95f, _fItemHeight * 0.7f), Color3B::BLACK);

		string _sTempDescription = JSMENU->getItemDescription();
		Label *_pLabelDescriptionItem = createLabelTTFOnParent(_pItem, NULL, 1, "fontsDPM/UTM_BRUSHSCI.ttf",
			StringUtils::format("%s", _sTempDescription.c_str()), 0.25f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 0.5f), Vec2(_fItemHeight * 0.95f, _fItemHeight * 0.3f), Color3B::BLACK);
		_pLabelDescriptionItem->setMaxLineWidth((_pItem->getContentSize().width - _fItemHeight * 2.2f) / _pLabelDescriptionItem->getScale());

		m_arItemCoinSprite[i] = createSpriteOnParent(_pItem, NULL, 1, "UI/UI_main_menu/ItemBoard/icon_money_small.png",
			0.4f, 0.4f, false, Vec2(1.0f, 0.5f), Vec2(_fItemWidth, _fItemHeight * 0.75f));

		m_arItemPrice[i] = JSMENU->getItemPrice();
		m_arItemLabelCost[i] = createLabelBMOnParent(_pItem, NULL, 1, "fontsDPM/font_coin-export.fnt",
			StringUtils::format("%i", m_arItemPrice[i]), _fItemHeight * 0.4f, TextHAlignment::RIGHT, TextVAlignment::CENTER,
			Vec2(1.0f, 0.5f), Vec2(_fItemWidth - _fItemHeight * 0.4f, _fItemHeight * 0.75f));
		m_arItemLabelCost[i]->setBMFontSize(_fItemHeight * 0.4f);

		m_arBuyItemButton[i] = Button::create("UI/UI_main_menu/ItemBoard/btn_buy_1.png", "UI/UI_main_menu/ItemBoard/btn_buy_3.png", "UI/UI_main_menu/ItemBoard/btn_buy_3.png");
		float _fTemp = m_arBuyItemButton[i]->getContentSize().height * _fItemHeight / m_arBuyItemButton[i]->getContentSize().width;
		if (_fTemp > _fItemHeight * 0.45f) {
			m_arBuyItemButton[i]->setScale(_fItemHeight / m_arBuyItemButton[i]->getContentSize().height * 0.45f);
		}
		else {
			m_arBuyItemButton[i]->setScale(_fItemHeight / m_arBuyItemButton[i]->getContentSize().width);
		}
		m_arBuyItemButton[i]->setAnchorPoint(Vec2(0.5, 0.5));
		m_arBuyItemButton[i]->setTouchEnabled(true);
		m_arBuyItemButton[i]->addTouchEventListener(CC_CALLBACK_2(MenuLayer::receiveButtonPressEvent, this, i));
		m_arBuyItemButton[i]->setPosition(Vec2(_fItemWidth - (_fItemHeight * 0.4f + m_arItemLabelCost[i]->getContentSize().width * m_arItemLabelCost[i]->getScale()) / 2, _fItemHeight * (_nNumberItems - i - 0.7f)));
		m_arBuyItemButton[i]->setSwallowTouches(true);
		m_pItemScrollView->addChild(m_arBuyItemButton[i]);
		ScaleBy *_pZoomOut = ScaleBy::create(0.5f, 1.1f);
		Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
		RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
		m_arBuyItemButton[i]->runAction(_pZoomRepeat);

		m_arSpriteItemMax[i] = createSpriteOnParent(_pItem, NULL, 1, "UI/UI_main_menu/ItemBoard/icon_max.png", 1.0f, 1.0f, false,
			Vec2(1.0f, 1.0f), Vec2(_fItemWidth, _fItemHeight));
		m_arSpriteItemMax[i]->setScale(_fItemHeight / m_arSpriteItemMax[i]->getContentSize().height * 0.9f);

		if (m_arNumberItemOwning[i] <= 0) {
			_pItemNumberSprite->setVisible(false);
			m_arSpriteItemMax[i]->setVisible(false);
		}
		else {
			m_arBuyItemButton[i]->setVisible(false);
			m_arItemCoinSprite[i]->setVisible(false);
			m_arItemLabelCost[i]->setVisible(false);
		}

		if (i != _nNumberItems - 1) { // line between two items
			Sprite *_pItemLine = createSpriteOnParent(_pItem, NULL, 1, "UI/UI_main_menu/line.png", 1.0f, 1.0f, true,
				Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f));
			_pItemLine->setScale(_fItemWidth / _pItemLine->getContentSize().width);
		}
	}
}

void MenuLayer::initUpgradeBoard() {
	m_pUpgradeBoard->removeAllChildrenWithCleanup(true);
	REF->pointToCurrentHero(m_nIndexHeroPicked);

	// board upgrate
	Sprite *_pBoardUpgrade = Sprite::create("UI/UI_main_menu/UpgradeBoard/board_skill_upgrade.png");
	float _fTemp = _pBoardUpgrade->getContentSize().height * m_szVisibleSize.width / _pBoardUpgrade->getContentSize().width * 0.4f;
	if (_fTemp > m_szVisibleSize.height * 0.65f) {
		_pBoardUpgrade->setScale(m_szVisibleSize.height / _pBoardUpgrade->getContentSize().height * 0.65f);
	}
	else {
		_pBoardUpgrade->setScale(m_szVisibleSize.width / _pBoardUpgrade->getContentSize().width * 0.4f);
	}
	_pBoardUpgrade->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pBoardUpgrade->setPosition(Vec2(m_pUpgradeBoard->getContentSize().width * 0.5f, m_szVisibleSize.height * 0.575f));
	m_pUpgradeBoard->addChild(_pBoardUpgrade, 0);

	JSHERO->readFile(m_nLanguage, m_nIndexHeroPicked);
	string _arSkillSpritePath[3] = { JSHERO->getPathMainImageSkill1(), JSHERO->getPathMainImageSkill2(), JSHERO->getPathMainImageSkill3() };
	string _arSkillName[3] = { JSHERO->getNameOfSkill_1(), JSHERO->getNameOfSkill_2(), JSHERO->getNameOfSkill_3() };
	MenuItemSprite *_arUpgrateSkill[3];
	float _arDuration[3] = { REF->getDurationSkill_1(), REF->getDurationSkill_2(), REF->getDurationSkill_3() };
	float _arCoolDown[3] = { REF->getCoolDownSkill_1(), REF->getCoolDownSkill_2(), REF->getCoolDownSkill_3() };
	int _arNumberUse[3] = { REF->getNumberUseSkill_1(), REF->getNumberUseSkill_2(), REF->getNumberUseSkill_3() };

	int _arSkillLevel[3] = { REF->getLevelSkill_1(), REF->getLevelSkill_2(), REF->getLevelSkill_3() };
	for (int i = 0; i < 3; i++) {
		Layer *_pSkillInfo = Layer::create();
		_pSkillInfo->setContentSize(Size(_pBoardUpgrade->getContentSize().width * 0.8f, _pBoardUpgrade->getContentSize().height * 0.23f));
		_pSkillInfo->setPosition(_pBoardUpgrade->getContentSize().width * 0.1f, _pBoardUpgrade->getContentSize().height * (0.57f - i * 0.23f));
		_pBoardUpgrade->addChild(_pSkillInfo, 1);

		Sprite *_pSkillSprite = Sprite::create(_arSkillSpritePath[i]);
		_pSkillSprite->setScale(_pSkillInfo->getContentSize().height / _pSkillSprite->getContentSize().height * 0.8f);
		_pSkillSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pSkillSprite->setPosition(0.0f, _pSkillInfo->getContentSize().height / 2);
		_pSkillInfo->addChild(_pSkillSprite, 0);
		
		Label *_pSkillNameLabel = createLabelTTFOnParent(_pSkillInfo, NULL, 1, "fontsDPM/UTM_DK_Drop_Dead.ttf",
			StringUtils::format("%s", _arSkillName[i].c_str()), 0.15f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 1.0f), Vec2(_pSkillInfo->getContentSize().height * 0.85f, _pSkillInfo->getContentSize().height * 0.95f), Color3B::BLACK);

		Label *_pLevelLabel = createLabelTTFOnParent(_pSkillInfo, NULL, 1, "fontsDPM/UTM_BRUSHSCI.ttf",
			JSMENU->readMenuText(m_nLanguage, 4).c_str(), 0.25f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 1.0f), Vec2(_pSkillInfo->getContentSize().height * 0.85f, _pSkillInfo->getContentSize().height * 0.7f), Color3B::BLACK);

		Sprite *_pSkillLevelBar = Sprite::create("UI/UI_main_menu/UpgradeBoard/level_skill_bar.png");
		_pSkillLevelBar->setScale(_pSkillInfo->getContentSize().width / _pSkillLevelBar->getContentSize().width * 0.25f / _pLevelLabel->getScale());
		_pSkillLevelBar->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pSkillLevelBar->setPosition(Vec2(_pLevelLabel->getContentSize().width, _pLevelLabel->getContentSize().height * 0.6f));
		_pLevelLabel->addChild(_pSkillLevelBar, 1);
		for (int j = 0; j < _arSkillLevel[i]; j++) {
			Sprite *_pSkillLevelPoint = createSpriteOnParent(NULL, _pSkillLevelBar, 1, "UI/UI_main_menu/UpgradeBoard/level_skill_point.png",
				1.0f, 1.0f, false, Vec2(0.0f, 0.0f), Vec2(_pSkillLevelBar->getContentSize().width * 0.1 * j, 0.0f));
		}

		Label *_pDurationLabel = createLabelTTFOnParent(_pSkillInfo, NULL, 1, "fontsDPM/UTM_BRUSHSCI.ttf",
			JSMENU->readMenuText(m_nLanguage, 5).c_str(), 0.25f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 1.0f), Vec2(_pSkillInfo->getContentSize().height * 0.85f, _pSkillInfo->getContentSize().height * 0.45f),
			Color3B::BLACK);
		if (_arNumberUse[i] > 1) _pDurationLabel->setString(JSMENU->readMenuText(m_nLanguage, 6).c_str());

		Label *_pDurationValueLabel = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%i", (int)_arDuration[i]));
		if (_arNumberUse[i] > 1) _pDurationValueLabel->setString(StringUtils::format("%i", _arNumberUse[i]));
		_pDurationValueLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f / _pLevelLabel->getScale());
		_pDurationValueLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
		_pDurationValueLabel->setPosition(Vec2(_pDurationLabel->getContentSize().width, _pDurationLabel->getContentSize().height));
		_pDurationLabel->addChild(_pDurationValueLabel, 1);

		Label *_pCoolDownLabel = createLabelTTFOnParent(_pSkillInfo, NULL, 1, "fontsDPM/UTM_BRUSHSCI.ttf",
			JSMENU->readMenuText(m_nLanguage, 7).c_str(), 0.25f, false, TextHAlignment::LEFT, TextVAlignment::CENTER,
			Vec2(0.0f, 1.0f), Vec2(_pSkillInfo->getContentSize().height * 0.85f, _pSkillInfo->getContentSize().height * 0.2f),
			Color3B::BLACK);

		Label *_pCoolDownValueLabel = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%i", (int)_arCoolDown[i]));
		_pCoolDownValueLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f / _pLevelLabel->getScale());
		_pCoolDownValueLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
		_pCoolDownValueLabel->setPosition(Vec2(_pCoolDownLabel->getContentSize().width, _pCoolDownLabel->getContentSize().height));
		_pCoolDownLabel->addChild(_pCoolDownValueLabel, 1);

		int _nCost = 300 + _arSkillLevel[i] * 200;
		
		_arUpgrateSkill[i] = createButtonOnParent(NULL, _pBoardUpgrade, "UI/UI_main_menu/UpgradeBoard/btn_upgrade_1.png",
			CC_CALLBACK_0(MenuLayer::buttonUpgradeSkillHandle, this, i, _nCost), 0.2f, 0.2f, true, Vec2(0.5f, 0.0f),
			Vec2(_pBoardUpgrade->getContentSize().width * 0.77f, _pBoardUpgrade->getContentSize().height * (0.6f - i * 0.23f)));
		if (REF->getIsLockedHero()) {
			_arUpgrateSkill[i]->setEnabled(false);
			_arUpgrateSkill[i]->setNormalImage(Sprite::create("UI/UI_main_menu/UpgradeBoard/btn_upgrade_2.png"));
		}
		else {
			buttonSpringy(_arUpgrateSkill[i]);
		}

		Label *_pLabelCost = createLabelBMOnParent(NULL, _pBoardUpgrade, 1, "fontsDPM/font_coin-export.fnt",
			StringUtils::format("%i", _nCost), 0.35f, TextHAlignment::LEFT, TextVAlignment::CENTER, Vec2(0.0f, 0.0f),
			Vec2(_arUpgrateSkill[i]->getPosition().x, _arUpgrateSkill[i]->getPosition().y + _arUpgrateSkill[i]->getContentSize().height));
		_pLabelCost->setBMFontSize(_pSkillInfo->getContentSize().height * 0.35f);

		Sprite *_CoinSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
		_CoinSprite->setScale(_pLabelCost->getContentSize().height / _CoinSprite->getContentSize().height);
		_CoinSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
		_CoinSprite->setPosition(Vec2(_pLabelCost->getContentSize().width * 1.1f, _pLabelCost->getContentSize().height * 0.5f));
		_pLabelCost->addChild(_CoinSprite, 1);
		_pLabelCost->setPosition(Vec2(_arUpgrateSkill[i]->getPosition().x - (_pLabelCost->getContentSize().width * 1.1f + _CoinSprite->getContentSize().width * _CoinSprite->getScaleX()) * 0.5f,
			_pLabelCost->getPosition().y));

		if (_arSkillLevel[i] >= 10) {
			_CoinSprite->setVisible(false);
			_pLabelCost->setVisible(false);
			_arUpgrateSkill[i]->setVisible(false);

			Sprite *_pMaxUpgrate = createSpriteOnParent(_pSkillInfo, NULL, 1, "UI/UI_main_menu/ItemBoard/icon_max.png",
				0.9f, 0.9f, false, Vec2(0.5f, 0.5f), Vec2(_pSkillInfo->getContentSize().width * 0.85f, _pSkillInfo->getContentSize().height * 0.5f));
		}

		if (i < 2) {
			Sprite *_pItemLine = createSpriteOnParent(_pSkillInfo, NULL, 1, "UI/UI_main_menu/line.png", 1.0f, 1.0f, true,
				Vec2(0.5f, 0.5f), Vec2(_pSkillInfo->getContentSize().width * 0.5f, 0.0f));
		}
	}

	m_pSkillBoardMenu = Menu::create(_arUpgrateSkill[0], _arUpgrateSkill[1], _arUpgrateSkill[2], NULL);
	m_pSkillBoardMenu->setContentSize(_pBoardUpgrade->getContentSize());
	m_pSkillBoardMenu->setPosition(0.0f, 0.0f);
	_pBoardUpgrade->addChild(m_pSkillBoardMenu, 2);
}

void MenuLayer::initQuestBoard(int p_nFocus) {
	m_pQuestBoard->removeAllChildrenWithCleanup(true);

	// board quest
	Sprite *_pQuestBoardBackground = Sprite::create("UI/UI_main_menu/QuestBoard/board_quest.png");
	float _fTemp = _pQuestBoardBackground->getContentSize().height * m_szVisibleSize.width / _pQuestBoardBackground->getContentSize().width * 0.7f;
	if (_fTemp > m_szVisibleSize.height * 0.8f) {
		_pQuestBoardBackground->setScale(m_szVisibleSize.height / _pQuestBoardBackground->getContentSize().height * 0.8f);
	}
	else {
		_pQuestBoardBackground->setScale(m_szVisibleSize.width / _pQuestBoardBackground->getContentSize().width * 0.7f);
	}
	_pQuestBoardBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pQuestBoardBackground->setPosition(Vec2(m_pQuestBoard->getContentSize().width * 0.5f, m_szVisibleSize.height * 0.45f));
	m_pQuestBoard->addChild(_pQuestBoardBackground, 0);

	int _nNumberQuests = JSQUEST->getNumberQuest();
	int _nNumberQuestsVisiable = 4;
	ScrollView *_pQuestBoardZone = ScrollView::create();
	_pQuestBoardZone->setContentSize(Size(_pQuestBoardBackground->getContentSize().width * 0.85f, _pQuestBoardBackground->getContentSize().height * 0.75f));
	_pQuestBoardZone->setAnchorPoint(Vec2(0.5f, 1.0f));
	_pQuestBoardZone->setPosition(Vec2(_pQuestBoardBackground->getContentSize().width * 0.5f, _pQuestBoardBackground->getContentSize().height * 0.85f));
	_pQuestBoardZone->setDirection(ScrollView::Direction::VERTICAL);
	_pQuestBoardZone->setBounceEnabled(true);
	_pQuestBoardZone->setTouchEnabled(true);
	_pQuestBoardZone->setScrollBarOpacity(0.0f);
	_pQuestBoardZone->setInnerContainerSize(Size(_pQuestBoardZone->getContentSize().width,
		_pQuestBoardZone->getContentSize().height * _nNumberQuests / _nNumberQuestsVisiable));
	_pQuestBoardBackground->addChild(_pQuestBoardZone, 1);
	_pQuestBoardZone->addEventListener(CC_CALLBACK_2(MenuLayer::scrollSlideHandle, this));

	float _fItemWidth = _pQuestBoardZone->getContentSize().width;
	float _fItemHeight = _pQuestBoardZone->getContentSize().height / _nNumberQuestsVisiable;
	float _fScrollViewPadding = _fItemHeight / 20;

	m_pQuestBoardMenu = Menu::create();
	m_pQuestBoardMenu->setContentSize(_pQuestBoardZone->getContentSize());
	m_pQuestBoardMenu->setPosition(0.0f, 0.0f);
	_pQuestBoardZone->addChild(m_pQuestBoardMenu, 2);

	m_pSpriteQuestAttention->setVisible(false);

	for (int i = 0; i < _nNumberQuests; i++) {
		Layer *_pQuestLayer = Layer::create();
		_pQuestLayer->setContentSize(Size(_fItemWidth, _fItemHeight));
		_pQuestLayer->setPosition(0.0f, _fItemHeight * (_nNumberQuests - i - 1));
		_pQuestBoardZone->addChild(_pQuestLayer, 1);
		JSQUEST->readQuest(m_nLanguage, i);
		REF->readDataQuest(i);

		Label *_pLabelQuestName = Label::createWithTTF(StringUtils::format("%s", JSQUEST->getQuestName().c_str()),
			"fontsDPM/UTM_DK_Drop_Dead.ttf", _fItemHeight * 0.25f);
		_pLabelQuestName->setColor(Color3B::BLACK);
		_pLabelQuestName->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
		_pLabelQuestName->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pLabelQuestName->setPosition(Vec2(_fItemWidth * 0.0f, _fItemHeight * 0.7f));
		_pQuestLayer->addChild(_pLabelQuestName, 1);

		Label *_pQuestDescription = Label::createWithTTF(StringUtils::format("%s", JSQUEST->getQuestDescription().c_str()),
			"fontsDPM/UTM_BRUSHSCI.ttf", _fItemHeight * 0.25f);
		_pQuestDescription->setColor(Color3B::BLACK);
		_pQuestDescription->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
		//_pQuestDescription->setLineHeight(_fItemHeight * 0.35f);
		_pQuestDescription->setLineSpacing(0.0f);
		_pQuestDescription->setMaxLineWidth(_fItemWidth * 0.6f);
		_pQuestDescription->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pQuestDescription->setPosition(Vec2(0.0f, _fItemHeight * 0.3f));
		_pQuestLayer->addChild(_pQuestDescription, 1);

		if (REF->getRewardedQuestTimes() >= JSQUEST->getLimitRequest()) {
			Sprite *_pQuestComplete = createSpriteOnParent(_pQuestLayer, NULL, 1, "UI/UI_main_menu/QuestBoard/icon_completed.png",
				0.8f, 0.8f, false, Vec2(0.5f, 0.5f), Vec2(_pQuestLayer->getContentSize().width * 0.8125f, _pQuestLayer->getContentSize().height * 0.5f));
		}
		else {
			int _nNumber = REF->getNumberQuest();
			int _nComplete = JSQUEST->getCompleteRequest();
			int _nGoldReward = JSQUEST->getGoldReward();
			for (int j = 0; j < REF->getRewardedQuestTimes(); j++) {
				_nComplete *= JSQUEST->getStepRequest();
				_nGoldReward *= JSQUEST->getStepRequest();
			}
			Label *_pLabelGoldReward = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d ", _nGoldReward));
			_pLabelGoldReward->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
			_pLabelGoldReward->setBMFontSize(_pQuestLayer->getContentSize().height * 0.35f);
			_pLabelGoldReward->setAnchorPoint(Vec2(1.0f, 0.5f));
			_pQuestLayer->addChild(_pLabelGoldReward, 1);
			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
			_pCoinSprite->setScale(_pQuestLayer->getContentSize().height / _pCoinSprite->getContentSize().height * 0.4f);
			_pCoinSprite->setAnchorPoint(Vec2(1.0f, 0.5f));
			_pQuestLayer->addChild(_pCoinSprite, 1);
			_pCoinSprite->setPosition(Vec2(_pQuestLayer->getContentSize().width * 0.8125f + (_pLabelGoldReward->getContentSize().width + _pCoinSprite->getContentSize().width) * 0.5f,
				_pQuestLayer->getContentSize().height * 0.8f));
			_pLabelGoldReward->setPosition(Vec2(_pCoinSprite->getPosition().x - _pCoinSprite->getContentSize().width * _pCoinSprite->getScaleX(),
				_pQuestLayer->getContentSize().height * 0.8f));

			float _fScaleX = (float)(_nNumber < _nComplete ? _nNumber : _nComplete) / (float)_nComplete;
			if (_fScaleX < 1.0f) {
				Sprite *_pQuestBar = Sprite::create("UI/UI_main_menu/QuestBoard/quest_progress_bar.png");
				_pQuestBar->setScaleX(_pQuestLayer->getContentSize().width / _pQuestBar->getContentSize().width * 0.35f);
				_pQuestBar->setScaleY(_pQuestLayer->getContentSize().height / _pQuestBar->getContentSize().height * 0.3f);
				_pQuestBar->setAnchorPoint(Vec2(0.5f, 1.0f));
				_pQuestBar->setPosition(Vec2(_pQuestLayer->getContentSize().width * 0.8125f, _pQuestLayer->getContentSize().height * 0.5f));
				_pQuestLayer->addChild(_pQuestBar, 1);

				Label *_pLabelQuestStatus = Label::createWithBMFont("fontsDPM/font_normal_white-export.fnt", StringUtils::format("%d / %d", _nNumber < _nComplete ? _nNumber : _nComplete, _nComplete));
				_pLabelQuestStatus->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
				_pLabelQuestStatus->setBMFontSize(_pQuestBar->getContentSize().height * 0.8f);
				_pLabelQuestStatus->setAnchorPoint(Vec2(0.5f, 0.5f));
				_pLabelQuestStatus->setPosition(Vec2(_pQuestBar->getContentSize().width * 0.5f, _pQuestBar->getContentSize().height * 0.5f));
				_pQuestBar->addChild(_pLabelQuestStatus, 2);

				Sprite *_pQuestPoint = Sprite::create("UI/UI_main_menu/QuestBoard/quest_progress_point.png");
				_pQuestPoint->setScaleX(_pQuestBar->getContentSize().width / _pQuestPoint->getContentSize().width);
				_pQuestPoint->setScaleY(_pQuestBar->getContentSize().height / _pQuestPoint->getContentSize().height);
				_pQuestPoint->setAnchorPoint(Vec2(0.0f, 0.0f));
				_pQuestPoint->setPosition(Vec2(0.0f, 0.0f));
				_pQuestBar->addChild(_pQuestPoint, 1);
				_pQuestPoint->setSpriteFrame(SpriteFrame::create("UI/UI_main_menu/QuestBoard/quest_progress_point.png", Rect(0.0f, 0.0f,
					_pQuestPoint->getContentSize().width * _fScaleX, _pQuestPoint->getContentSize().height)));
			}
			else {
				m_pSpriteQuestAttention->setVisible(true);

				auto _pRewardNormal = Sprite::create("UI/UI_main_menu/QuestBoard/btn_claim_1.png");
				auto _pRewardSelected = Sprite::create("UI/UI_main_menu/QuestBoard/btn_claim_2.png");
				MenuItemSprite *_aQuestRewardButton = MenuItemSprite::create(_pRewardNormal, _pRewardSelected, CC_CALLBACK_0(MenuLayer::buttonRewardQuest, this, i));
				_aQuestRewardButton->setScale(_fItemHeight / _aQuestRewardButton->getContentSize().height * 0.4f);
				_aQuestRewardButton->setAnchorPoint(Vec2(0.5f, 1.0f));
				_aQuestRewardButton->setPosition(Vec2(_fItemWidth * 0.8125f, _fItemHeight * (_nNumberQuests - i - 0.5f)));

				m_pQuestBoardMenu->addChild(_aQuestRewardButton, 1);

				buttonSpringy(_aQuestRewardButton);

				float _fPercent = i * 100 / (_nNumberQuests - _nNumberQuestsVisiable);
				_pQuestBoardZone->scrollToPercentVertical(_fPercent > 100.0f ? 100.0f : _fPercent, 0.0f, false);
			}
		}

		if (i != _nNumberQuests - 1) { // in
			Sprite *_ItemLine = Sprite::create("UI/UI_main_menu/line.png");
			_ItemLine->setScale(_fItemWidth / _ItemLine->getContentSize().width);
			_ItemLine->setAnchorPoint(Vec2(0.0f, 0.0f));
			_ItemLine->setPosition(Vec2(0.0f, 0.0f));
			_pQuestLayer->addChild(_ItemLine, 1);
		}
	}
	if (!m_pSpriteQuestAttention->isVisible()) {
		float _fPercent = p_nFocus * 100 / (_nNumberQuests - _nNumberQuestsVisiable);
		_pQuestBoardZone->scrollToPercentVertical(_fPercent > 100.0f ? 100.0f : _fPercent, 0.0f, false);
	}
}

void MenuLayer::initHeroInfoBoard() {
	m_pHeroInfoBoard->removeAllChildrenWithCleanup(true);

	// board upgrate
	Sprite *_pInfoBoard = Sprite::create("UI/UI_main_menu/InfoBoard/board_information.png");
	float _fTemp = _pInfoBoard->getContentSize().height * m_szVisibleSize.width / _pInfoBoard->getContentSize().width * 0.35f;
	if (_fTemp > m_szVisibleSize.height * 0.65f) {
		_pInfoBoard->setScale(m_szVisibleSize.height / _pInfoBoard->getContentSize().height * 0.65f);
	}
	else {
		_pInfoBoard->setScale(m_szVisibleSize.width / _pInfoBoard->getContentSize().width * 0.35f);
	}
	_pInfoBoard->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pInfoBoard->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_szVisibleSize.height * 0.57f));
	m_pHeroInfoBoard->addChild(_pInfoBoard, 0);

	// character name
	string _sTempName = JSHERO->getName();
	Label *_pLabelNameHero = Label::createWithTTF(StringUtils::format("%s", _sTempName.c_str()), "fontsDPM/UTM_DK_Drop_Dead.ttf",
		_pInfoBoard->getContentSize().height * 0.07f);
	_pLabelNameHero->setColor(Color3B::BLACK);
	_pLabelNameHero->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
	_pLabelNameHero->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pLabelNameHero->setPosition(Vec2(_pInfoBoard->getContentSize().width * 0.5f, _pInfoBoard->getContentSize().height * 0.8f));
	_pInfoBoard->addChild(_pLabelNameHero, 1);

	string _sHeroInfo = JSHERO->getInfor();
	Label *_pHeroDescription = Label::createWithTTF(StringUtils::format("%s", _sHeroInfo.c_str()), "fontsDPM/UTM_BRUSHSCI.ttf",
		_pInfoBoard->getContentSize().height * 0.06f);
	_pHeroDescription->setColor(Color3B::BLACK);
	_pHeroDescription->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	//_pHeroDescription->setLineHeight(_pInfoBoard->getContentSize().height * 0.1f);
	_pHeroDescription->setLineSpacing(0.0f);
	_pHeroDescription->setMaxLineWidth(_pInfoBoard->getContentSize().width * 0.7f);
	_pHeroDescription->setAnchorPoint(Vec2(0.0f, 1.0f));
	_pHeroDescription->setPosition(Vec2(_pInfoBoard->getContentSize().width * 0.125f, _pInfoBoard->getContentSize().height * 0.77f));
	_pInfoBoard->addChild(_pHeroDescription, 1);

	string _sHeroIntrinsic = JSHERO->getIntrinsic();
	Label *_pHeroIntrinsic = Label::createWithTTF(StringUtils::format("%s", _sHeroIntrinsic.c_str()), "fontsDPM/UTM_BRUSHSCI.ttf",
		_pInfoBoard->getContentSize().height * 0.06f);
	_pHeroIntrinsic->setColor(Color3B(148, 0, 0));
	_pHeroIntrinsic->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	//_pHeroIntrinsic->setLineHeight(_pInfoBoard->getContentSize().height * 0.1f);
	_pHeroIntrinsic->setLineSpacing(0.0f);
	_pHeroIntrinsic->setMaxLineWidth(_pInfoBoard->getContentSize().width * 0.75f);
	_pHeroIntrinsic->setAnchorPoint(Vec2(0.0f, 1.0f));
	_pHeroIntrinsic->setPosition(Vec2(0.0f, 0.0f));
	_pHeroDescription->addChild(_pHeroIntrinsic, 1);

	Sprite *_pLine = Sprite::create("UI/UI_main_menu/line.png");
	_pLine->setScale(_pInfoBoard->getContentSize().height / _pLine->getContentSize().width * 0.4f);
	_pLine->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pLine->setRotation(90.0f);
	_pLine->setPosition(_pInfoBoard->getContentSize().width * 0.5f, _pInfoBoard->getContentSize().height * 0.5f);
	_pInfoBoard->addChild(_pLine, 1);

	// character current level label
	int _nCurrentLevel = REF->getCurrentLevel();

	Sprite *_pCurrentLevelSprite = Sprite::create("UI/UI_main_menu/InfoBoard/text_current.png");
	_pCurrentLevelSprite->setScale(_pInfoBoard->getContentSize().width / _pCurrentLevelSprite->getContentSize().width * 0.23f);
	_pCurrentLevelSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentLevelSprite->setPosition(_pInfoBoard->getContentSize().width * 0.1f, _pInfoBoard->getContentSize().height * 0.4f);
	_pInfoBoard->addChild(_pCurrentLevelSprite, 0);
	Label *_pLabelCurrentLevel = Label::createWithTTF(StringUtils::format((JSMENU->readMenuText(m_nLanguage, 0) + "%d").c_str(), _nCurrentLevel),
		"fontsDPM/UTM_DK_Drop_Dead.ttf", _pCurrentLevelSprite->getContentSize().height * 0.5f);
	_pLabelCurrentLevel->setColor(Color3B::BLACK);
	_pLabelCurrentLevel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_pLabelCurrentLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pLabelCurrentLevel->setPosition(Vec2(_pCurrentLevelSprite->getContentSize().width, _pCurrentLevelSprite->getContentSize().height * 0.5f));
	_pCurrentLevelSprite->addChild(_pLabelCurrentLevel, 1);

	int _nHealthPoint = REF->getCurrentHealth();
	int _nBonusScore = REF->getBonusScore();
	int _nBonusGold = REF->getBonusGold();

	Sprite *_pCurrentHPIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_hp.png");
	_pCurrentHPIcon->setScale(_pInfoBoard->getContentSize().height / _pCurrentHPIcon->getContentSize().height * 0.07f);
	_pCurrentHPIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentHPIcon->setPosition(_pInfoBoard->getContentSize().width * 0.13f, _pInfoBoard->getContentSize().height * 0.3f);
	_pInfoBoard->addChild(_pCurrentHPIcon, 1);
	Label *_pCurrentHPLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 1).c_str()),
		"fontsDPM/UTM_DK_Drop_Dead.ttf", _pCurrentHPIcon->getContentSize().height * 0.5f);
	_pCurrentHPLabel->setColor(Color3B::BLACK);
	_pCurrentHPLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pCurrentHPLabel->setPosition(Vec2(_pCurrentHPIcon->getContentSize().width, _pCurrentHPIcon->getContentSize().height * 0.5f));
	_pCurrentHPIcon->addChild(_pCurrentHPLabel, 1);
	Label *_pCurrentHPValueLabel = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%d", _nHealthPoint));
	_pCurrentHPValueLabel->setBMFontSize(_pCurrentHPIcon->getContentSize().height);
	_pCurrentHPValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentHPValueLabel->setPosition(Vec2(_pCurrentHPLabel->getContentSize().width, 0.0f));
	_pCurrentHPLabel->addChild(_pCurrentHPValueLabel, 1);

	Sprite *_pCurrentScoreIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_score.png");
	_pCurrentScoreIcon->setScale(_pInfoBoard->getContentSize().height / _pCurrentScoreIcon->getContentSize().height * 0.07f);
	_pCurrentScoreIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentScoreIcon->setPosition(_pInfoBoard->getContentSize().width * 0.13f, _pInfoBoard->getContentSize().height * 0.225f);
	_pInfoBoard->addChild(_pCurrentScoreIcon, 1);
	Label *_pCurrentScoreLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 2).c_str()),
		"fontsDPM/UTM_DK_Drop_Dead.ttf", _pCurrentScoreIcon->getContentSize().height * 0.5f);
	_pCurrentScoreLabel->setColor(Color3B::BLACK);
	_pCurrentScoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pCurrentScoreLabel->setPosition(Vec2(_pCurrentScoreIcon->getContentSize().width, _pCurrentScoreIcon->getContentSize().height * 0.5f));
	_pCurrentScoreIcon->addChild(_pCurrentScoreLabel, 1);
	Label *_pCurrentScoreValueLabel = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%d%%", _nBonusScore));
	_pCurrentScoreValueLabel->setBMFontSize(_pCurrentScoreIcon->getContentSize().height * 0.7f);
	_pCurrentScoreValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentScoreValueLabel->setPosition(Vec2(_pCurrentScoreLabel->getContentSize().width, 0.0f));
	_pCurrentScoreLabel->addChild(_pCurrentScoreValueLabel, 1);

	Sprite *_pCurrentGoldIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_coin.png");
	_pCurrentGoldIcon->setScale(_pInfoBoard->getContentSize().height / _pCurrentGoldIcon->getContentSize().height * 0.07f);
	_pCurrentGoldIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentGoldIcon->setPosition(_pInfoBoard->getContentSize().width * 0.13f, _pInfoBoard->getContentSize().height * 0.15f);
	_pInfoBoard->addChild(_pCurrentGoldIcon, 1);
	Label *_pCurrentGoldLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 3).c_str()),
		"fontsDPM/UTM_DK_Drop_Dead.ttf", _pCurrentGoldIcon->getContentSize().height * 0.5f);
	_pCurrentGoldLabel->setColor(Color3B::BLACK);
	_pCurrentGoldLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pCurrentGoldLabel->setPosition(Vec2(_pCurrentGoldIcon->getContentSize().width, _pCurrentGoldIcon->getContentSize().height * 0.5f));
	_pCurrentGoldIcon->addChild(_pCurrentGoldLabel, 1);
	Label *_pCurrentGoldValueLabel = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d%%", _nBonusGold));
	_pCurrentGoldValueLabel->setBMFontSize(_pCurrentGoldIcon->getContentSize().height * 0.7f);
	_pCurrentGoldValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pCurrentGoldValueLabel->setPosition(Vec2(_pCurrentGoldLabel->getContentSize().width, 0.0f));
	_pCurrentGoldLabel->addChild(_pCurrentGoldValueLabel, 1);

	if (_nCurrentLevel < JSHERO->getMaxLevel()) {
		// character next level label
		Sprite *_pNextLevelSprite = Sprite::create("UI/UI_main_menu/InfoBoard/text_next.png");
		_pNextLevelSprite->setScale(_pCurrentLevelSprite->getScale());
		_pNextLevelSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextLevelSprite->setPosition(_pInfoBoard->getContentSize().width * 0.55f, _pInfoBoard->getContentSize().height * 0.4f);
		_pInfoBoard->addChild(_pNextLevelSprite, 0);
		Label *_pLabelNextLevel = Label::createWithTTF(StringUtils::format((JSMENU->readMenuText(m_nLanguage, 0) + "%d").c_str(), _nCurrentLevel + 1),
			"fontsDPM/UTM_DK_Drop_Dead.ttf", _pNextLevelSprite->getContentSize().height * 0.5f);
		_pLabelNextLevel->setColor(Color3B::BLACK);
		_pLabelNextLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pLabelNextLevel->setPosition(Vec2(_pNextLevelSprite->getContentSize().width, _pNextLevelSprite->getContentSize().height * 0.5f));
		_pNextLevelSprite->addChild(_pLabelNextLevel, 1);

		if ((_nCurrentLevel + 1) % 10 == 0) {
			_nHealthPoint++;
		}
		_nBonusScore++;
		_nBonusGold++;

		Sprite *_pNextHPIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_hp.png");
		_pNextHPIcon->setScale(_pInfoBoard->getContentSize().height / _pNextHPIcon->getContentSize().height * 0.07f);
		_pNextHPIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextHPIcon->setPosition(_pInfoBoard->getContentSize().width * 0.53f, _pInfoBoard->getContentSize().height * 0.3f);
		_pInfoBoard->addChild(_pNextHPIcon, 1);
		Label *_pNextHPLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 1).c_str()),
			"fontsDPM/UTM_DK_Drop_Dead.ttf", _pNextHPIcon->getContentSize().height * 0.5f);
		_pNextHPLabel->setColor(Color3B::BLACK);
		_pNextHPLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pNextHPLabel->setPosition(Vec2(_pNextHPIcon->getContentSize().width, _pNextHPIcon->getContentSize().height * 0.5f));
		_pNextHPIcon->addChild(_pNextHPLabel, 1);
		Label *_pNextHPValueLabel = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%d", _nHealthPoint));
		_pNextHPValueLabel->setBMFontSize(_pNextHPIcon->getContentSize().height);
		_pNextHPValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextHPValueLabel->setPosition(Vec2(_pNextHPLabel->getContentSize().width, 0.0f));
		_pNextHPLabel->addChild(_pNextHPValueLabel, 1);

		Sprite *_pNextScoreIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_score.png");
		_pNextScoreIcon->setScale(_pInfoBoard->getContentSize().height / _pNextScoreIcon->getContentSize().height * 0.07f);
		_pNextScoreIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextScoreIcon->setPosition(_pInfoBoard->getContentSize().width * 0.53f, _pInfoBoard->getContentSize().height * 0.225f);
		_pInfoBoard->addChild(_pNextScoreIcon, 1);
		Label *_pNextScoreLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 2).c_str()),
			"fontsDPM/UTM_DK_Drop_Dead.ttf", _pNextScoreIcon->getContentSize().height * 0.5f);
		_pNextScoreLabel->setColor(Color3B::BLACK);
		_pNextScoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pNextScoreLabel->setPosition(Vec2(_pNextScoreIcon->getContentSize().width, _pNextScoreIcon->getContentSize().height * 0.5f));
		_pNextScoreIcon->addChild(_pNextScoreLabel, 1);
		Label *_pNextScoreValueLabel = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%d%%", _nBonusScore));
		_pNextScoreValueLabel->setBMFontSize(_pNextScoreIcon->getContentSize().height * 0.7f);
		_pNextScoreValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextScoreValueLabel->setPosition(Vec2(_pNextScoreLabel->getContentSize().width, 0.0f));
		_pNextScoreLabel->addChild(_pNextScoreValueLabel, 1);

		Sprite *_pNextGoldIcon = Sprite::create("UI/UI_main_menu/InfoBoard/icon_coin.png");
		_pNextGoldIcon->setScale(_pInfoBoard->getContentSize().height / _pNextGoldIcon->getContentSize().height * 0.07f);
		_pNextGoldIcon->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextGoldIcon->setPosition(_pInfoBoard->getContentSize().width * 0.53f, _pInfoBoard->getContentSize().height * 0.15f);
		_pInfoBoard->addChild(_pNextGoldIcon, 1);
		Label *_pNextGoldLabel = Label::createWithTTF(StringUtils::format("%s", JSMENU->readMenuText(m_nLanguage, 3).c_str()),
			"fontsDPM/UTM_DK_Drop_Dead.ttf", _pNextGoldIcon->getContentSize().height * 0.5f);
		_pNextGoldLabel->setColor(Color3B::BLACK);
		_pNextGoldLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pNextGoldLabel->setPosition(Vec2(_pNextGoldIcon->getContentSize().width, _pNextGoldIcon->getContentSize().height * 0.5f));
		_pNextGoldIcon->addChild(_pNextGoldLabel, 1);
		Label *_pNextGoldValueLabel = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d%%", _nBonusGold));
		_pNextGoldValueLabel->setBMFontSize(_pNextGoldIcon->getContentSize().height * 0.7f);
		_pNextGoldValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pNextGoldValueLabel->setPosition(Vec2(_pNextGoldLabel->getContentSize().width, 0.0f));
		_pNextGoldLabel->addChild(_pNextGoldValueLabel, 1);
	}
	else {
		Sprite *_pMaxUpgrate = Sprite::create("UI/UI_main_menu/ItemBoard/icon_max.png");
		_pMaxUpgrate->setScale(_pInfoBoard->getContentSize().width / _pMaxUpgrate->getContentSize().width * 0.3f);
		_pMaxUpgrate->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pMaxUpgrate->setPosition(Vec2(_pInfoBoard->getContentSize().width * 0.7f, _pInfoBoard->getContentSize().height * 0.3f));
		_pInfoBoard->addChild(_pMaxUpgrate, 1);
	}
}

void MenuLayer::initBottomHeroMenu() {
	m_pBottomHeroLayer->removeAllChildrenWithCleanup(true);

	// hero face frame
	auto _pHeroFrame = Sprite::create("UI/UI_main_menu/HeroMenu/bottom_bar.png");
	_pHeroFrame->setScaleX(m_pBottomHeroLayer->getContentSize().width / _pHeroFrame->getContentSize().width * 0.9f);
	_pHeroFrame->setScaleY(m_pBottomHeroLayer->getContentSize().height / _pHeroFrame->getContentSize().height);
	_pHeroFrame->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pHeroFrame->setPosition(m_szVisibleSize.width / 50, 0.0f);
	m_pBottomHeroLayer->addChild(_pHeroFrame, 0);

	float _fHeroHeight = m_pBottomHeroLayer->getContentSize().height;
	float _fStepPositionX = m_pBottomHeroLayer->getContentSize().width * 0.1f;
	float _fXPositionCounter = _fStepPositionX / 2;

	for (int i = 0; i < 5; i++) {
		REF->pointToCurrentHero(i);
		JSHERO->readFile(m_nLanguage, i);
		Sprite *_pHeroSprite;
		if (REF->getIsLockedHero()) {
			_pHeroSprite = Sprite::create(JSHERO->getIconLocked());
			if (m_nIndexHeroPicked == i) {
				_pHeroSprite = Sprite::create(JSHERO->getIconLockedSelected());
			}
		}
		else {
			_pHeroSprite = Sprite::create(JSHERO->getIconUnlocked());
			if (m_nIndexHeroPicked == i) {
				_pHeroSprite = Sprite::create(JSHERO->getIconUnlockedSelected());
			}
		}
		m_arHeroButton[i] = MenuItemSprite::create(_pHeroSprite, _pHeroSprite, CC_CALLBACK_0(MenuLayer::buttonPickHeroHandle, this, i));
		float _fTemp = m_arHeroButton[i]->getContentSize().width * _fHeroHeight / _pHeroSprite->getContentSize().height;
		if (_fTemp > m_pBottomHeroLayer->getContentSize().width * 0.1f) {
			m_arHeroButton[i]->setScale(m_pBottomHeroLayer->getContentSize().width / _pHeroSprite->getContentSize().width * 0.1f);
		}
		else {
			m_arHeroButton[i]->setScale(_fHeroHeight / _pHeroSprite->getContentSize().height);
		}
		m_arHeroButton[i]->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_arHeroButton[i]->setPosition(Vec2(_fXPositionCounter, 0.0f));

		_fXPositionCounter += _fStepPositionX;
	}
	_fXPositionCounter -= _fStepPositionX;
	// button try
	auto _pTryHeroNormal = Sprite::create("UI/UI_main_menu/HeroMenu/btn_try.png");
	auto _pTryHeroSelected = Sprite::create("UI/UI_main_menu/HeroMenu/btn_try.png");
	_pTryHeroSelected->setColor(Color3B(128, 128, 128));
	auto _aTryHeroButton = MenuItemSprite::create(_pTryHeroNormal, _pTryHeroSelected, CC_CALLBACK_0(MenuLayer::buttonTryHeroHandle, this));
	_aTryHeroButton->setScale(m_pBottomHeroLayer->getContentSize().height / _aTryHeroButton->getContentSize().height);
	_aTryHeroButton->setAnchorPoint(Vec2(0.5f, 0.5f));
	_aTryHeroButton->setPosition((m_fButtonStartPosition + _fXPositionCounter + m_arHeroButton[4]->getContentSize().width * m_arHeroButton[4]->getScale()) / 2, 0.0f);

	// button unlock
	auto _pUnlockNormal = Sprite::create("UI/UI_main_menu/HeroMenu/btn_unlock.png");
	auto _pUnlockSelected = Sprite::create("UI/UI_main_menu/HeroMenu/btn_unlock.png");
	_pUnlockSelected->setColor(Color3B(128, 128, 128));
	auto _aUnlockButton = MenuItemSprite::create(_pUnlockNormal, _pUnlockSelected, CC_CALLBACK_0(MenuLayer::buttonUnlockHeroHandle, this));
	_aUnlockButton->setScaleX(m_pBottomHeroLayer->getContentSize().width / _aUnlockButton->getContentSize().width * 0.26f);
	_aUnlockButton->setScaleY(m_pBottomHeroLayer->getContentSize().height / _aUnlockButton->getContentSize().height);
	_aUnlockButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aUnlockButton->setPosition(m_fButtonStartPosition, 0.0f);

	REF->pointToCurrentHero(m_nIndexHeroPicked);
	if (!REF->getIsLockedHero()) {
		_aUnlockButton->setNormalImage(Sprite::create("UI/UI_main_menu/UpgradeBoard/btn_upgrade.png"));
		auto _pTemp = Sprite::create("UI/UI_main_menu/UpgradeBoard/btn_upgrade.png");
		_pTemp->setColor(Color3B(128, 128, 128));
		_aUnlockButton->setSelectedImage(_pTemp);
		auto _pTemp2 = Sprite::create("UI/UI_main_menu/UpgradeBoard/btn_upgrade.png");
		_pTemp2->setColor(Color3B(128, 128, 128));
		_aUnlockButton->setDisabledImage(_pTemp2);
		_aUnlockButton->setCallback(CC_CALLBACK_0(MenuLayer::buttonUpgradeHeroHandle, this));
		_aTryHeroButton->setVisible(false);
		if (REF->getCurrentLevel() == JSHERO->getMaxLevel()) {
			_aUnlockButton->setEnabled(false);
			_aUnlockButton->setColor(Color3B::GRAY);
		}
		else {
			float _nUpgradeCost = JSHERO->getGoldUpgradeLevelX(m_nIndexHeroPicked);
			for (int i = 1; i < REF->getCurrentLevel(); i++) {
				_nUpgradeCost *= 1.05f;
			}
			Label *_pLabelCost = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d", (int)(_nUpgradeCost)));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.6f, _aUnlockButton->getContentSize().height * 0.52f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
			_pCoinSprite->setScale(_aUnlockButton->getContentSize().height / _pCoinSprite->getContentSize().height * 0.3f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
			_pCoinSprite->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.65f, _aUnlockButton->getContentSize().height * 0.65f));
			_aUnlockButton->addChild(_pCoinSprite, 1);
		}
	}
	else {
		if (JSHERO->getGoldPrice() > 0) {
			Label *_pLabelCost = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d", JSHERO->getGoldPrice()));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.6f, _aUnlockButton->getContentSize().height * 0.52f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
			_pCoinSprite->setScale(_aUnlockButton->getContentSize().height / _pCoinSprite->getContentSize().height * 0.3f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
			_pCoinSprite->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.65f, _aUnlockButton->getContentSize().height * 0.65f));
			_aUnlockButton->addChild(_pCoinSprite, 1);
		}
		else if (JSHERO->getDiamondPrice() > 0) {
			Label *_pLabelCost = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%d", JSHERO->getDiamondPrice()));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.55f, _aUnlockButton->getContentSize().height * 0.52f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
			_pDiamondSprite->setScale(_aUnlockButton->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.3f);
			_pDiamondSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
			_pDiamondSprite->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.6f, _aUnlockButton->getContentSize().height * 0.65f));
			_aUnlockButton->addChild(_pDiamondSprite, 1);
		}
	}

	m_pBottomHeroMenu = Menu::create(m_arHeroButton[0], m_arHeroButton[1], m_arHeroButton[2], m_arHeroButton[3], m_arHeroButton[4], _aTryHeroButton, _aUnlockButton, NULL);
	m_pBottomHeroMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 27% screen height
	m_pBottomHeroMenu->setPosition(0.0f, 0.0f);
	m_pBottomHeroLayer->addChild(m_pBottomHeroMenu, 2);
}

void MenuLayer::buttonLeaderBoardHandle() {
	int a = 0;
	//if(SPHelper::getInstance()->isSigned())
	SPHelper::getInstance()->showBoard("score");
}

void MenuLayer::showMainMenu() {
	m_pBottomMainMenu->setEnabled(true);
	for (int i = 0; i < 5; i++) {
		m_arBuyItemButton[i]->setTouchEnabled(true);
	}
	moveLayerViaDirection(m_pBottomMainLayer, 8);
	moveLayerViaDirection(m_pItemBoard, 4);
}

void MenuLayer::hideMainMenu() {
	m_pBottomMainMenu->setEnabled(false);
	for (int i = 0; i < 5; i++) {
		m_arBuyItemButton[i]->setTouchEnabled(false);
	}
	moveLayerViaDirection(m_pBottomMainLayer, 2);
	moveLayerViaDirection(m_pItemBoard, 6);
}

void MenuLayer::showBlurScreen() {
	m_pTopMenu->setEnabled(false);
	if (m_nMenuStatus != 4) {
		m_pBottomMainMenu->setEnabled(false);
		for (int i = 0; i < 5; i++) {
			m_arBuyItemButton[i]->setTouchEnabled(false);
		}
		m_pSkillBoardMenu->setEnabled(false);
		m_pBottomHeroMenu->setEnabled(false);
		m_pQuestBoardMenu->setEnabled(false);
		m_pItemScrollView->setTouchEnabled(false);
	}
	m_pBlurScreen->setVisible(true);

	LayerColor *_pBlurBlackLayer = LayerColor::create(Color4B(0, 0, 0, 220));
	m_pBlurScreen->addChild(_pBlurBlackLayer, 0);
}

void MenuLayer::hideBlurScreen() {
	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
		m_pBlurScreen->removeAllChildrenWithCleanup(true);
		m_pBlurScreen->setVisible(false);
	}), nullptr));

	runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
		m_pTopMenu->setEnabled(true);
		if (m_nMenuStatus != 4) {
			m_pBottomMainMenu->setEnabled(true);
			for (int i = 0; i < 5; i++) {
				m_arBuyItemButton[i]->setTouchEnabled(true);
			}
			m_pSkillBoardMenu->setEnabled(true);
			m_pBottomHeroMenu->setEnabled(true);
			m_pQuestBoardMenu->setEnabled(true);
			m_pItemScrollView->setTouchEnabled(true);
		}
	}), nullptr));
}

void MenuLayer::buttonStartHandle() {
	// select stage layer
	m_pSelectStageLayer = SelectStageLayer::create(m_nIndexHeroSelected);
	m_nMenuStatus = 3;
	this->addChild(m_pSelectStageLayer, 3);

	m_pBottomMainMenu->setEnabled(false);
	for (int i = 0; i < 5; i++) {
		m_arBuyItemButton[i]->setTouchEnabled(false);
	}
	m_pSkillBoardMenu->setEnabled(false);
	m_pBottomHeroMenu->setEnabled(false);
	m_pQuestBoardMenu->setEnabled(false);
}

void MenuLayer::buttonBackHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_pShopBlurBackground->isVisible() == true) {
		buttonCloseShopHandle();
		return;
	}
	if (m_nMenuStatus == 0) {
		auto _aIntroScene = SceneIntro::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, _aIntroScene));
	}
	if (m_nMenuStatus == 1) {
		m_nMenuStatus = 0;
		moveLayerViaDirection(m_pQuestBoard, 4);
		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.2f, Vec2(0.0f, 0.0f)));
		}), nullptr));
		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			showMainMenu();
		}), nullptr));
	}
	if (m_nMenuStatus == 2) {
		m_nMenuStatus = 0;
		if (m_nIndexHeroSelected != m_nIndexHeroPicked) {
			m_nIndexHeroPicked = m_nIndexHeroSelected;
			REF->pointToCurrentHero(m_nIndexHeroPicked);
			initSceneLayer();
			initBottomHeroMenu();
			initHeroInfoBoard();
			initUpgradeBoard();
		}
		moveLayerViaDirection(m_pUpgradeBoard, 6);
		moveLayerViaDirection(m_pHeroInfoBoard, 4);
		moveLayerViaDirection(m_pBottomHeroLayer, 2);
		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.2f, Vec2(0.0f, 0.0f)));
		}), nullptr));
		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			showMainMenu();
		}), nullptr));
	}
	if (m_nMenuStatus == 3) {
		m_nMenuStatus = 0;
		m_pSelectStageLayer->removeFromParentAndCleanup(true);
		m_pBottomMainMenu->setEnabled(true);
		for (int i = 0; i < 5; i++) {
			m_arBuyItemButton[i]->setTouchEnabled(true);
		}
		m_pSkillBoardMenu->setEnabled(true);
		m_pBottomHeroMenu->setEnabled(true);
		m_pQuestBoardMenu->setEnabled(true);
	}
	if (m_nMenuStatus == 4) {
		Layer *_pMenuScene = MenuLayer::create(false);
		auto scene = Scene::create();
		scene->addChild(_pMenuScene);
		Director::getInstance()->replaceScene(scene);
	}
}

void MenuLayer::buttonAddLifeHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	m_pShopBlurBackground->setVisible(true);
	initShopBoard(2);
}

void MenuLayer::buttonAddGoldHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	m_pShopBlurBackground->setVisible(true);
	initShopBoard(0);
}

void MenuLayer::buttonAddDiamondHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	m_pShopBlurBackground->setVisible(true);
	initShopBoard(1);
}

void MenuLayer::buttonQuestHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	logButtonClickEvent("Quest");
	if (m_nMenuStatus != 1) {
		m_nMenuStatus = 1;
		hideMainMenu();
		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.2f, Vec2(m_szVisibleSize.width * 0.55f, m_szVisibleSize.height * (-0.15f))));
		}), nullptr));
		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pQuestBoard, 6);
		}), nullptr));
	}
}

void MenuLayer::buttonHeroesHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	logButtonClickEvent("Hero");
	if (m_nIndexHeroSelected != m_nIndexHeroPicked) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
	}
	initBottomHeroMenu();
	if (m_nMenuStatus != 2) {
		m_nMenuStatus = 2;
		hideMainMenu();

		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.2f, Vec2(m_szVisibleSize.width * 0.2f, m_szVisibleSize.height * 0.0f)));
		}), nullptr));

		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pUpgradeBoard, 4);
			moveLayerViaDirection(m_pHeroInfoBoard, 6);
			moveLayerViaDirection(m_pBottomHeroLayer, 8);
		}), nullptr));
	}
}

void MenuLayer::buttonShopHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	logButtonClickEvent("Shop");
	m_pShopBlurBackground->setVisible(true);
	initShopBoard(m_nShopOption);
}

void MenuLayer::buttonFreeCoinHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	logButtonClickEvent("Free coin");
	if (REF->getFreeCoin() > 0) {
		//GAHelper::getInstance()->logEvent("FreeCoin", "click", "can getFreecoin", 1);
		// TODO : show ads and check view ads finish
		// after that, increase gold
		VungleHelper::getInstance()->showReward();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(11), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
		//GAHelper::getInstance()->logEvent("FreeCoin", "click", "can not getFreecoin", 1);
	}
}

void MenuLayer::buttonSettingHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	//logButtonClickEvent("Setting");
	showBlurScreen();

	Sprite *_pSettingBackground = Sprite::create("UI/UI_main_menu/SettingBoard/setting_bg.png");

	float _fTemp = _pSettingBackground->getContentSize().height * m_pBlurScreen->getContentSize().width / _pSettingBackground->getContentSize().width * 0.6f;
	if (_fTemp > m_szVisibleSize.height * 0.6f) {
		_pSettingBackground->setScale(m_pBlurScreen->getContentSize().height / _pSettingBackground->getContentSize().height * 0.6f);
	}
	else {
		_pSettingBackground->setScale(m_pBlurScreen->getContentSize().width / _pSettingBackground->getContentSize().width * 0.6f);
	}
	_pSettingBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pSettingBackground->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
	m_pBlurScreen->addChild(_pSettingBackground, 1);

	// button close setting
	auto _pCloseNormal = Sprite::create("UI/UI_main_menu/SettingBoard/btn_close.png");
	_pCloseNormal->setColor(Color3B(240, 0, 0));
	auto _pCloseSelected = Sprite::create("UI/UI_main_menu/SettingBoard/btn_close.png");
	_pCloseSelected->setColor(Color3B(128, 128, 128));
	auto _aCloseButton = MenuItemSprite::create(_pCloseNormal, _pCloseSelected, CC_CALLBACK_0(MenuLayer::hideBlurScreen, this));
	_aCloseButton->setScale(_pSettingBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.2f);
	_aCloseButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	_aCloseButton->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.95f, _pSettingBackground->getContentSize().height * 0.9f));

	m_pLanguageButtonGroup = RadioButtonGroup::create();
	_pSettingBackground->addChild(m_pLanguageButtonGroup);

	Label *_pLabelEnglish = Label::createWithTTF(JSMENU->readMenuText(m_nLanguage, 8), "fontsDPM/UTM_BRUSHSCI.ttf", _pSettingBackground->getContentSize().height * 0.1f);
	_pLabelEnglish->setColor(Color3B::BLACK);
	_pLabelEnglish->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	_pLabelEnglish->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pLabelEnglish->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.35f, _pSettingBackground->getContentSize().height * 0.6f));
	_pSettingBackground->addChild(_pLabelEnglish, 1);
	RadioButton *_pEnglishButton = RadioButton::create("UI/UI_main_menu/SettingBoard/btn_off.png", "UI/UI_main_menu/SettingBoard/btn_on.png");
	_pEnglishButton->setScale(_pSettingBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.15f);
	_pEnglishButton->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pEnglishButton->setPosition(Vec2(0.0f, 0.0f));
	_pEnglishButton->addEventListener(CC_CALLBACK_0(MenuLayer::onChangedLanguage, this));
	_pEnglishButton->setTag(0);
	m_pLanguageButtonGroup->addRadioButton(_pEnglishButton);
	_pLabelEnglish->addChild(_pEnglishButton, 1);

	Label *_pLabelVietnamese = Label::createWithTTF(JSMENU->readMenuText(m_nLanguage, 9), "fontsDPM/UTM_BRUSHSCI.ttf", _pSettingBackground->getContentSize().height * 0.1f);
	_pLabelVietnamese->setColor(Color3B::BLACK);
	_pLabelVietnamese->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	_pLabelVietnamese->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pLabelVietnamese->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.65f, _pSettingBackground->getContentSize().height * 0.6f));
	_pSettingBackground->addChild(_pLabelVietnamese, 1);
	RadioButton *_pVietnameseButton = RadioButton::create("UI/UI_main_menu/SettingBoard/btn_off.png", "UI/UI_main_menu/SettingBoard/btn_on.png");
	_pVietnameseButton->setScale(_pSettingBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.15f);
	_pVietnameseButton->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pVietnameseButton->setPosition(Vec2(0.0f, 0.0f));
	_pVietnameseButton->addEventListener(CC_CALLBACK_0(MenuLayer::onChangedLanguage, this));
	_pVietnameseButton->setTag(1);
	m_pLanguageButtonGroup->addRadioButton(_pVietnameseButton);
	_pLabelVietnamese->addChild(_pVietnameseButton, 1);
	if (REF->getLanguage() == 0) {
		m_pLanguageButtonGroup->setSelectedButton(_pEnglishButton);
	}
	else {
		m_pLanguageButtonGroup->setSelectedButton(_pVietnameseButton);
	}

	// sound
	auto _aSoundOn = Sprite::create("UI/UI_main_menu/SettingBoard/btn_on.png");
	auto _aSoundOff = Sprite::create("UI/UI_main_menu/SettingBoard/btn_off.png");
	auto _aSoundControlOn = MenuItemSprite::create(_aSoundOn, _aSoundOn, NULL);
	auto _aSoundControlOff = MenuItemSprite::create(_aSoundOff, _aSoundOff, NULL);
	MenuItemToggle *_pButtonSoundControl = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuLayer::buttonSoundControlHandle, this),
		_aSoundControlOn, _aSoundControlOff, NULL);
	_pButtonSoundControl->setScale(_pSettingBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.15f);
	_pButtonSoundControl->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pButtonSoundControl->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.5f, _pSettingBackground->getContentSize().height * 0.4f));
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	bool checkSound = ref->getBoolForKey(KEY_IS_SOUND, true);
	_pButtonSoundControl->setSelectedIndex(checkSound == true ? 0 : 1);
	Label *_pLabelSound = Label::createWithTTF(" Sound", "fontsDPM/UTM_BRUSHSCI.ttf", _pSettingBackground->getContentSize().height * 0.1f);
	_pLabelSound->setColor(Color3B::BLACK);
	_pLabelSound->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	_pLabelSound->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pLabelSound->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.5f, _pSettingBackground->getContentSize().height * 0.4f));
	_pSettingBackground->addChild(_pLabelSound, 1);

	// music
	auto _aMusicOn = Sprite::create("UI/UI_main_menu/SettingBoard/btn_on.png");
	auto _aMusicOff = Sprite::create("UI/UI_main_menu/SettingBoard/btn_off.png");
	auto _aMusicControlOn = MenuItemSprite::create(_aMusicOn, _aMusicOn, NULL);
	auto _aMusicControlOff = MenuItemSprite::create(_aMusicOff, _aMusicOff, NULL);
	MenuItemToggle *_pButtonMusicControl = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuLayer::buttonMusicControlHandle, this),
		_aMusicControlOn, _aMusicControlOff, NULL);
	_pButtonMusicControl->setScale(_pSettingBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.15f);
	_pButtonMusicControl->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pButtonMusicControl->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.5f, _pSettingBackground->getContentSize().height * 0.15f));
	bool checkMusic = ref->getBoolForKey(KEY_IS_MUSIC, true);
	_pButtonMusicControl->setSelectedIndex(checkMusic == true ? 0 : 1);
	Label *_pLabelMusic = Label::createWithTTF(" Music", "fontsDPM/UTM_BRUSHSCI.ttf", _pSettingBackground->getContentSize().height * 0.1f);
	_pLabelMusic->setColor(Color3B::BLACK);
	_pLabelMusic->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	_pLabelMusic->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pLabelMusic->setPosition(Vec2(_pSettingBackground->getContentSize().width * 0.5f, _pSettingBackground->getContentSize().height * 0.15f));
	_pSettingBackground->addChild(_pLabelMusic, 1);

	Menu *_pShopMenu = Menu::create(_pButtonSoundControl, _pButtonMusicControl, _aCloseButton, NULL);
	_pShopMenu->setContentSize(Size(_pSettingBackground->getContentSize().width, _pSettingBackground->getContentSize().height));
	_pShopMenu->setPosition(0.0f, 0.0f);
	_pSettingBackground->addChild(_pShopMenu, 2);
}

void MenuLayer::buttonMoreGameHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	logButtonClickEvent("MoreGame");
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) { // if running on iOS
		//GAHelper::getInstance()->logEvent("Moregame", "click", "go to moregame ios", 1);
		Application::getInstance()->openURL("https://itunes.apple.com/us/developer/b-gate-jsc./id1037975709"); // open pipo iTunes
	}
	else { // running on another platform
		//GAHelper::getInstance()->logEvent("Moregame", "click", "go to moregame android", 1);
		Application::getInstance()->openURL("https://play.google.com/store/apps/dev?id=8988959007894361415&hl=vi"); // open pipo CHPlay
	}
}

void MenuLayer::buttonUpgradeSkillHandle(int p_nIndexSkill, int p_nCost) {
	AudioManager::playSound(SOUND_BTCLICK);

	if (m_nCurrentGold < p_nCost) {
		// TOAST
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(2), TOAST_SHORT);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
		return;
	}

	m_nCurrentGold -= p_nCost;
	REF->setDownGold(p_nCost);

	if (p_nIndexSkill == 0) {
		REF->increaseLevelSkill_1();
		REF->decreaseCoolDownSkill_X(1, 1);
		if ((REF->getLevelSkill_1() % 3) == 0) {		// reach 3, 6, 9 increase
			if (REF->getNumberUseSkill_1() > 1)
				REF->increaseNumberUseSkill_X(1, 1);
			else
				REF->increaseDurationSkill_X(1, 1);
		}
		logUpgradeSkillEvent(m_nIndexHeroPicked, p_nIndexSkill, REF->getLevelSkill_1());
	}
	if (p_nIndexSkill == 1) {
		REF->increaseLevelSkill_2();
		REF->decreaseCoolDownSkill_X(2, 1);
		if ((REF->getLevelSkill_2() % 3) == 0) {		// reach 3, 6, 9 increase
			if (REF->getNumberUseSkill_2() > 1)
				REF->increaseNumberUseSkill_X(2, 1);
			else
				REF->increaseDurationSkill_X(2, 1);
		}
		logUpgradeSkillEvent(m_nIndexHeroPicked, p_nIndexSkill, REF->getLevelSkill_2());
	}
	if (p_nIndexSkill == 2) {
		REF->increaseLevelSkill_3();
		REF->decreaseCoolDownSkill_X(3, 1);
		if ((REF->getLevelSkill_3() % 3) == 0) {		// reach 3, 6, 9 increase
			if (REF->getNumberUseSkill_3() > 1)
				REF->increaseNumberUseSkill_X(3, 1);
			else
				REF->increaseDurationSkill_X(3, 1);
		}
		logUpgradeSkillEvent(m_nIndexHeroPicked, p_nIndexSkill, REF->getLevelSkill_3());
	}

	initTopMainMenu();
	initUpgradeBoard();
}

void MenuLayer::receiveButtonPressEvent(Ref *pSender, Widget::TouchEventType type, int p_nIndexItem) {
	if (type == Widget::TouchEventType::ENDED) {
		buttonBuyItemHandle(p_nIndexItem);
	}
}

void MenuLayer::buttonBuyItemHandle(int p_nIndexItem) {
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nCurrentGold >= m_arItemPrice[p_nIndexItem]) {
		m_nCurrentGold -= m_arItemPrice[p_nIndexItem];
		m_arNumberItemOwning[p_nIndexItem]++;
		m_arLabelNumberItemOwning[p_nIndexItem]->setString((String::createWithFormat("%d", m_arNumberItemOwning[p_nIndexItem]))->getCString());
		m_arLabelNumberItemOwning[p_nIndexItem]->getParent()->setVisible(true);
		initTopMainMenu();

		// save data
		REF->setDownGold(m_arItemPrice[p_nIndexItem]);
		switch (p_nIndexItem) {
		case 0: {
					logBuyItemEvent("health");
					REF->increaseNumberItemHealth();
					m_arBuyItemButton[0]->setVisible(false);
					m_arSpriteItemMax[0]->setVisible(true);
					m_arItemCoinSprite[0]->setVisible(false);
					m_arItemLabelCost[0]->setVisible(false);
					break;
		}
		case 1: {
					logBuyItemEvent("bird");
					REF->increaseNumberItemBird();
					m_arBuyItemButton[1]->setVisible(false);
					m_arSpriteItemMax[1]->setVisible(true);
					m_arItemCoinSprite[1]->setVisible(false);
					m_arItemLabelCost[1]->setVisible(false);
					break;
		}
		case 2: {
					logBuyItemEvent("magnet");
					REF->increaseNumberItemMagnet();
					m_arBuyItemButton[2]->setVisible(false);
					m_arSpriteItemMax[2]->setVisible(true);
					m_arItemCoinSprite[2]->setVisible(false);
					m_arItemLabelCost[2]->setVisible(false);
					break;
		}
		case 3: {
					logBuyItemEvent("X2Gold");
					REF->increaseNumberItemDoubleGold();
					m_arBuyItemButton[3]->setVisible(false);
					m_arSpriteItemMax[3]->setVisible(true);
					m_arItemCoinSprite[3]->setVisible(false);
					m_arItemLabelCost[3]->setVisible(false);
					break;
		}
		case 4: {
					logBuyItemEvent("CoolDown");
					REF->increaseNumberItemCoolDown();
					m_arBuyItemButton[4]->setVisible(false);
					m_arSpriteItemMax[4]->setVisible(true);
					m_arItemCoinSprite[4]->setVisible(false);
					m_arItemLabelCost[4]->setVisible(false);
					break;
		}
		default: {
					 break;
		}
		}
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(8), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonRewardQuest(int p_nQuestIndex) {
	AudioManager::playSound(SOUND_BTCLICK);
	// update gold by reward
	JSQUEST->readQuest(m_nLanguage, p_nQuestIndex);
	int _nGoldReward = JSQUEST->getGoldReward();
	for (int j = 0; j < REF->getRewardedQuestTimes(); j++) {
		_nGoldReward *= JSQUEST->getStepRequest();
	}
	m_nCurrentGold += _nGoldReward;

	REF->updateRewardedQuestTimes(p_nQuestIndex);
	REF->setUpGoldExplored(_nGoldReward);
	initQuestBoard(p_nQuestIndex);
	initTopMainMenu();
	//log
}

bool MenuLayer::createRequestToGoogle() {
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("https://www.google.com");
	request->setResponseCallback(CC_CALLBACK_2(MenuLayer::onHttpRequestCompleted, this));
	request->setTag("Get test");
	HttpClient::getInstance()->send(request);
	request->release();

	return true;
}

void MenuLayer::onHttpRequestCompleted(HttpClient *p_pSender, HttpResponse *p_pResponse) {
	if (p_pResponse && p_pResponse->getResponseCode() == 200 && p_pResponse->getResponseData()) { // is able to get data from google
		vector<char> *_pData = p_pResponse->getResponseHeader();
		string _sResult(&(_pData->front()), _pData->size());
		//CCLOG("%s", ("Response message: " + _sResult).c_str());
		m_nCurrentTimeFromGoogle = calTimeFromString(_sResult);
		//CCLOG("m_nCurrentTimeFromGoogle : %d", m_nCurrentTimeFromGoogle);
		initDailyRewardBoard();
	}
	else {
		// CCLOG("%s", ("Error " + to_string(p_pResponse->getResponseCode()) + " in request").c_str());
		/*CustomLayerToToast *_pToast = CustomLayerToToast::create("Turn on your internet connection to get daily reward information", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pGameControl->addChild(_pToast, 10);*/
	}
}

int MenuLayer::calTimeFromString(string p_sInputString) {
	int _nGMTPosition = p_sInputString.find(" GMT");
	int _nYear = (p_sInputString.at(_nGMTPosition - 13) - 48) * 1000
		+ (p_sInputString.at(_nGMTPosition - 12) - 48) * 100
		+ (p_sInputString.at(_nGMTPosition - 11) - 48) * 10
		+ (p_sInputString.at(_nGMTPosition - 10) - 48);

	int _nMonth = 0;
	string _sMonth = p_sInputString.substr(_nGMTPosition - 17, 3);
	string _arMonths[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	for (int i = 0; i < 12; i++) {
		if (_sMonth.compare(_arMonths[i]) == 0) {
			_nMonth = i;
			break;
		}
	}
	int _nDay = (p_sInputString.at(_nGMTPosition - 20) - 48) * 10 + (p_sInputString.at(_nGMTPosition - 19) - 48) * 1;

	tm _pTempTime;
	_pTempTime.tm_year = _nYear - 1900;
	_pTempTime.tm_mon = _nMonth;
	_pTempTime.tm_mday = _nDay;
	_pTempTime.tm_hour = 0;
	_pTempTime.tm_min = 0;
	_pTempTime.tm_sec = 0;
	_pTempTime.tm_isdst = -1;

	return mktime(&_pTempTime);
}

void MenuLayer::initDailyRewardBoard() {
	if (REF->getDailyRewardCounter() > 6) {
		return;
	}
	time_t _nLastDailyRewardTime = REF->getLastDailyRewardTime();

	if (_nLastDailyRewardTime < m_nCurrentTimeFromGoogle) { // if realtime from google.vn "greater than" last daily reward time "more than 1 day"
		REF->updateDailyRewardAvailable(true);
		REF->resetFreeCoin();
		m_pSpriteFreeCoinAttention->setVisible(true);
		REF->updateTimeFromGoogle(m_nCurrentTimeFromGoogle);
	}
	if (REF->getDailyRewardAvailable()) { // if daily reward is available
		showBlurScreen(); // open daily reward

		Sprite *_pDailyRewardBackground = Sprite::create("UI/UI_main_menu/DailyReward/daily_reward_bg.png");
		float _fTemp = _pDailyRewardBackground->getContentSize().height * m_szVisibleSize.width / _pDailyRewardBackground->getContentSize().width * 0.9f;
		if (_fTemp > m_szVisibleSize.height * 0.7f) {
			_pDailyRewardBackground->setScale(m_szVisibleSize.height / _pDailyRewardBackground->getContentSize().height * 0.7f);
		}
		else {
			_pDailyRewardBackground->setScale(m_szVisibleSize.width / _pDailyRewardBackground->getContentSize().width * 0.9f);
		}
		_pDailyRewardBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pDailyRewardBackground->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
		m_pBlurScreen->addChild(_pDailyRewardBackground, 1);

		Sprite *_pDailyRewardTitle = Sprite::create("UI/UI_main_menu/DailyReward/titles.png");
		_pDailyRewardTitle->setScale(_pDailyRewardBackground->getContentSize().width / _pDailyRewardTitle->getContentSize().width * 0.6f);
		_pDailyRewardTitle->setAnchorPoint(Vec2(0.5f, 0.0f));
		_pDailyRewardTitle->setPosition(_pDailyRewardBackground->getContentSize().width * 0.5f, _pDailyRewardBackground->getContentSize().height * 0.85f);
		_pDailyRewardBackground->addChild(_pDailyRewardTitle, 1);

		float _fCounterX = _pDailyRewardBackground->getContentSize().width * 0.2f;
		float _fCounterY = _pDailyRewardBackground->getContentSize().height * 0.5f;
		for (int i = 0; i < 7; i++) {
			if (i == 3) {
				_fCounterX = _pDailyRewardBackground->getContentSize().width * 0.1f;
				_fCounterY = _pDailyRewardBackground->getContentSize().height * 0.15f;
			}

			Sprite *_pDailyReward = Sprite::create(StringUtils::format("UI/UI_main_menu/DailyReward/day_%d.png", i + 1));

			float _fTemp = _pDailyReward->getContentSize().height * _pDailyRewardBackground->getContentSize().width / _pDailyReward->getContentSize().width * 0.2f;
			if (_fTemp > _pDailyRewardBackground->getContentSize().height * 0.4f) {
				_pDailyReward->setScale(_pDailyRewardBackground->getContentSize().height / _pDailyReward->getContentSize().height * 0.4f);
			}
			else {
				_pDailyReward->setScale(_pDailyRewardBackground->getContentSize().width / _pDailyReward->getContentSize().width * 0.2f);
			}
			_pDailyReward->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pDailyReward->setPosition(_fCounterX, _fCounterY);
			_pDailyRewardBackground->addChild(_pDailyReward, 1);

			JSQUEST->readDailyReward(i);
			Label *_pRewardNumber = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("+%d", JSQUEST->getDailyRewardNumber()));
			_pRewardNumber->setBMFontSize(_pDailyReward->getContentSize().height * 0.2f);
			_pRewardNumber->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pRewardNumber->setPosition(Vec2(_pDailyReward->getContentSize().width * 0.55f, _pDailyReward->getContentSize().height * 0.1f));
			_pDailyReward->addChild(_pRewardNumber, 1);
			Label *_pDayNumber = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%d", i + 1));
			_pDayNumber->setBMFontSize(_pDailyReward->getContentSize().height * 0.2f);
			_pDayNumber->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pDayNumber->setPosition(Vec2(_pDailyReward->getContentSize().width * 0.2f, _pDailyReward->getContentSize().height * 0.3f));
			_pDailyReward->addChild(_pDayNumber, 1);

			if (i < REF->getDailyRewardCounter()) {
				Sprite *_pDailyRewardCheck = Sprite::create("UI/UI_main_menu/DailyReward/recevived.png");
				_pDailyRewardCheck->setScale(_pDailyReward->getContentSize().width / _pDailyRewardCheck->getContentSize().width * 0.6f);
				_pDailyRewardCheck->setAnchorPoint(Vec2(0.5f, 0.5f));
				_pDailyRewardCheck->setPosition(_pDailyReward->getContentSize().width * 0.6f, _pDailyReward->getContentSize().height * 0.6f);
				_pDailyReward->addChild(_pDailyRewardCheck, 1);
			}
			if (i == REF->getDailyRewardCounter()) {
				Sprite *_pDailyRewardCheck = Sprite::create("UI/UI_main_menu/DailyReward/recevive_on.png");
				_pDailyRewardCheck->setScale(_pDailyReward->getContentSize().width / _pDailyRewardCheck->getContentSize().width * 1.15f);
				_pDailyRewardCheck->setAnchorPoint(Vec2(0.5f, 0.5f));
				_pDailyRewardCheck->setPosition(_pDailyReward->getContentSize().width * 0.5f, _pDailyReward->getContentSize().height * 0.5f);
				_pDailyReward->addChild(_pDailyRewardCheck, 1);

				Sequence *_pBlink = Sequence::create(Blink::create(1.0f, 5), Show::create(), NULL);
				_pDailyRewardCheck->runAction(RepeatForever::create(_pBlink));

				m_pReceviveDailyRewardSprite = Sprite::create("UI/UI_main_menu/DailyReward/recevived.png");
				m_pReceviveDailyRewardSprite->setScale(_pDailyReward->getContentSize().width / m_pReceviveDailyRewardSprite->getContentSize().width * 3.0f);
				m_pReceviveDailyRewardSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
				m_pReceviveDailyRewardSprite->setPosition(_pDailyReward->getContentSize().width * 0.6f, _pDailyReward->getContentSize().height * 0.6f);
				_pDailyReward->addChild(m_pReceviveDailyRewardSprite, 1);
				m_pReceviveDailyRewardSprite->setVisible(false);
			}

			_fCounterX += _pDailyRewardBackground->getContentSize().width * 0.2f;
		}

		// button reward
		auto _pRewardNormal = Sprite::create("UI/UI_main_menu/DailyReward/btn_recevive.png");
		auto _pRewardSelected = Sprite::create("UI/UI_main_menu/DailyReward/btn_recevive.png");
		auto _aRewardButton = MenuItemSprite::create(_pRewardNormal, _pRewardSelected, CC_CALLBACK_0(MenuLayer::buttonDailyRewardHandle, this));
		_aRewardButton->setScale(_pDailyRewardBackground->getContentSize().height / _aRewardButton->getContentSize().height * 0.2f);
		_aRewardButton->setAnchorPoint(Vec2(0.5f, 1.0f));
		_aRewardButton->setPosition(_pDailyRewardBackground->getContentSize().width * 0.5f, _pDailyRewardBackground->getContentSize().height * 0.05f);
		buttonSpringy(_aRewardButton);

		Menu *_pDailyRewardDialogMenu = Menu::create(_aRewardButton, NULL);
		_pDailyRewardDialogMenu->setContentSize(Size(_pDailyRewardBackground->getContentSize().width, _pDailyRewardBackground->getContentSize().height));
		_pDailyRewardDialogMenu->setPosition(0.0f, 0.0f);
		_pDailyRewardBackground->addChild(_pDailyRewardDialogMenu, 2);

		float _fTempScale = _pDailyRewardBackground->getScale();
		_pDailyRewardBackground->setScale(0.0f);
		_pDailyRewardBackground->runAction(ScaleTo::create(0.2f, _fTempScale));
	}
}

void MenuLayer::initShopBoard(int p_nOption) {
	if (m_nMenuStatus == 3 || m_nMenuStatus == 4) {
		m_pSelectStageLayer->getmScrollView()->setEnabled(false);
		m_pSelectStageLayer->getmMenu()->setEnabled(false);
	}
	if (m_nMenuStatus != 4) {
		m_pBottomMainMenu->setEnabled(false);
		for (int i = 0; i < 5; i++) {
			m_arBuyItemButton[i]->setTouchEnabled(false);
		}
		m_pSkillBoardMenu->setEnabled(false);
		m_pBottomHeroMenu->setEnabled(false);
		m_pQuestBoardMenu->setEnabled(false);
		m_pItemScrollView->setTouchEnabled(false);
	}

	m_nShopOption = p_nOption;
	m_pShopBoardLayer->removeAllChildrenWithCleanup(true);

	Sprite *_pShopBackground = Sprite::create("UI/UI_main_menu/ShopBoard/board_shop.png");
	_pShopBackground->setScale(m_pShopBoardLayer->getContentSize().width / _pShopBackground->getContentSize().width * 0.9f,
		m_pShopBoardLayer->getContentSize().height / _pShopBackground->getContentSize().height * 0.8f);
	_pShopBackground->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pShopBackground->setPosition(m_pShopBoardLayer->getContentSize().width * 0.5f, m_pShopBoardLayer->getContentSize().height * 0.05f);
	m_pShopBoardLayer->addChild(_pShopBackground, 1);

	m_pPacksZone = ListView::create();
	m_pPacksZone->setContentSize(Size(m_pShopBoardLayer->getContentSize().width * 0.8f, m_pShopBoardLayer->getContentSize().height * 0.6f));
	m_pPacksZone->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_pPacksZone->setPosition(Vec2(m_pShopBoardLayer->getContentSize().width * 0.1f, m_pShopBoardLayer->getContentSize().height * 0.11f));
	m_pPacksZone->setDirection(ListView::Direction::HORIZONTAL);
	m_pPacksZone->setSwallowTouches(true);
	m_pPacksZone->setBounceEnabled(true);
	m_pPacksZone->setTouchEnabled(true);
	m_pPacksZone->setScrollBarOpacity(0.0f);
	m_pShopBoardLayer->addChild(m_pPacksZone, 1);

	m_pPacksZone->addEventListener((ListView::ccScrollViewCallback)CC_CALLBACK_2(MenuLayer::scrollShopHandle, this));
	m_pPacksZone->addEventListener((ListView::ccListViewCallback)CC_CALLBACK_2(MenuLayer::selectedItemEvent, this));

	// button energy tab
	auto _pEnergyTabNormal = Sprite::create("UI/UI_main_menu/ShopBoard/tab_energy_off.png");
	auto _pEnergyTabSelected = Sprite::create("UI/UI_main_menu/ShopBoard/tab_energy_off.png");
	auto _aEnergyTabButton = MenuItemSprite::create(_pEnergyTabNormal, _pEnergyTabSelected, CC_CALLBACK_0(MenuLayer::initShopBoard, this, 2));
	_aEnergyTabButton->setScale(m_pShopBoardLayer->getContentSize().width / _aEnergyTabButton->getContentSize().width * 0.25f);
	_aEnergyTabButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	_aEnergyTabButton->setPosition(m_pShopBoardLayer->getContentSize().width * 0.25f, m_pShopBoardLayer->getContentSize().height * 0.85f);

	// button gold tab
	auto _pGoldTabNormal = Sprite::create("UI/UI_main_menu/ShopBoard/tab_gold_off.png");
	auto _pGoldTabTabSelected = Sprite::create("UI/UI_main_menu/ShopBoard/tab_gold_off.png");
	auto _aGoldTabTabButton = MenuItemSprite::create(_pGoldTabNormal, _pGoldTabTabSelected, CC_CALLBACK_0(MenuLayer::initShopBoard, this, 0));
	_aGoldTabTabButton->setScale(m_pShopBoardLayer->getContentSize().width / _aGoldTabTabButton->getContentSize().width * 0.25f);
	_aGoldTabTabButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	_aGoldTabTabButton->setPosition(m_pShopBoardLayer->getContentSize().width * 0.5f, m_pShopBoardLayer->getContentSize().height * 0.85f);

	// button diamond tab
	auto _pDiamondTabNormal = Sprite::create("UI/UI_main_menu/ShopBoard/tab_diamond_off.png");
	auto _pDiamondTabSelected = Sprite::create("UI/UI_main_menu/ShopBoard/tab_diamond_off.png");
	auto _aDiamondTabButton = MenuItemSprite::create(_pDiamondTabNormal, _pDiamondTabSelected, CC_CALLBACK_0(MenuLayer::initShopBoard, this, 1));
	_aDiamondTabButton->setScale(m_pShopBoardLayer->getContentSize().width / _aDiamondTabButton->getContentSize().width * 0.25f);
	_aDiamondTabButton->setAnchorPoint(Vec2(0.5f, 1.0f));
	_aDiamondTabButton->setPosition(m_pShopBoardLayer->getContentSize().width * 0.75f, m_pShopBoardLayer->getContentSize().height * 0.85f);

	// button close shop
	auto _pCloseNormal = Sprite::create("UI/UI_main_menu/ShopBoard/btn_close_shop.png");
	auto _pCloseSelected = Sprite::create("UI/UI_main_menu/ShopBoard/btn_close_shop.png");
	_pCloseSelected->setColor(Color3B(128, 128, 128));
	auto _aCloseButton = MenuItemSprite::create(_pCloseNormal, _pCloseSelected, CC_CALLBACK_0(MenuLayer::buttonCloseShopHandle, this));
	_aCloseButton->setScale(m_pShopBoardLayer->getContentSize().height / _aCloseButton->getContentSize().height * 0.1f);
	_aCloseButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	_aCloseButton->setPosition(m_pShopBoardLayer->getContentSize().width * 0.95f, m_pShopBoardLayer->getContentSize().height * 0.85f);

	m_pShopMenu = Menu::create(_aGoldTabTabButton, _aDiamondTabButton, _aEnergyTabButton, _aCloseButton, NULL);
	m_pShopMenu->setContentSize(Size(m_pShopBoardLayer->getContentSize().width, m_pShopBoardLayer->getContentSize().height));
	m_pShopMenu->setPosition(0.0f, 0.0f);
	m_pShopBoardLayer->addChild(m_pShopMenu, 2);

	if (p_nOption == 0) {
		_aGoldTabTabButton->setNormalImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_gold_on.png"));
		_aGoldTabTabButton->setSelectedImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_gold_on.png"));

		for (int i = 0; i < JSMENU->getNumberGoldPack(); i++) {
			JSMENU->readGoldPack(i);
			auto _pCoinPackBackground = TMXTiledMap::create(JSMENU->getIconGoldPackPath());
			loadTwinkle(_pCoinPackBackground, 0.15f, 0.25f);
			Sprite *_pPackCostSprite = Sprite::create("UI/UI_main_menu/ShopBoard/btn_price.png");
			float _fTempScale = m_pPacksZone->getContentSize().height / (_pCoinPackBackground->getContentSize().height + _pPackCostSprite->getContentSize().height);

			auto _aPackButton = ui::Button::create();
			_aPackButton->setTouchEnabled(true);
			_aPackButton->ignoreContentAdaptWithSize(false);
			_aPackButton->setContentSize(Size(_pCoinPackBackground->getContentSize().width * _fTempScale * 1.1f,
				m_pPacksZone->getContentSize().height));
			_aPackButton->setAnchorPoint(Vec2(0.0f, 0.0f));
			auto _aWidget = Widget::create();
			_aWidget->setContentSize(_aPackButton->getContentSize());
			_aWidget->addChild(_aPackButton);
			m_pPacksZone->pushBackCustomItem(_aWidget);

			_pCoinPackBackground->setScale(_fTempScale);
			_pCoinPackBackground->setAnchorPoint(Vec2(0.5f, 1.0f));
			_pCoinPackBackground->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, _aPackButton->getContentSize().height));
			_aPackButton->addChild(_pCoinPackBackground, 1);

			// coin number
			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
			_pCoinSprite->setScale(_pCoinPackBackground->getContentSize().height / _pCoinSprite->getContentSize().height * 0.12f);
			_pCoinSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pCoinSprite->setPosition(Vec2(_pCoinPackBackground->getContentSize().width * 0.2f, _pCoinPackBackground->getContentSize().height * 0.02f));
			_pCoinPackBackground->addChild(_pCoinSprite, 1);
			Label *_pLabelCoinNumber = Label::createWithBMFont("fontsDPM/font_coin-export.fnt", StringUtils::format("%d", JSMENU->getCoinPackNumberGold()));
			_pLabelCoinNumber->setBMFontSize(_pCoinPackBackground->getContentSize().height * 0.13f);
			_pLabelCoinNumber->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pLabelCoinNumber->setPosition(Vec2(_pCoinPackBackground->getContentSize().width * 0.6f, _pCoinPackBackground->getContentSize().height * 0.03f));
			_pCoinPackBackground->addChild(_pLabelCoinNumber, 1);

			// diamond cost
			_pPackCostSprite->setScale(_fTempScale);
			_pPackCostSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pPackCostSprite->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, 0.0f));
			_aPackButton->addChild(_pPackCostSprite, 0);
			Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
			_pDiamondSprite->setScale(_pPackCostSprite->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.6f);
			_pDiamondSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pDiamondSprite->setPosition(Vec2(_pPackCostSprite->getContentSize().width * 0.4f, _pPackCostSprite->getContentSize().height * 0.2f));
			_pPackCostSprite->addChild(_pDiamondSprite, 1);

			Label *_pLabelDiamondCost = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%d", JSMENU->getCoinPackDiamondPrice()));
			_pLabelDiamondCost->setBMFontSize(_pPackCostSprite->getContentSize().height * 0.5f);
			_pLabelDiamondCost->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pLabelDiamondCost->setPosition(Vec2(_pPackCostSprite->getContentSize().width * 0.45f, _pPackCostSprite->getContentSize().height * 0.3f));
			_pPackCostSprite->addChild(_pLabelDiamondCost, 1);
		}
	}
	if (p_nOption == 1) {
		_aDiamondTabButton->setNormalImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_diamond_on.png"));
		_aDiamondTabButton->setSelectedImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_diamond_on.png"));

		for (int i = 0; i < JSMENU->getNumberDiamondPack(); i++) {
			JSMENU->readDiamondPack(i);
			auto _pDiamondPackBackground = TMXTiledMap::create(JSMENU->getIconDiamondPackPath());
			loadTwinkle(_pDiamondPackBackground, 0.15f, 0.25f);
			Sprite *_pPackCostSprite = Sprite::create("UI/UI_main_menu/ShopBoard/btn_price.png");
			float _fTempScale = m_pPacksZone->getContentSize().height / (_pDiamondPackBackground->getContentSize().height + _pPackCostSprite->getContentSize().height);

			auto _aPackButton = ui::Button::create();
			_aPackButton->setTouchEnabled(true);
			_aPackButton->ignoreContentAdaptWithSize(false);
			_aPackButton->setContentSize(Size(_pDiamondPackBackground->getContentSize().width * _fTempScale * 1.1f, m_pPacksZone->getContentSize().height));
			_aPackButton->setAnchorPoint(Vec2(0.0f, 0.0f));
			auto _aWidget = Widget::create();
			_aWidget->setContentSize(_aPackButton->getContentSize());
			_aWidget->addChild(_aPackButton);
			m_pPacksZone->pushBackCustomItem(_aWidget);

			_pDiamondPackBackground->setScale(_fTempScale);
			_pDiamondPackBackground->setAnchorPoint(Vec2(0.5f, 1.0f));
			_pDiamondPackBackground->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, _aPackButton->getContentSize().height));
			_aPackButton->addChild(_pDiamondPackBackground, 1);

			// coin number
			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
			_pCoinSprite->setScale(_pDiamondPackBackground->getContentSize().height / _pCoinSprite->getContentSize().height * 0.12f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pCoinSprite->setPosition(Vec2(_pDiamondPackBackground->getContentSize().width * 0.15f, _pDiamondPackBackground->getContentSize().height * 0.02f));
			_pDiamondPackBackground->addChild(_pCoinSprite, 1);
			Label *_pLabelCoinNumber = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%d", JSMENU->getDiamondPackNumberDiamond()));
			_pLabelCoinNumber->setBMFontSize(_pDiamondPackBackground->getContentSize().height * 0.13f);
			_pLabelCoinNumber->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pLabelCoinNumber->setPosition(Vec2(_pDiamondPackBackground->getContentSize().width * 0.6f, _pDiamondPackBackground->getContentSize().height * 0.03f));
			_pDiamondPackBackground->addChild(_pLabelCoinNumber, 1);

			// diamond cost
			_pPackCostSprite->setScale(_fTempScale);
			_pPackCostSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pPackCostSprite->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, 0.0f));
			_aPackButton->addChild(_pPackCostSprite, 0);
			Label *_pLabelDiamondCost = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%s", JSMENU->getDiamondPackMoneyPrice().c_str()));
			_pLabelDiamondCost->setBMFontSize(_pPackCostSprite->getContentSize().height * 0.5f);
			_pLabelDiamondCost->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pLabelDiamondCost->setPosition(Vec2(_pPackCostSprite->getContentSize().width * 0.5f, _pPackCostSprite->getContentSize().height * 0.3f));
			_pPackCostSprite->addChild(_pLabelDiamondCost, 1);
		}
	}
	if (p_nOption == 2) {
		_aEnergyTabButton->setNormalImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_energy_on.png"));
		_aEnergyTabButton->setSelectedImage(Sprite::create("UI/UI_main_menu/ShopBoard/tab_energy_on.png"));

		for (int i = 0; i < JSMENU->getNumberEnergyPack(); i++) {
			JSMENU->readEnergyPack(i);

			Sprite *_pEnergyPackBackground = Sprite::create(JSMENU->getIconEnergyPackPath());
			Sprite *_pPackCostSprite = Sprite::create("UI/UI_main_menu/ShopBoard/btn_price.png");
			float _fTempScale = m_pPacksZone->getContentSize().height / (_pEnergyPackBackground->getContentSize().height + _pPackCostSprite->getContentSize().height);

			auto _aPackButton = ui::Button::create();
			_aPackButton->setTouchEnabled(true);
			_aPackButton->ignoreContentAdaptWithSize(false);
			_aPackButton->setContentSize(Size(_pEnergyPackBackground->getContentSize().width * _fTempScale * 1.1f, m_pPacksZone->getContentSize().height));
			_aPackButton->setAnchorPoint(Vec2(0.0f, 0.0f));
			auto _aWidget = Widget::create();
			_aWidget->setContentSize(_aPackButton->getContentSize());
			_aWidget->addChild(_aPackButton);
			m_pPacksZone->pushBackCustomItem(_aWidget);

			_pEnergyPackBackground->setScale(_fTempScale);
			_pEnergyPackBackground->setAnchorPoint(Vec2(0.5f, 1.0f));
			_pEnergyPackBackground->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, _aPackButton->getContentSize().height));
			_aPackButton->addChild(_pEnergyPackBackground, 1);

			// energy number
			Label *_pLabelCoinNumber = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("x%d", JSMENU->getEnergyPackNumberEnergy()));
			_pLabelCoinNumber->setBMFontSize(_pEnergyPackBackground->getContentSize().height * 0.13f);
			_pLabelCoinNumber->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pLabelCoinNumber->setPosition(Vec2(_pEnergyPackBackground->getContentSize().width * 0.5f, _pEnergyPackBackground->getContentSize().height * 0.03f));
			_pEnergyPackBackground->addChild(_pLabelCoinNumber, 1);

			// diamond cost
			_pPackCostSprite->setScale(_fTempScale);
			_pPackCostSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pPackCostSprite->setPosition(Vec2(_aPackButton->getContentSize().width * 0.5f, 0.0f));
			_aPackButton->addChild(_pPackCostSprite, 0);
			Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
			_pDiamondSprite->setScale(_pPackCostSprite->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.6f);
			_pDiamondSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pDiamondSprite->setPosition(Vec2(_pPackCostSprite->getContentSize().width * 0.48f, _pPackCostSprite->getContentSize().height * 0.2f));
			_pPackCostSprite->addChild(_pDiamondSprite, 1);
			Label *_pLabelDiamondCost = Label::createWithBMFont("fontsDPM/font_life-export.fnt", StringUtils::format("%d", JSMENU->getEnergyPackDiamondPrice()));
			_pLabelDiamondCost->setBMFontSize(_pPackCostSprite->getContentSize().height * 0.5f);
			_pLabelDiamondCost->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pLabelDiamondCost->setPosition(Vec2(_pPackCostSprite->getContentSize().width * 0.53f, _pPackCostSprite->getContentSize().height * 0.3f));
			_pPackCostSprite->addChild(_pLabelDiamondCost, 1);
		}
	}

	m_pShopBoardLayer->runAction(ScaleTo::create(0.2f, 1.0f));
}

void MenuLayer::buttonCloseShopHandle() {
	if (m_nMenuStatus == 3 || m_nMenuStatus == 4) {
		m_pSelectStageLayer->getmScrollView()->setEnabled(true);
		m_pSelectStageLayer->getmMenu()->setEnabled(true);
	}
	if (m_nMenuStatus != 4) {
		m_pBottomMainMenu->setEnabled(true);
		for (int i = 0; i < 5; i++) {
			m_arBuyItemButton[i]->setTouchEnabled(true);
		}
		m_pSkillBoardMenu->setEnabled(true);
		m_pBottomHeroMenu->setEnabled(true);
		m_pQuestBoardMenu->setEnabled(true);
		m_pItemScrollView->setTouchEnabled(true);
	}

	AudioManager::playSound(SOUND_BTCLICK);
	m_pShopBoardLayer->runAction(ScaleTo::create(0.2f, 0.0f));
	// TODO: fix custom sprite to buy pack, because if you dont remove children of shop board, they still get response clicks on screen
	m_pShopBlurBackground->setVisible(false);
	m_pTopMenu->setEnabled(true);
	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
		m_pShopBoardLayer->removeAllChildrenWithCleanup(true);
	}), nullptr));
}

void MenuLayer::buttonDailyRewardHandle() {
	if (m_pReceviveDailyRewardSprite->isVisible() == true) {
		return;
	}
	AudioManager::playSound(SOUND_BTCLICK);
	JSQUEST->readDailyReward(REF->getDailyRewardCounter());

	m_pReceviveDailyRewardSprite->setVisible(true);
	m_pReceviveDailyRewardSprite->runAction(ScaleTo::create(0.3f, m_pReceviveDailyRewardSprite->getParent()->getContentSize().width / m_pReceviveDailyRewardSprite->getContentSize().width * 0.6f));

	runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([&]() {
		m_pBuyPackConfirmBackground->setVisible(true);
		Sprite *_pConfirmBackground = Sprite::create("UI/UI_main_menu/DailyReward/popup_confirm.png");
		_pConfirmBackground->setScale(m_pBuyPackConfirmBackground->getContentSize().height / _pConfirmBackground->getContentSize().height * 0.35f);
		_pConfirmBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pConfirmBackground->setPosition(m_pBuyPackConfirmBackground->getContentSize().width * 0.5f, m_pBuyPackConfirmBackground->getContentSize().height * 0.5f);
		m_pBuyPackConfirmBackground->addChild(_pConfirmBackground, 1);

		Sprite *_pIconSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
		if (JSQUEST->getDailyRewardType().compare("gold") == 0) {
			m_nCurrentGold += JSQUEST->getDailyRewardNumber();
			REF->setUpGoldExplored(JSQUEST->getDailyRewardNumber());
			_pIconSprite = Sprite::create("UI/UI_main_menu/ItemBoard/icon_money_small.png");
		}
		if (JSQUEST->getDailyRewardType().compare("play_turn") == 0) {
			m_nLifeNumber += JSQUEST->getDailyRewardNumber();
			REF->setUpLife(JSQUEST->getDailyRewardNumber());
			_pIconSprite = Sprite::create("UI/UI_main_menu/TopMenu/icon_life.png");
		}
		if (JSQUEST->getDailyRewardType().compare("item_bird") == 0) {
			m_arNumberItemOwning[1] += JSQUEST->getDailyRewardNumber();
			REF->increaseNumberItemBird();
			m_arBuyItemButton[1]->setVisible(false);
			m_arSpriteItemMax[1]->setVisible(true);
			_pIconSprite = Sprite::create("UI/UI_main_menu/ItemBoard/item2_bird.png");
		}
		if (JSQUEST->getDailyRewardType().compare("diamond") == 0) {
			m_nCurrentDiamond += JSQUEST->getDailyRewardNumber();
			REF->setUpDiamondBuy(JSQUEST->getDailyRewardNumber());
			_pIconSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
		}
		REF->increaseDailyRewardCounter();
		REF->updateDailyRewardAvailable(false);

		_pIconSprite->setScale(_pConfirmBackground->getContentSize().height / _pIconSprite->getContentSize().height * 0.18f);
		_pIconSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pConfirmBackground->addChild(_pIconSprite, 1);

		Label *_pLabelNumber = Label::createWithBMFont("fontsDPM/font_normal-export.fnt", StringUtils::format(" x %d", JSQUEST->getDailyRewardNumber()));
		_pLabelNumber->setBMFontSize(_pConfirmBackground->getContentSize().height * 0.15f);
		_pLabelNumber->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pConfirmBackground->addChild(_pLabelNumber, 1);

		_pIconSprite->setPosition(Vec2((_pConfirmBackground->getContentSize().width - _pIconSprite->getContentSize().width - _pLabelNumber->getContentSize().width) * 0.5f,
			_pConfirmBackground->getContentSize().height * 0.4f));
		_pLabelNumber->setPosition(Vec2(_pIconSprite->getPosition().x + _pIconSprite->getContentSize().width * _pIconSprite->getScaleX(),
			_pConfirmBackground->getContentSize().height * 0.43f));

		Label *_pLabelTip = Label::createWithTTF("Tap to continue", "fonts/arial.ttf", 32);
		_pLabelTip->setColor(Color3B(144, 32, 12));
		_pLabelTip->setScale(_pConfirmBackground->getContentSize().width / _pLabelTip->getContentSize().width * 0.35f);
		_pLabelTip->setAnchorPoint(Vec2(0.5f, 0.0f));
		_pLabelTip->setPosition(Vec2(_pConfirmBackground->getContentSize().width * 0.5f, _pConfirmBackground->getContentSize().height * 0.2f));
		_pConfirmBackground->addChild(_pLabelTip, 1);

		/*auto actionSpawn = CallFunc::create([&]() {
		auto particlePaper = ParticleSystemQuad::create("Effect/firework_paper.plist");
		particlePaper->setDuration(0.2f);
		particlePaper->setPosition(CCRANDOM_0_1() * m_pBuyPackConfirmBackground->getContentSize().width,
		m_pBuyPackConfirmBackground->getContentSize().height - CCRANDOM_0_1() * m_pBuyPackConfirmBackground->getContentSize().height * 0.5f);
		m_pBuyPackConfirmBackground->addChild(particlePaper, 10);

		auto particleStar = ParticleSystemQuad::create("Effect/firework_star.plist");
		particleStar->setDuration(0.1f);
		particleStar->setPosition(particlePaper->getPosition());
		m_pBuyPackConfirmBackground->addChild(particleStar, 2);
		});

		auto seq = Sequence::create(actionSpawn, DelayTime::create(CCRANDOM_0_1() + 0.75f), nullptr);

		auto forever = RepeatForever::create(seq);
		m_pBuyPackConfirmBackground->runAction(forever);*/

		// button tap to continue
		auto _pCloseNormal = Sprite::create("UI/UI_main_menu/DailyReward/recevive_on.png");
		auto _pCloseSelected = Sprite::create("UI/UI_main_menu/DailyReward/recevive_on.png");
		_pCloseNormal->setColor(Color3B(128, 128, 128));
		auto _aCloseButton = MenuItemSprite::create(_pCloseNormal, _pCloseSelected, CC_CALLBACK_0(MenuLayer::buttonConfirmDailyRewardHandle, this));
		_aCloseButton->setScaleX(m_szVisibleSize.width / _aCloseButton->getContentSize().width);
		_aCloseButton->setScaleY(m_szVisibleSize.height / _aCloseButton->getContentSize().height);
		_aCloseButton->setAnchorPoint(Vec2(0.0f, 0.0f));
		_aCloseButton->setPosition(0.0f, 0.0f);
		_aCloseButton->setOpacity(0.0f);

		Menu *_pConfirmMenu = Menu::create(_aCloseButton, NULL);
		_pConfirmMenu->setContentSize(Size(_pConfirmBackground->getContentSize().width, _pConfirmBackground->getContentSize().height));
		_pConfirmMenu->setPosition(0.0f, 0.0f);
		m_pBuyPackConfirmBackground->addChild(_pConfirmMenu, 2);
	}), nullptr));

	auto actionSpawn = CallFunc::create([&]() {
		auto _aParticleFeather = ParticleSystemQuad::create("Effect/firework_paper.plist");
		_aParticleFeather->setDuration(0.2f);
		_aParticleFeather->setScale(0.5f);
		_aParticleFeather->setPosition(Vec2(m_szVisibleSize.width * CCRANDOM_0_1(), m_szVisibleSize.height * CCRANDOM_0_1()));
		m_pBuyPackConfirmBackground->addChild(_aParticleFeather, 2);
		auto _aParticleFeather1 = ParticleSystemQuad::create("Effect/firework_star.plist");
		_aParticleFeather1->setDuration(0.1f);
		_aParticleFeather1->setScale(0.5f);
		_aParticleFeather1->setPosition(_aParticleFeather->getPosition());
		m_pBuyPackConfirmBackground->addChild(_aParticleFeather1, 2);
	});
	Sequence *_pBlink = Sequence::create(actionSpawn, DelayTime::create(1.5f), NULL);
	m_pBuyPackConfirmBackground->runAction(RepeatForever::create(_pBlink));
}

void MenuLayer::buttonConfirmDailyRewardHandle() {
	initTopMainMenu();
	initItemBoard();
	hideBlurScreen();
	m_pBuyPackConfirmBackground->setVisible(false);
	m_pBuyPackConfirmBackground->removeAllChildrenWithCleanup(true);
}

void MenuLayer::buttonBuyLifeHandle(int p_nIndexEnergyPack) {
	AudioManager::playSound(SOUND_BTCLICK);
	JSMENU->readEnergyPack(p_nIndexEnergyPack);
	if (JSMENU->getEnergyPackDiamondPrice() <= m_nCurrentDiamond) {
		//GAHelper::getInstance()->logEvent("buylife", "click", "UpgradeQuachtinh", 1);
		logBuyLife(p_nIndexEnergyPack);
		m_nCurrentDiamond -= JSMENU->getEnergyPackDiamondPrice();
		m_nLifeNumber += JSMENU->getEnergyPackNumberEnergy();
		REF->setDownDiamond(JSMENU->getEnergyPackDiamondPrice());
		REF->setUpLife(JSMENU->getEnergyPackNumberEnergy());
		initTopMainMenu();
		m_pTopMenu->setEnabled(false);
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(9), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonBuyCoinHandle(int p_nIndexCoinPack) {
	AudioManager::playSound(SOUND_BTCLICK);
	JSMENU->readGoldPack(p_nIndexCoinPack);
	if (JSMENU->getCoinPackDiamondPrice() <= m_nCurrentDiamond) {
		logBuyCoin(p_nIndexCoinPack);
		m_nCurrentDiamond -= JSMENU->getCoinPackDiamondPrice();
		m_nCurrentGold += JSMENU->getCoinPackNumberGold();
		REF->setDownDiamond(JSMENU->getCoinPackDiamondPrice());
		REF->setUpGoldExplored(JSMENU->getCoinPackNumberGold());
		initTopMainMenu();
		m_pTopMenu->setEnabled(false);
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(9), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonBuyDiamondHandle(int p_nIndexDiamondPack) {
	AudioManager::playSound(SOUND_BTCLICK);
	JSMENU->readDiamondPack(p_nIndexDiamondPack);
	/*if (false) {
	return;
	}*/
	/*m_nCurrentDiamond += JSMENU->getDiamondPackNumberDiamond();
	REF->setUpDiamondBuy(JSMENU->getDiamondPackNumberDiamond());
	initTopMainMenu();
	m_pTopMenu->setEnabled(false);*/

	//REF->setUpNumberQuest(7, JSMENU->getDiamondPackNumberDiamond());
	//initQuestBoard(0);
	switch (p_nIndexDiamondPack)
	{
	case 0: {
				IAPHelper::getInstance()->purchase("diamond_1");
	}
	case 1: {
				IAPHelper::getInstance()->purchase("diamond_2");
	}
	case 2: {
				IAPHelper::getInstance()->purchase("diamond_3");
	}
	case 3: {
				IAPHelper::getInstance()->purchase("diamond_4");
	}
	case 4: {
				IAPHelper::getInstance()->purchase("diamond_5");
	}
	default:
		break;
	}
}

void MenuLayer::buttonPickHeroHandle(int p_nIndexHero) {
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nIndexHeroPicked == p_nIndexHero) {
		return;
	}
	m_nIndexHeroPicked = p_nIndexHero;
	logClickHeroEvent(m_nIndexHeroPicked);
	REF->pointToCurrentHero(m_nIndexHeroPicked);
	if (!REF->getIsLockedHero()) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
		REF->setSelectedHero(m_nIndexHeroPicked);
		REF->setLastPickHero(m_nIndexHeroSelected);
	}
	initSceneLayer();
	initBackgroundLayer();
	initBottomHeroMenu();
	initHeroInfoBoard();
	initUpgradeBoard();
}

void MenuLayer::buttonTryHeroHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	auto _scene = LoadingLayer::createScene(1, 2, m_nIndexHeroPicked);
	logTryHeroEvent(m_nIndexHeroPicked);
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, _scene));
}

void MenuLayer::buttonUnlockHeroHandle() {
	AudioManager::playSound(SOUND_BTCLICK);

	REF->pointToCurrentHero(m_nIndexHeroPicked - 1); // point to pre-hero
	int _nPreHeroLevel = REF->getCurrentLevel(); // get level of pre-hero
	REF->pointToCurrentHero(m_nIndexHeroPicked);
	int _arLevelToUnlock[] = { 0, 10, 10, 15, 15 };
	if (_nPreHeroLevel < _arLevelToUnlock[m_nIndexHeroPicked]) {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getTipAtX(11 + m_nIndexHeroPicked), TOAST_SHORT);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);

		return;
	}
	if (m_nCurrentGold >= JSHERO->getGoldPrice() && m_nCurrentDiamond >= JSHERO->getDiamondPrice()) {

		AudioManager::playSound(SOUND_UNLOCK_HERO);
		logUnlockHeroEvent(m_nIndexHeroPicked);
		auto _aAnimSprite = Sprite::create("UI/UI_main_menu/HeroMenu/effect_unlock_2.png");
		auto _aCache = AnimationCache::getInstance();
		Vector<SpriteFrame*> _arFrames(2);
		_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/HeroMenu/effect_unlock_2.png",
			Rect(0, 0, _aAnimSprite->getContentSize().width, _aAnimSprite->getContentSize().height)));
		_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/HeroMenu/effect_unlock_1.png",
			Rect(0, 0, _aAnimSprite->getContentSize().width, _aAnimSprite->getContentSize().height)));
		_aCache->addAnimation(Animation::createWithSpriteFrames(_arFrames, 0.1f), "bum");
		_aAnimSprite->runAction(Sequence::create(Animate::create(_aCache->getAnimation("bum")), NULL));
		_aAnimSprite->setPosition(m_arHeroButton[m_nIndexHeroPicked]->getContentSize().width / 2,
			m_arHeroButton[m_nIndexHeroPicked]->getContentSize().height / 2);
		m_arHeroButton[m_nIndexHeroPicked]->addChild(_aAnimSprite, 1);

		m_nCurrentGold -= JSHERO->getGoldPrice();
		m_nCurrentDiamond -= JSHERO->getDiamondPrice();
		initTopMainMenu();
		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() { initBottomHeroMenu(); }), nullptr));
		// save data
		REF->unLockHero(m_nIndexHeroPicked);
		REF->setDownGold(JSHERO->getGoldPrice());
		REF->setDownDiamond(JSHERO->getDiamondPrice());
		initBottomHeroMenu();
		initUpgradeBoard();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(5), TOAST_SHORT);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonUpgradeHeroHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	AudioManager::playSound(SOUND_UPGRADE_LEVEL);
	float _nUpgradeCost = JSHERO->getGoldUpgradeLevelX(m_nIndexHeroPicked);

	int level = REF->getCurrentLevel();

	for (int i = 1; i < level; i++) {
		_nUpgradeCost *= 1.05f;
	}

	if (m_nCurrentGold >= _nUpgradeCost) {
		logUpgradeHeroEvent(m_nIndexHeroPicked, level + 1);
		m_nCurrentGold -= (int)_nUpgradeCost;
		REF->setDownGold(_nUpgradeCost);
		REF->increaseLevel();
		initTopMainMenu();
		initBottomHeroMenu();
		initHeroInfoBoard();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(8), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	}
}

void MenuLayer::createLayerViaInput(Layer *p_pLayer, Size p_v2Size, Vec2 p_v2Position) {
	p_pLayer = Layer::create();
	p_pLayer->setContentSize(p_v2Size);
	p_pLayer->setPosition(p_v2Position);
	m_pGameControl->addChild(p_pLayer, 1);
}

void MenuLayer::scrollSlideHandle(Ref* sender, ScrollView::EventType type) {
	if (type == ScrollView::EventType::SCROLLING) {
		static int a = 20;
		if (a < 20) {
			a++;
		}
		else {
			AudioManager::playSound(SOUND_SCROLL_SLIDE);
			a = 0;
		}
	}
}

void MenuLayer::scrollShopHandle(Ref* sender, ScrollView::EventType type) {
	if (type == ScrollView::EventType::SCROLLING) {
		int _nSelectedIndex = m_pPacksZone->getCurSelectedIndex();
		auto _aTempObject = m_pPacksZone->getChildren().at(_nSelectedIndex);
		float _fBigScale = m_pPacksZone->getContentSize().height / m_pPacksZone->getChildren().at(_nSelectedIndex)->getContentSize().height;
		_aTempObject->runAction(ScaleTo::create(0.05f, _fBigScale));

		static int a = 20;
		if (a < 20) {
			a++;
		}
		else {
			AudioManager::playSound(SOUND_SCROLL_SLIDE);
			a = 0;
		}
	}
}

void MenuLayer::selectedItemEvent(Ref* sender, ListView::EventType type) {
	int _nSelectedIndex = m_pPacksZone->getCurSelectedIndex();
	auto _aTempObject = m_pPacksZone->getChildren().at(_nSelectedIndex);
	float _fBigScale = m_pPacksZone->getContentSize().height / m_pPacksZone->getChildren().at(0)->getContentSize().height;
	float _fSmallScale = m_pPacksZone->getContentSize().height / m_pPacksZone->getChildren().at(0)->getContentSize().height * 0.9f;

	if (type == ListView::EventType::ON_SELECTED_ITEM_START) {
		_aTempObject->runAction(ScaleTo::create(0.05f, _fSmallScale));
	}
	if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
		_aTempObject->runAction(ScaleTo::create(0.05f, _fBigScale));
		if (m_nShopOption == 1) {
			buttonBuyDiamondHandle(_nSelectedIndex);
		}
		else {
			JSMENU->readGoldPack(_nSelectedIndex);
			JSMENU->readEnergyPack(_nSelectedIndex);
			if (m_nShopOption == 0 && JSMENU->getCoinPackDiamondPrice() > m_nCurrentDiamond
				|| m_nShopOption == 2 && JSMENU->getEnergyPackDiamondPrice() > m_nCurrentDiamond) {
				CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(9), TOAST_LONG);
				_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
				this->addChild(_pToast, 10);

				return;
			}

			m_pBuyPackConfirmBackground->setVisible(true);
			m_pPacksZone->setTouchEnabled(false);
			m_pShopMenu->setEnabled(false);

			Sprite *_pConfirmBackground = Sprite::create("UI/UI_main_menu/ShopBoard/confirm_dialog.png");
			_pConfirmBackground->setScale(m_pBuyPackConfirmBackground->getContentSize().height / _pConfirmBackground->getContentSize().height * 0.6f);
			_pConfirmBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
			_pConfirmBackground->setPosition(m_pBuyPackConfirmBackground->getContentSize().width * 0.5f, m_pBuyPackConfirmBackground->getContentSize().height * 0.5f);
			m_pBuyPackConfirmBackground->addChild(_pConfirmBackground, 1);

			Label *_pLabelConfirm = Label::createWithTTF(StringUtils::format("%s", JSHERO->getNotifyAtX(10).c_str()), "fontsDPM/UTM_BRUSHSCI.ttf", _pConfirmBackground->getContentSize().height * 0.3f);
			_pLabelConfirm->setColor(Color3B::BLACK);
			_pLabelConfirm->setScale(_pConfirmBackground->getContentSize().width / _pLabelConfirm->getContentSize().width * 0.7f);
			_pLabelConfirm->setAnchorPoint(Vec2(0.5f, 0.0f));
			_pLabelConfirm->setPosition(Vec2(_pConfirmBackground->getContentSize().width * 0.5f,
				_pConfirmBackground->getContentSize().height * 0.6f));
			_pConfirmBackground->addChild(_pLabelConfirm, 1);

			Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/ShopBoard/icon_diamond.png");
			_pDiamondSprite->setScale(_pConfirmBackground->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.1f);
			_pDiamondSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
			_pDiamondSprite->setPosition(Vec2(_pConfirmBackground->getContentSize().width * 0.49f, _pConfirmBackground->getContentSize().height * 0.5f));
			_pConfirmBackground->addChild(_pDiamondSprite, 1);

			int _nTempPrice = 0;
			if (m_nShopOption == 0) {
				JSMENU->readGoldPack(_nSelectedIndex);
				_nTempPrice = JSMENU->getCoinPackDiamondPrice();
			}
			if (m_nShopOption == 2) {
				JSMENU->readEnergyPack(_nSelectedIndex);
				_nTempPrice = JSMENU->getEnergyPackDiamondPrice();
			}

			Label *_pLabelDiamondCost = Label::createWithBMFont("fontsDPM/font_diamond-export.fnt", StringUtils::format("%d", _nTempPrice));
			_pLabelDiamondCost->setBMFontSize(_pConfirmBackground->getContentSize().height * 0.11f);
			_pLabelDiamondCost->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pLabelDiamondCost->setPosition(Vec2(_pConfirmBackground->getContentSize().width * 0.5f, _pConfirmBackground->getContentSize().height * 0.5f));
			_pConfirmBackground->addChild(_pLabelDiamondCost, 1);

			auto _pConfirmNormal = Sprite::create("UI/UI_main_menu/ShopBoard/btn_ok.png");
			auto _pConfirmSelected = Sprite::create("UI/UI_main_menu/ShopBoard/btn_ok.png");
			_pConfirmSelected->setColor(Color3B(128, 128, 128));
			auto _aConfirmButton = MenuItemSprite::create(_pConfirmNormal, _pConfirmSelected, CC_CALLBACK_0(MenuLayer::buttonConfirmHandle, this, true, _nSelectedIndex));
			_aConfirmButton->setScale(_pConfirmBackground->getContentSize().height / _aConfirmButton->getContentSize().height * 0.3f);
			_aConfirmButton->setAnchorPoint(Vec2(0.5f, 0.5f));
			_aConfirmButton->setPosition(_pConfirmBackground->getContentSize().width * 0.5f, _pConfirmBackground->getContentSize().height * 0.3f);

			// button close shop
			auto _pCloseNormal = Sprite::create("UI/UI_main_menu/ShopBoard/btn_close_shop.png");
			auto _pCloseSelected = Sprite::create("UI/UI_main_menu/ShopBoard/btn_close_shop.png");
			_pCloseSelected->setColor(Color3B(128, 128, 128));
			auto _aCloseButton = MenuItemSprite::create(_pCloseNormal, _pCloseSelected, CC_CALLBACK_0(MenuLayer::buttonConfirmHandle, this, false, _nSelectedIndex));
			_aCloseButton->setScale(_pConfirmBackground->getContentSize().height / _aCloseButton->getContentSize().height * 0.15f);
			_aCloseButton->setAnchorPoint(Vec2(1.0f, 1.0f));
			_aCloseButton->setPosition(_pConfirmBackground->getContentSize().width * 0.92f, _pConfirmBackground->getContentSize().height * 0.92f);

			Menu *_pConfirmMenu = Menu::create(_aConfirmButton, _aCloseButton, NULL);
			_pConfirmMenu->setContentSize(Size(_pConfirmBackground->getContentSize().width, _pConfirmBackground->getContentSize().height));
			_pConfirmMenu->setPosition(0.0f, 0.0f);
			_pConfirmBackground->addChild(_pConfirmMenu, 2);
		}
	}
}

void MenuLayer::buttonConfirmHandle(bool p_bConfirm, int p_nIndexPack) {
	AudioManager::playSound(SOUND_BTCLICK);
	if (p_bConfirm == true) {
		if (m_nShopOption == 0) {
			buttonBuyCoinHandle(p_nIndexPack);
		}
		if (m_nShopOption == 2) {
			buttonBuyLifeHandle(p_nIndexPack);
		}
	}
	m_pBuyPackConfirmBackground->setVisible(false);
	m_pPacksZone->setTouchEnabled(true);
	m_pShopMenu->setEnabled(true);
	m_pShopMenu->setEnabled(true);
}

void MenuLayer::onChangedLanguage() {
	REF->setLanguage(m_pLanguageButtonGroup->getSelectedButtonIndex() == 0 ? 1 : 0);
	m_nLanguage = REF->getLanguage();
	initQuestBoard(0);
	initItemBoard();
	initUpgradeBoard();
	initHeroInfoBoard();
}

void MenuLayer::buttonSoundControlHandle(Ref* p_pSender) {
	auto _aToggleItem = dynamic_cast<MenuItemToggle*>(p_pSender);
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	if (_aToggleItem->getSelectedIndex() == 0) { // turn sound on
		ref->setBoolForKey(KEY_IS_SOUND, true);
	}
	else if (_aToggleItem->getSelectedIndex() == 1) { // turn sound off
		ref->setBoolForKey(KEY_IS_SOUND, false);
	}
	AudioManager::playSound(SOUND_BTCLICK);
}

void MenuLayer::buttonMusicControlHandle(Ref* p_pSender) {
	auto _aToggleItem = dynamic_cast<MenuItemToggle*>(p_pSender);
	auto ref = UserDefault::getInstance()->sharedUserDefault();
	if (_aToggleItem->getSelectedIndex() == 0) { // turn sound on
		ref->setBoolForKey(KEY_IS_MUSIC, true);
		AudioManager::playMusic(MUSIC_MENU);
	}
	else if (_aToggleItem->getSelectedIndex() == 1) { // turn sound off
		ref->setBoolForKey(KEY_IS_MUSIC, false);
		AudioManager::stopMusic();
	}
	AudioManager::playSound(SOUND_BTCLICK);
}

void MenuLayer::logButtonClickEvent(string button)
{
	GAHelper::getInstance()->logEvent("Button", "Click", button, 1);
}

void MenuLayer::logBuyItemEvent(string item)
{
	GAHelper::getInstance()->logEvent("Item", "buy", item, 1);
}

void MenuLayer::logUnlockHeroEvent(int indexhero)
{
	GAHelper::getInstance()->logEvent("Hero", "Unlock", indexHeroToName(indexhero), 1);
}

void MenuLayer::logClickHeroEvent(int index)
{

	GAHelper::getInstance()->logEvent("Hero", "Click", indexHeroToName(index), 1);
}

void MenuLayer::logTryHeroEvent(int indexhero)
{
	GAHelper::getInstance()->logEvent("Hero", "Try", indexHeroToName(indexhero), 1);
}

void MenuLayer::logUpgradeHeroEvent(int  indexhero, int level)
{
	GAHelper::getInstance()->logEvent("Hero", StringUtils::format("Upgrade level: %d", level), indexHeroToName(indexhero), 1);
}

void MenuLayer::logUpgradeSkillEvent(int indexhero, int indexskill, int level)
{
	GAHelper::getInstance()->logEvent("UpgradeSkill", indexHeroToName(indexhero) + StringUtils::format(" skill %d", indexskill), StringUtils::format("level %d", level), 1);
}


void MenuLayer::logShowMoreCoin()
{
	GAHelper::getInstance()->logEvent("Coin", "Show", "", 1);
}

void MenuLayer::logShowMoreDiamond()
{
	GAHelper::getInstance()->logEvent("Diamond", "Show", "", 1);
}

void MenuLayer::logShowMoreLife()
{
	GAHelper::getInstance()->logEvent("Life", "Show", "", 1);
}

void MenuLayer::logBuyCoin(int dexOfPack)
{
	GAHelper::getInstance()->logEvent("Coin", "Buy", StringUtils::format("Pack %d", dexOfPack + 1), 1);
}

void MenuLayer::logBuyLife(int dexOfPack)
{
	GAHelper::getInstance()->logEvent("Life", "Buy", StringUtils::format("Pack %d", dexOfPack + 1), 1);
}

void MenuLayer::logBuyDiamond(int dexOfPack, float money)
{
	GAHelper::getInstance()->logEvent("Diamond", "Buy", StringUtils::format("Pack %d", dexOfPack + 1), money);
}

#ifdef SDKBOX_ENABLED


void MenuLayer::onInitialized(bool ok)
{
}

void MenuLayer::onSuccess(sdkbox::Product const & p)
{
	int p_nIndexDiamondPack;
	if (p.name == "diamond_1") {
		p_nIndexDiamondPack = 0;
	}
	else if (p.name == "diamond_2") {
		p_nIndexDiamondPack = 1;

	}
	else if (p.name == "diamond_3") {
		p_nIndexDiamondPack = 2;

	}
	else if (p.name == "diamond_4") {
		p_nIndexDiamondPack = 3;

	}
	else if (p.name == "diamond_5") {
		p_nIndexDiamondPack = 4;
	}
	JSMENU->readDiamondPack(p_nIndexDiamondPack);
	if (false) {
		return;
	}
	logBuyDiamond(p_nIndexDiamondPack, p.priceValue);
	m_nCurrentDiamond += JSMENU->getDiamondPackNumberDiamond();
	REF->setUpDiamondBuy(JSMENU->getDiamondPackNumberDiamond());
	initTopMainMenu();
	m_pTopMenu->setEnabled(false);

	REF->setUpNumberQuest(7, JSMENU->getDiamondPackNumberDiamond());
}

void MenuLayer::onFailure(sdkbox::Product const & p, const std::string & msg)
{
}

void MenuLayer::onCanceled(sdkbox::Product const & p)
{
}

void MenuLayer::onRestored(sdkbox::Product const & p)
{
}

void MenuLayer::onProductRequestSuccess(std::vector<sdkbox::Product> const & products)
{
}

void MenuLayer::onProductRequestFailure(const std::string & msg)
{
}

void MenuLayer::onRestoreComplete(bool ok, const std::string & msg)
{
}

void MenuLayer::onVungleAdViewed(bool isComplete)
{
}

void MenuLayer::onVungleCacheAvailable()
{
}

void MenuLayer::onVungleStarted()
{
}

void MenuLayer::onVungleFinished()
{
}

void MenuLayer::onVungleAdReward(const std::string & name)
{
	REF->decreaseFreeCoin();
	if (REF->getFreeCoin() <= 0) {
		m_pSpriteFreeCoinAttention->setVisible(false);
	}
	if (REF->getNumberOfLife() == 0) {
		m_nLifeNumber += 5;
		REF->setUpLife(5);
		initTopMainMenu();
		m_pTopMenu->setEnabled(true);
		actionToast(13, 5);
	}
	else {
		float type = CCRANDOM_0_1();
		float percent = CCRANDOM_0_1();
		if (type >= 0.5f) {
			if (percent < 0.5f) {
				m_nCurrentGold += 300;
				REF->setUpGoldExplored(300);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);

				actionToast(14, 300);
			}
			else if (percent < 0.85f) {
				m_nCurrentGold += 400;
				REF->setUpGoldExplored(400);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);
				actionToast(14, 400);
			}
			else {
				m_nCurrentGold += 500;
				REF->setUpGoldExplored(500);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);
				actionToast(14, 500);
			}
		}
		else {
			if (percent < 0.5f) {
				m_nLifeNumber += 3;
				REF->setUpLife(3);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);
				
				actionToast(13, 3);
			}
			else if (percent < 0.85f) {
				m_nLifeNumber += 4;
				REF->setUpLife(4);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);

				actionToast(13, 4);
			}
			else {
				m_nLifeNumber += 5;
				REF->setUpLife(5);
				initTopMainMenu();
				m_pTopMenu->setEnabled(true);

				actionToast(13, 5);
			}
		}
	}

}

#endif 


string MenuLayer::indexHeroToName(int indexHero)
{
	//string namehero;
	switch (indexHero)
	{
	case 0: {
				return "DuongQua";
				break;
	}
	case 1: {
				return "CoLong";
				break;
	}
	case 2: {
				return "HoangDung";
				break;
	}
	case 3: {
				return "HoangDuocSu";
				break;
	}
	case 4: {
				return "QuachTinh";
				break;
	}
	default:
		return "DuongQua";
		break;
	}
}

void MenuLayer::singlePress(float dt)
{
	this->unschedule(schedule_selector(MenuLayer::singlePress));
	backNumber = 0;
}

void MenuLayer::actionToast(int index, int value)
{
	auto action = CallFunc::create([=]() {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(StringUtils::format(JSHERO->getNotifyAtX(index).c_str(), value), TOAST_SHORT);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		this->addChild(_pToast, 10);
	});
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), action));
}

void MenuLayer::loadTwinkle(TMXTiledMap *p_pTMXTiledMap, float p_fMinScaleViaHeight, float p_fMaxScaleViaHeight) {
	for (auto _aTwinkle : p_pTMXTiledMap->getObjectGroup("Twinkle")->getObjects()) {
		auto _aObjectTwinkle = _aTwinkle.asValueMap();
		Point _ptPositionTwinkle = Point(_aObjectTwinkle["x"].asFloat(), _aObjectTwinkle["y"].asFloat());

		Twinkle *_pTwinkle = Twinkle::create();
		_pTwinkle->setScale(p_pTMXTiledMap->getContentSize().height / _pTwinkle->getContentSize().height * (p_fMinScaleViaHeight + CCRANDOM_0_1() * (p_fMaxScaleViaHeight - p_fMinScaleViaHeight)));
		_pTwinkle->setPosition(Vec2(_ptPositionTwinkle.x, _ptPositionTwinkle.y));
		p_pTMXTiledMap->addChild(_pTwinkle, 1);
	}
}

void MenuLayer::buttonSpringy(MenuItemSprite *p_pButton) {
	ScaleBy *_pZoomOut = ScaleBy::create(0.5f, 0.9f);
	Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
	RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
	p_pButton->runAction(_pZoomRepeat);
}

void MenuLayer::moveLayerViaDirection(Layer *p_pLayer, int p_nDirection) {
	switch (p_nDirection) {
		case 2:
			p_pLayer->runAction(MoveBy::create(0.2f, Vec2(0.0f, -p_pLayer->getContentSize().height)));
			break;
		case 4:
			p_pLayer->runAction(MoveBy::create(0.2f, Vec2(-p_pLayer->getContentSize().width, 0.0f)));
			break;
		case 6:
			p_pLayer->runAction(MoveBy::create(0.2f, Vec2(p_pLayer->getContentSize().width, 0.0f)));
			break;
		case 8:
			p_pLayer->runAction(MoveBy::create(0.2f, Vec2(0.0f, p_pLayer->getContentSize().height)));
			break;
		default:
			break;
	}
}

Sprite * MenuLayer::createSpriteOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sPath,
	float p_fScaleX, float p_fScaleY, bool p_bScaleByWidth, Vec2 p_v2Anchor, Vec2 p_v2Position)
{
	Sprite *_pChild = Sprite::create(p_sPath);
	_pChild->setAnchorPoint(p_v2Anchor);
	_pChild->setPosition(p_v2Position);
	if (p_pLayerParent != NULL) {
		_pChild->setScaleX(p_pLayerParent->getContentSize().width / _pChild->getContentSize().width * p_fScaleX);
		_pChild->setScaleY(p_pLayerParent->getContentSize().height / _pChild->getContentSize().height * p_fScaleY);
		if (p_fScaleX == p_fScaleY) {
			if (p_bScaleByWidth == true) {
				_pChild->setScale(p_pLayerParent->getContentSize().width / _pChild->getContentSize().width * p_fScaleX);
			}
			else {
				_pChild->setScale(p_pLayerParent->getContentSize().height / _pChild->getContentSize().height * p_fScaleY);
			}
		}
		p_pLayerParent->addChild(_pChild, p_nLayer);
	}
	else {
		_pChild->setScaleX(p_pSpriteParent->getContentSize().width / _pChild->getContentSize().width * p_fScaleX);
		_pChild->setScaleY(p_pSpriteParent->getContentSize().height / _pChild->getContentSize().height * p_fScaleY);
		if (p_fScaleX == p_fScaleY) {
			if (p_bScaleByWidth == true) {
				_pChild->setScale(p_pSpriteParent->getContentSize().width / _pChild->getContentSize().width * p_fScaleX);
			}
			else {
				_pChild->setScale(p_pSpriteParent->getContentSize().height / _pChild->getContentSize().height * p_fScaleY);
			}
		}
		p_pSpriteParent->addChild(_pChild, p_nLayer);
	}

	return _pChild;
}

MenuItemSprite * MenuLayer::createButtonOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, string p_sPath, ccMenuCallback p_pCallback,
	float p_fScaleX, float p_fScaleY, bool p_bScaleByWidth, Vec2 p_v2Anchor, Vec2 p_v2Position)
{
	auto _aButtonNormal = Sprite::create(p_sPath);
	auto _aButtonSelected = Sprite::create(p_sPath);
	_aButtonSelected->setColor(Color3B(128, 128, 128));
	MenuItemSprite *_pButton = MenuItemSprite::create(_aButtonNormal, _aButtonSelected, p_pCallback);

	if (p_pLayerParent != NULL) {
		_pButton->setScaleX(p_pLayerParent->getContentSize().width / _pButton->getContentSize().width * p_fScaleX);
		_pButton->setScaleY(p_pLayerParent->getContentSize().height / _pButton->getContentSize().height * p_fScaleY);
		if (p_fScaleX == p_fScaleY) {
			if (p_bScaleByWidth == true) {
				_pButton->setScale(p_pLayerParent->getContentSize().width / _pButton->getContentSize().width * p_fScaleX);
			}
			else {
				_pButton->setScale(p_pLayerParent->getContentSize().height / _pButton->getContentSize().height * p_fScaleY);
			}
		}
	}
	else {
		_pButton->setScaleX(p_pSpriteParent->getContentSize().width / _pButton->getContentSize().width * p_fScaleX);
		_pButton->setScaleY(p_pSpriteParent->getContentSize().height / _pButton->getContentSize().height * p_fScaleY);
		if (p_fScaleX == p_fScaleY) {
			if (p_bScaleByWidth == true) {
				_pButton->setScale(p_pSpriteParent->getContentSize().width / _pButton->getContentSize().width * p_fScaleX);
			}
			else {
				_pButton->setScale(p_pSpriteParent->getContentSize().height / _pButton->getContentSize().height * p_fScaleY);
			}
		}
	}
	_pButton->setAnchorPoint(p_v2Anchor);
	_pButton->setPosition(p_v2Position);

	return _pButton;
}

Label * MenuLayer::createLabelBMOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sFontName,
	string p_sText, float p_fFontSize, TextHAlignment p_pHAlignment, TextVAlignment p_pVAlignment, Vec2 p_v2Anchor,
	Vec2 p_v2Position)
{
	Label *_pLabel = Label::createWithBMFont(p_sFontName, p_sText);
	_pLabel->setAlignment(p_pHAlignment, p_pVAlignment);
	_pLabel->setAnchorPoint(p_v2Anchor);
	_pLabel->setPosition(p_v2Position);
	if (p_pLayerParent != NULL) {
		_pLabel->setBMFontSize(p_pLayerParent->getContentSize().height * p_fFontSize);
		p_pLayerParent->addChild(_pLabel, p_nLayer);
	}
	else {
		_pLabel->setBMFontSize(p_pSpriteParent->getContentSize().height * p_fFontSize);
		p_pSpriteParent->addChild(_pLabel, p_nLayer);
	}

	return _pLabel;
}

Label * MenuLayer::createLabelTTFOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sFontName,
	string p_sText, float p_fScale, bool p_bScaleByWidth, TextHAlignment p_pHAlignment, TextVAlignment p_pVAlignment,
	Vec2 p_v2Anchor, Vec2 p_v2Position, Color3B p_b3Color)
{
	Label *_pLabel = Label::createWithTTF(p_sText, p_sFontName, 48);
	_pLabel->setColor(p_b3Color);
	_pLabel->setAlignment(p_pHAlignment, p_pVAlignment);
	_pLabel->setAnchorPoint(p_v2Anchor);
	_pLabel->setPosition(p_v2Position);
	if (p_pLayerParent != NULL) {
		if (p_bScaleByWidth == true) {
			_pLabel->setScale(p_pLayerParent->getContentSize().width / _pLabel->getContentSize().width * p_fScale);
		}
		else {
			_pLabel->setScale(p_pLayerParent->getContentSize().height / _pLabel->getContentSize().height * p_fScale);
		}
		p_pLayerParent->addChild(_pLabel, 1);
	}
	else {
		if (p_bScaleByWidth == true) {
			_pLabel->setScale(p_pSpriteParent->getContentSize().width / _pLabel->getContentSize().width * p_fScale);
		}
		else {
			_pLabel->setScale(p_pSpriteParent->getContentSize().height / _pLabel->getContentSize().height * p_fScale);
		}
		p_pSpriteParent->addChild(_pLabel, 1);
	}

	return _pLabel;
}
