#include "MenuScene.h"
#include "LoadingLayer.h"
#include "SelectStageScene.h"
#include "IntroScene.h"

#include "manager/JSonHeroManager.h"
#include "manager/JSonMenuManager.h"
#include "manager/JSonQuestManager.h"
#include "manager/RefManager.h"
#include "ui_custom/CustomSpriteToBuyPack.h"
#include "manager/RefManager.h"

Scene * MenuLayer::createScene() {
	auto scene = Scene::create();
	auto layer = MenuLayer::create();
	scene->addChild(layer);

	return scene;
}

bool MenuLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	initInputData();
	Vec2 _v2Origin = Director::getInstance()->getVisibleOrigin();

	m_pGameBackground = new Layer(); // layer 1 : background
	m_pGameBackground->setPosition(_v2Origin);
	this->addChild(m_pGameBackground, 1);
	initBackgroundLayer();

	m_pGameScene = new Layer(); // layer 2 : scene
	m_pGameScene->setPosition(_v2Origin);
	this->addChild(m_pGameScene, 2);
	float _fScale = m_szVisibleSize.height / 2;
	m_pGameScene->setContentSize(Size(m_szVisibleSize.width * 0.6f, m_szVisibleSize.height * 0.65f));
	m_pGameScene->setPosition(0.0f, m_szVisibleSize.height * 0.2f);
	m_arPreviewHero[0] = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_arPreviewHero[1] = new SkeletonAnimation("UI/UI_main_menu/PreviewCoLong/s_CoCo.json",
		"UI/UI_main_menu/PreviewCoLong/s_CoCo.atlas", m_pGameScene->getContentSize().height / 650);
	m_arPreviewHero[2] = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_arPreviewHero[3] = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_arPreviewHero[4] = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);

	m_arPreviewHero[0]->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_arPreviewHero[1]->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_arPreviewHero[2]->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_arPreviewHero[3]->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_arPreviewHero[4]->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));

	initSceneLayer();

	m_pGameControl = new Layer(); // layer 3 : control
	m_pGameControl->setPosition(_v2Origin);
	this->addChild(m_pGameControl, 3);
	initControlLayer();

	m_pBlurScreen = Layer::create();
	m_pBlurScreen->setScaleX(m_szVisibleSize.width / m_pBlurScreen->getContentSize().width); // full screen size width
	m_pBlurScreen->setScaleY(m_szVisibleSize.height / m_pBlurScreen->getContentSize().height); // full screen size height
	m_pBlurScreen->setPosition(Vec2(0.0f, m_szVisibleSize.height)); // center screen
	this->addChild(m_pBlurScreen, 4);
	m_pBlurScreen->setVisible(false);

	this->scheduleUpdate();

	return true;
}

void MenuLayer::update(float p_fDelta) {
	time_t _nCurrentTime = time(0);
	if (m_nLifeNumber < 5) {
		int _nDeltaTime = _nCurrentTime - m_nAnchorTime;
		if (_nDeltaTime >= 300) {
			m_nAnchorTime = time(0);
			REF->resetAnchorTime();
			m_nLifeNumber += 1;
			REF->setUpLife(1);
			initTopMainMenu();
			return;
		}
		else {
			m_pTimeCounter->setVisible(true);
			int _nMinute = (300 - _nDeltaTime) / 60;
			int _nSecond = (300 - _nDeltaTime) % 60;
			m_pTimeCounter->setString(StringUtils::format(_nSecond < 10 ? "%i:0%i" : "%i:%i", _nMinute, _nSecond));
		}
	}
	else {
		m_pTimeCounter->setVisible(false);
	}
}

void MenuLayer::initInputData() {
	m_nIndexHeroSelected = m_nIndexHeroPicked = REF->getLastPickHero();
	m_nAnchorTime = REF->getAnchorTime();
	m_nLifeNumber = REF->getNumberOfLife();
	m_nCurrentGold = REF->getGoldExplored();
	m_nCurrentDiamond = REF->getDiamondBuy();
	m_arNumberItemOwning[0] = REF->getNumberItemHealth();
	m_arNumberItemOwning[1] = REF->getNumberItemBird();
	m_arNumberItemOwning[2] = REF->getNumberItemMagnet();
	m_arNumberItemOwning[3] = REF->getNumberItemDoubleGold();
	m_arNumberItemOwning[4] = REF->getNumberItemCoolDown();
	// m_arItemPrice will be loaded in initItemBoard, just once
	createRequestToGoogle();
}

void MenuLayer::initBackgroundLayer() {
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
}

void MenuLayer::initSceneLayer() {
	m_pGameScene->removeAllChildrenWithCleanup(true);
	m_arPreviewHero[m_nIndexHeroPicked]->setAnimation(0, "appear", false);
	m_arPreviewHero[m_nIndexHeroPicked]->addAnimation(0, "idle", true);
	m_pGameScene->addChild(m_arPreviewHero[m_nIndexHeroPicked], 1);
}

void MenuLayer::initControlLayer() {
	// top menu
	m_pTopMainMenu = Layer::create();
	m_pTopMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMainMenu->setPosition(m_szVisibleSize.width / 2, m_szVisibleSize.height + m_pTopMainMenu->getContentSize().height / 2);
	m_pGameControl->addChild(m_pTopMainMenu, 1);
	initTopMainMenu();

	// bottom main menu
	m_pBottomMainLayer = Layer::create();
	m_pBottomMainLayer->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 25% screen height
	m_pBottomMainLayer->setPosition(0.0f, -m_pBottomMainLayer->getContentSize().height);
	m_pGameControl->addChild(m_pBottomMainLayer, 1);
	initBottomMainMenu();

	initItemBoard(0.0f);

	// upgrade board
	m_pUpgradeBoard = Layer::create();
	m_pUpgradeBoard->setContentSize(Size(m_szVisibleSize.width * 0.35f, m_szVisibleSize.height * 0.62f)); // 35% screen width, 62% screen height
	m_pUpgradeBoard->setPosition(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f);
	m_pGameControl->addChild(m_pUpgradeBoard, 1);
	initUpgradeBoard();

	// quest board
	m_pQuestBoard = Layer::create();
	m_pQuestBoard->setContentSize(Size(m_szVisibleSize.width * 0.6f, m_szVisibleSize.height * 0.77f)); // 60% screen width, 77% screen height
	m_pQuestBoard->setPosition(-m_pQuestBoard->getContentSize().width, m_szVisibleSize.height * 0.1f);
	m_pGameControl->addChild(m_pQuestBoard, 1);
	initQuestBoard(0);

	// hero info board
	m_pHeroInfoBoard = Layer::create();
	m_pHeroInfoBoard->setContentSize(Size(m_szVisibleSize.width * 0.3f, m_szVisibleSize.height * 0.65f)); // 30% screen width, 65% screen height
	m_pHeroInfoBoard->setPosition(-m_pHeroInfoBoard->getContentSize().width, m_szVisibleSize.height * 0.22f);
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
	auto _pBackNormal = Sprite::create("UI/UI_main_menu/btn_back.png");
	auto _pBackSelected = Sprite::create("UI/UI_main_menu/btn_back.png");
	_pBackSelected->setColor(Color3B(128, 128, 128));
	auto _aBackButton = MenuItemSprite::create(_pBackNormal, _pBackSelected, CC_CALLBACK_0(MenuLayer::buttonBackHandle, this));
	_aBackButton->setScaleX(m_pTopMainMenu->getContentSize().width / _aBackButton->getContentSize().width * 0.1f);
	_aBackButton->setScaleY(m_pTopMainMenu->getContentSize().height / _aBackButton->getContentSize().height);
	_aBackButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aBackButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aBackButton->getContentSize().width * _aBackButton->getScaleX() + _fPadding;

	// life frame
	auto _pLifeFrame = Sprite::create("UI/UI_main_menu/frame_life.png");
	_pLifeFrame->setScaleX(m_pTopMainMenu->getContentSize().width / _pLifeFrame->getContentSize().width * 0.35f);
	_pLifeFrame->setScaleY(m_pTopMainMenu->getContentSize().height / _pLifeFrame->getContentSize().height);
	_pLifeFrame->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pLifeFrame->setPosition(_fXPositionCounter, 0.0f);
	m_pTopMainMenu->addChild(_pLifeFrame, 1);

	for (int i = 0; i < (m_nLifeNumber > 5 ? 5 : m_nLifeNumber); i++) {
		auto _pLifeIcon = Sprite::create("UI/UI_main_menu/icon_life.png");
		_pLifeIcon->setScale(_pLifeFrame->getContentSize().height / _pLifeIcon->getContentSize().height);
		_pLifeIcon->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pLifeIcon->setPosition(_pLifeIcon->getContentSize().width * _pLifeIcon->getScale() * i * 0.9f,
			_pLifeFrame->getContentSize().height / 2);
		_pLifeFrame->addChild(_pLifeIcon, 1);
	}
	m_pTimeCounter = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("5:00"));
	m_pTimeCounter->setBMFontSize(_pLifeFrame->getContentSize().height * 0.6f);
	m_pTimeCounter->setAlignment(TextHAlignment::RIGHT);
	m_pTimeCounter->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_pTimeCounter->setPosition(Vec2(_pLifeFrame->getContentSize().width * 0.8f, _pLifeFrame->getContentSize().height * 0.25f));
	_pLifeFrame->addChild(m_pTimeCounter, 1);
	m_pTimeCounter->setVisible(false);
	if (m_nLifeNumber > 5) {
		Label *_pLabelExtraLife = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("+%i", m_nLifeNumber - 5));
		_pLabelExtraLife->setBMFontSize(_pLifeFrame->getContentSize().height * 0.6f);
		_pLabelExtraLife->setAlignment(TextHAlignment::RIGHT);
		_pLabelExtraLife->setAnchorPoint(Vec2(1.0f, 0.0f));
		_pLabelExtraLife->setPosition(Vec2(_pLifeFrame->getContentSize().width * 0.8f, _pLifeFrame->getContentSize().height * 0.25f));
		_pLifeFrame->addChild(_pLabelExtraLife, 1);
	}

	_fXPositionCounter += 0.0f;

	// add life button
	auto _pAddLifeNormal = Sprite::create("UI/UI_main_menu/btn_add_life.png");
	auto _pAddLifeSelected = Sprite::create("UI/UI_main_menu/btn_add_life.png");
	_pAddLifeSelected->setColor(Color3B(128, 128, 128));
	auto _aAddLifeButton = MenuItemSprite::create(_pAddLifeNormal, _pAddLifeSelected, CC_CALLBACK_0(MenuLayer::buttonAddLifeHandle, this));
	_aAddLifeButton->setScaleX(_pLifeFrame->getContentSize().width * _pLifeFrame->getScaleX() / _aAddLifeButton->getContentSize().width);
	_aAddLifeButton->setScaleY(_pLifeFrame->getContentSize().height * _pLifeFrame->getScaleY() / _aAddLifeButton->getContentSize().height);
	_aAddLifeButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aAddLifeButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aAddLifeButton->getContentSize().width * _aAddLifeButton->getScaleX() + _fPadding;

	// money frame
	auto _pMoneyFrame = Sprite::create("UI/UI_main_menu/frame_money.png");
	_pMoneyFrame->setScaleX(m_pTopMainMenu->getContentSize().width / _pMoneyFrame->getContentSize().width * 0.23f);
	_pMoneyFrame->setScaleY(m_pTopMainMenu->getContentSize().height / _pMoneyFrame->getContentSize().height);
	_pMoneyFrame->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pMoneyFrame->setPosition(_fXPositionCounter, 0.0f);
	m_pTopMainMenu->addChild(_pMoneyFrame, 1);

	Label *_pLabelNumberGold = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", m_nCurrentGold));
	_pLabelNumberGold->setBMFontSize(_pMoneyFrame->getContentSize().height * 0.6f);
	_pLabelNumberGold->setAlignment(TextHAlignment::RIGHT);
	_pLabelNumberGold->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pLabelNumberGold->setPosition(Vec2(_pMoneyFrame->getContentSize().width * 0.7f, _pMoneyFrame->getContentSize().height * 0.25f));
	_pMoneyFrame->addChild(_pLabelNumberGold, 1);

	_fXPositionCounter += 0.0f;

	// add money button
	auto _pAddMoneyNormal = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	auto _pAddMoneySelected = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	_pAddMoneySelected->setColor(Color3B(128, 128, 128));
	auto _aAddMoneyButton = MenuItemSprite::create(_pAddMoneyNormal, _pAddMoneySelected, CC_CALLBACK_0(MenuLayer::buttonAddGoldHandle, this));
	_aAddMoneyButton->setScale(_pMoneyFrame->getContentSize().width * _pLifeFrame->getScaleX() / _aAddMoneyButton->getContentSize().width);
	_aAddMoneyButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aAddMoneyButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aAddMoneyButton->getContentSize().width * _aAddMoneyButton->getScaleX() + _fPadding;

	// diamond frame
	auto _pDiamondFrame = Sprite::create("UI/UI_main_menu/frame_diamond.png");
	_pDiamondFrame->setScaleX(m_pTopMainMenu->getContentSize().width / _pDiamondFrame->getContentSize().width * 0.23f);
	_pDiamondFrame->setScaleY(m_pTopMainMenu->getContentSize().height / _pDiamondFrame->getContentSize().height);
	_pDiamondFrame->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pDiamondFrame->setPosition(_fXPositionCounter, 0.0f);
	m_pTopMainMenu->addChild(_pDiamondFrame, 1);

	Label *_pLabelNumberDiamond = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", m_nCurrentDiamond));
	_pLabelNumberDiamond->setBMFontSize(_pMoneyFrame->getContentSize().height * 0.6f);
	_pLabelNumberDiamond->setAlignment(TextHAlignment::RIGHT);
	_pLabelNumberDiamond->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pLabelNumberDiamond->setPosition(Vec2(_pDiamondFrame->getContentSize().width * 0.7f, _pDiamondFrame->getContentSize().height * 0.25f));
	_pDiamondFrame->addChild(_pLabelNumberDiamond, 1);

	_fXPositionCounter += 0.0f;

	// add diamond button
	auto _pAddDiamondNormal = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	auto _pAddDiamondSelected = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	_pAddDiamondSelected->setColor(Color3B(128, 128, 128));
	auto _aAddDiamondButton = MenuItemSprite::create(_pAddDiamondNormal, _pAddDiamondSelected, CC_CALLBACK_0(MenuLayer::buttonAddDiamondHandle, this));
	_aAddDiamondButton->setScale(_pDiamondFrame->getContentSize().width * _pLifeFrame->getScaleX() / _aAddDiamondButton->getContentSize().width);
	_aAddDiamondButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aAddDiamondButton->setPosition(_fXPositionCounter, 0.0f);


	m_pTopMenu = Menu::create(_aBackButton, _aAddLifeButton, _aAddMoneyButton, _aAddDiamondButton, NULL);
	m_pTopMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMenu->setPosition(0.0f, 0.0f);
	m_pTopMainMenu->addChild(m_pTopMenu, 2);
}

void MenuLayer::initBottomMainMenu() {
	m_pBottomMainLayer->removeAllChildrenWithCleanup(true);

	float _fPadding = 0.0f; // m_szVisibleSize.width / 50;
	float _fXPositionCounter = _fPadding / 2;

	// button quest
	auto _pQuestNormal = Sprite::create("UI/UI_main_menu/btn_quest.png");
	auto _pQuestSelected = Sprite::create("UI/UI_main_menu/btn_quest.png");
	_pQuestSelected->setColor(Color3B(128, 128, 128));
	auto _aQuestButton = MenuItemSprite::create(_pQuestNormal, _pQuestSelected, CC_CALLBACK_0(MenuLayer::buttonQuestHandle, this));
	_aQuestButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aQuestButton->getContentSize().width * 0.1f);
	_aQuestButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aQuestButton->getContentSize().height);
	_aQuestButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aQuestButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aQuestButton->getContentSize().width * _aQuestButton->getScaleX() + _fPadding;

	m_pSpriteQuestAttention = Sprite::create("UI/UI_main_menu/noti.png");
	m_pSpriteQuestAttention->setScale(m_pBottomMainLayer->getContentSize().height / m_pSpriteQuestAttention->getContentSize().height * 0.4f);
	m_pSpriteQuestAttention->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_pSpriteQuestAttention->setPosition(_fXPositionCounter - m_pBottomMainLayer->getContentSize().height * 0.1f,
		m_pBottomMainLayer->getContentSize().height * 0.9f);
	m_pBottomMainLayer->addChild(m_pSpriteQuestAttention, 2);
	ScaleBy *_pZoomOut = ScaleBy::create(1.0f, 0.5f);
	Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
	RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
	m_pSpriteQuestAttention->runAction(_pZoomRepeat);
	m_pSpriteQuestAttention->setVisible(false);

	// button hero
	auto _pHeroNormal = Sprite::create("UI/UI_main_menu/btn_heroes.png");
	auto _pHeroSelected = Sprite::create("UI/UI_main_menu/btn_heroes.png");
	_pHeroSelected->setColor(Color3B(128, 128, 128));
	auto _aHeroButton = MenuItemSprite::create(_pHeroNormal, _pHeroSelected, CC_CALLBACK_0(MenuLayer::buttonHeroesHandle, this));
	_aHeroButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aHeroButton->getContentSize().width * 0.1f);
	_aHeroButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aHeroButton->getContentSize().height);
	_aHeroButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aHeroButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aHeroButton->getContentSize().width * _aHeroButton->getScaleX() + _fPadding;

	// button shop
	auto _pShopNormal = Sprite::create("UI/UI_main_menu/btn_shop.png");
	auto _pShopSelected = Sprite::create("UI/UI_main_menu/btn_shop.png");
	_pShopSelected->setColor(Color3B(128, 128, 128));
	auto _aShopButton = MenuItemSprite::create(_pShopNormal, _pShopSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aShopButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aShopButton->getContentSize().width * 0.1f);
	_aShopButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aShopButton->getContentSize().height);
	_aShopButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aShopButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aShopButton->getContentSize().width * _aShopButton->getScaleX() + _fPadding;

	// button leaderboard
	auto _pLeaderBoardNormal = Sprite::create("UI/UI_main_menu/btn_leaderboard.png");
	auto _pLeaderBoardSelected = Sprite::create("UI/UI_main_menu/btn_leaderboard.png");
	_pLeaderBoardSelected->setColor(Color3B(128, 128, 128));
	auto _aLeaderBoardButton = MenuItemSprite::create(_pLeaderBoardNormal, _pLeaderBoardSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aLeaderBoardButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aLeaderBoardButton->getContentSize().width * 0.1f);
	_aLeaderBoardButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aLeaderBoardButton->getContentSize().height);
	_aLeaderBoardButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aLeaderBoardButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aLeaderBoardButton->getContentSize().width * _aLeaderBoardButton->getScaleX() + _fPadding;

	// button free coin
	auto _pFreeCoinNormal = Sprite::create("UI/UI_main_menu/btn_free_coin.png");
	auto _pFreeCoinSelected = Sprite::create("UI/UI_main_menu/btn_free_coin.png");
	_pFreeCoinSelected->setColor(Color3B(128, 128, 128));
	auto _aFreeCoinButton = MenuItemSprite::create(_pFreeCoinNormal, _pFreeCoinSelected, CC_CALLBACK_0(MenuLayer::buttonFreeCoinHandle, this));
	_aFreeCoinButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aFreeCoinButton->getContentSize().width * 0.1f);
	_aFreeCoinButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aFreeCoinButton->getContentSize().height);
	_aFreeCoinButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aFreeCoinButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aFreeCoinButton->getContentSize().width * _aFreeCoinButton->getScaleX() + _fPadding;

	m_pSpriteFreeCoinAttention = Sprite::create("UI/UI_main_menu/noti.png");
	m_pSpriteFreeCoinAttention->setScale(m_pBottomMainLayer->getContentSize().height / m_pSpriteFreeCoinAttention->getContentSize().height * 0.4f);
	m_pSpriteFreeCoinAttention->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_pSpriteFreeCoinAttention->setPosition(_fXPositionCounter - m_pBottomMainLayer->getContentSize().height * 0.1f,
		m_pBottomMainLayer->getContentSize().height * 0.9f);
	m_pBottomMainLayer->addChild(m_pSpriteFreeCoinAttention, 2);
	ScaleBy *_pZoomOut2 = ScaleBy::create(1.0f, 0.5f);
	Sequence *_pZoomSequence2 = Sequence::create(_pZoomOut2, _pZoomOut2->reverse(), NULL);
	RepeatForever* _pZoomRepeat2 = RepeatForever::create(_pZoomSequence2);
	m_pSpriteFreeCoinAttention->runAction(_pZoomRepeat2);
	if (REF->getFreeCoin() <= 0) {
		m_pSpriteFreeCoinAttention->setVisible(false);
	}

	// button setting
	auto _pSettingNormal = Sprite::create("UI/UI_main_menu/btn_setting.png");
	auto _pSettingSelected = Sprite::create("UI/UI_main_menu/btn_setting.png");
	_pSettingSelected->setColor(Color3B(128, 128, 128));
	auto _aSettingButton = MenuItemSprite::create(_pSettingNormal, _pSettingSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aSettingButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aSettingButton->getContentSize().width * 0.1f);
	_aSettingButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aSettingButton->getContentSize().height);
	_aSettingButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aSettingButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aSettingButton->getContentSize().width * _aSettingButton->getScaleX() + _fPadding;

	// button more game
	auto _pMoreGameNormal = Sprite::create("UI/UI_main_menu/btn_more_games.png");
	auto _pMoreGameSelected = Sprite::create("UI/UI_main_menu/btn_more_games.png");
	_pMoreGameSelected->setColor(Color3B(128, 128, 128));
	auto _aMoreGameButton = MenuItemSprite::create(_pMoreGameNormal, _pMoreGameSelected, CC_CALLBACK_0(MenuLayer::buttonMoreGameHandle, this));
	_aMoreGameButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aMoreGameButton->getContentSize().width * 0.1f);
	_aMoreGameButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aMoreGameButton->getContentSize().height);
	_aMoreGameButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aMoreGameButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aMoreGameButton->getContentSize().width * _aMoreGameButton->getScaleX() + _fPadding
		+ m_pBottomMainLayer->getContentSize().width * 0.02f;

	// button start
	auto _pStartNormal = Sprite::create("UI/UI_main_menu/btn_start.png");
	auto _pStartSelected = Sprite::create("UI/UI_main_menu/btn_start.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_0(MenuLayer::buttonStartHandle, this));
	_aStartButton->setScaleX(m_pBottomMainLayer->getContentSize().width / _aStartButton->getContentSize().width * 0.26f);
	_aStartButton->setScaleY(m_pBottomMainLayer->getContentSize().height / _aStartButton->getContentSize().height * 0.8f);
	_aStartButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	_aStartButton->setPosition(_fXPositionCounter, 0.0f);

	m_fButtonStartPosition = _fXPositionCounter;

	m_pBottomMainMenu = Menu::create(_aQuestButton, _aHeroButton, _aShopButton, _aLeaderBoardButton, _aFreeCoinButton,
		_aSettingButton, _aMoreGameButton, _aStartButton, NULL);
	m_pBottomMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.27f)); // fill screen width, 27% screen height
	m_pBottomMainMenu->setPosition(0.0f, 0.0f);
	m_pBottomMainLayer->addChild(m_pBottomMainMenu, 1);
}

void MenuLayer::initItemBoard(float p_fPercent) {
	// item board
	m_pItemBoard = Layer::create();
	m_pItemBoard->setContentSize(Size(m_szVisibleSize.width * 0.45f, m_szVisibleSize.height * 0.62f)); // 35% screen width, 62% screen height
	m_pItemBoard->setPosition(m_szVisibleSize.width, m_szVisibleSize.height * 0.25);
	m_pGameControl->addChild(m_pItemBoard, 1);

	// board item
	Sprite *_pItemBoard = Sprite::create("UI/UI_main_menu/board_item.png");
	_pItemBoard->setScaleX(m_pItemBoard->getContentSize().width / _pItemBoard->getContentSize().width);
	_pItemBoard->setScaleY(m_pItemBoard->getContentSize().height / _pItemBoard->getContentSize().height);
	_pItemBoard->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pItemBoard->setPosition(0.0f, 0.0f);
	m_pItemBoard->addChild(_pItemBoard, 0);

	int _nNumberItems = 5;
	int _nNumberItemsVisiable = 3;
	// scroll view
	m_pItemScrollView = ScrollView::create();
	m_pItemScrollView->setContentSize(Size(_pItemBoard->getContentSize().width * _pItemBoard->getScaleX() * 0.8f,
		_pItemBoard->getContentSize().height * _pItemBoard->getScaleY() * 0.7f));
	m_pItemScrollView->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_pItemScrollView->setPosition(Vec2(m_pItemBoard->getContentSize().width * 0.1f, m_pItemBoard->getContentSize().height * 0.1f));
	m_pItemScrollView->setDirection(ScrollView::Direction::VERTICAL);
	m_pItemScrollView->setBounceEnabled(true);
	m_pItemScrollView->setTouchEnabled(true);
	m_pItemScrollView->setScrollBarOpacity(0.0f);
	m_pItemScrollView->setInnerContainerSize(Size(m_pItemScrollView->getContentSize().width,
		m_pItemScrollView->getContentSize().height * _nNumberItems / _nNumberItemsVisiable));
	m_pItemBoard->addChild(m_pItemScrollView, 1);

	// int _nItemIndexCounter = 0;
	float _fItemWidth = m_pItemScrollView->getContentSize().width;
	float _fItemHeight = m_pItemScrollView->getContentSize().height / _nNumberItemsVisiable;
	float _fScrollViewPadding = _fItemHeight / 20;

	m_pItemBoardMenu = Menu::create();
	m_pItemBoardMenu->setContentSize(m_pItemScrollView->getContentSize());
	m_pItemBoardMenu->setPosition(0.0f, 0.0f);
	m_pItemScrollView->addChild(m_pItemBoardMenu, 2);

	for (int i = 0; i < _nNumberItems; i++) {
		Layer *_pItem = new Layer();
		_pItem->setContentSize(Size(_fItemWidth, _fItemHeight));
		_pItem->setPosition(0.0f, _fItemHeight * (_nNumberItems - i - 1));
		m_pItemScrollView->addChild(_pItem, 1);
		JSMENU->readFile(i);

		Sprite *_pItemSprite = Sprite::create(JSMENU->getIconFilePath());
		_pItemSprite->setScale(_fItemHeight / _pItemSprite->getContentSize().height * 0.9f);
		_pItemSprite->setAnchorPoint(Vec2(0.0f, 1.0f));
		_pItemSprite->setPosition(Vec2(0.0f, _fItemHeight));
		_pItem->addChild(_pItemSprite, 1);

		Sprite *_ItemNumberSprite = Sprite::create("UI/UI_main_menu/red_circle.png");
		_ItemNumberSprite->setScale(_pItemSprite->getContentSize().height / _ItemNumberSprite->getContentSize().height * 0.4f);
		_ItemNumberSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_ItemNumberSprite->setPosition(Vec2(_pItemSprite->getContentSize().width, 0.0f));
		_pItemSprite->addChild(_ItemNumberSprite, 1);

		if (m_arNumberItemOwning[i] < 100) {
			m_arLabelNumberItemOwning[i] = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("%d", m_arNumberItemOwning[i]));
		}
		else {
			m_arLabelNumberItemOwning[i] = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("99+"));
		}
		m_arLabelNumberItemOwning[i]->setBMFontSize(_ItemNumberSprite->getContentSize().height * 0.8f);
		m_arLabelNumberItemOwning[i]->setAnchorPoint(Vec2(0.5f, 0.0f));
		m_arLabelNumberItemOwning[i]->setPosition(Vec2(_ItemNumberSprite->getContentSize().width / 2, _ItemNumberSprite->getContentSize().height * 0.1f));
		_ItemNumberSprite->addChild(m_arLabelNumberItemOwning[i], 1);

		string _sTempName = JSMENU->getName();
		Label *_pLabelNameItem = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", _sTempName.c_str()));
		_pLabelNameItem->setBMFontSize(_pItem->getContentSize().height * 0.3f);
		_pLabelNameItem->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pLabelNameItem->setPosition(Vec2(_pItemSprite->getContentSize().width * _pItemSprite->getScale() * 1.2f, _fItemHeight * 0.6f));
		_pItem->addChild(_pLabelNameItem, 1);
		string _sTempDescription = JSMENU->getDescription();
		Label *_pLabelDescriptionItem = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", _sTempDescription.c_str()));
		_pLabelDescriptionItem->setBMFontSize(_pItem->getContentSize().height * 0.2f);
		_pLabelDescriptionItem->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pLabelDescriptionItem->setPosition(Vec2(_pItemSprite->getContentSize().width * _pItemSprite->getScale() * 1.2f, _fItemHeight * 0.3f));
		_pItem->addChild(_pLabelDescriptionItem, 1);

		auto _pBuyItemNormal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
		auto _pBuyItemSelected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
		_pBuyItemSelected->setColor(Color3B(128, 128, 128));
		m_arBuyItemButton[i] = MenuItemSprite::create(_pBuyItemNormal, _pBuyItemSelected, CC_CALLBACK_0(MenuLayer::buttonBuyItemHandle, this, i));
		m_arBuyItemButton[i]->setScale(_fItemWidth / m_arBuyItemButton[i]->getContentSize().width * 0.25f);
		m_arBuyItemButton[i]->setAnchorPoint(Vec2(1.0f, 0.0f));
		m_arBuyItemButton[i]->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - i - 0.95f)));
		m_pItemBoardMenu->addChild(m_arBuyItemButton[i], 1);

		Sprite *_CoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
		_CoinSprite->setScale(m_arBuyItemButton[i]->getContentSize().height / _CoinSprite->getContentSize().height);
		_CoinSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_CoinSprite->setPosition(Vec2(m_arBuyItemButton[i]->getContentSize().width, m_arBuyItemButton[i]->getContentSize().height));
		m_arBuyItemButton[i]->addChild(_CoinSprite, 1);

		m_arItemPrice[i] = JSMENU->getPrice();
		Label *_pLabelCost = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", m_arItemPrice[i]));
		_pLabelCost->setBMFontSize(_CoinSprite->getContentSize().height);
		_pLabelCost->setAnchorPoint(Vec2(1.0f, 0.0f));
		_pLabelCost->setPosition(Vec2(0.0f, 0.0f));
		_CoinSprite->addChild(_pLabelCost, 1);

		m_arSpriteItemMax[i] = Sprite::create("UI/UI_main_menu/icon_max.png");
		m_arSpriteItemMax[i]->setScale(_fItemHeight / m_arSpriteItemMax[i]->getContentSize().height * 0.9f);
		m_arSpriteItemMax[i]->setAnchorPoint(Vec2(1.0f, 1.0f));
		m_arSpriteItemMax[i]->setPosition(Vec2(_fItemWidth, _fItemHeight));
		_pItem->addChild(m_arSpriteItemMax[i], 1);

		if (m_arNumberItemOwning[i] <= 0) {
			_ItemNumberSprite->setVisible(false);
			m_arSpriteItemMax[i]->setVisible(false);
		}
		else {
			m_arBuyItemButton[i]->setVisible(false);
		}

		if (i != _nNumberItems - 1) { // line between two items
			Sprite *_pItemLine = Sprite::create("UI/UI_main_menu/line.png");
			_pItemLine->setScale(_fItemWidth / _pItemLine->getContentSize().width);
			_pItemLine->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pItemLine->setPosition(Vec2(0.0f, 0.0f));
			_pItem->addChild(_pItemLine, 1);
		}
	}
}

void MenuLayer::initUpgradeBoard() {
	m_pUpgradeBoard->removeAllChildrenWithCleanup(true);

	// board upgrate
	Sprite *_pBoardUpgrate = Sprite::create("UI/UI_main_menu/board_skill_upgrade.png");
	_pBoardUpgrate->setScaleX(m_pUpgradeBoard->getContentSize().width / _pBoardUpgrate->getContentSize().width);
	_pBoardUpgrate->setScaleY(m_pUpgradeBoard->getContentSize().height / _pBoardUpgrate->getContentSize().height);
	_pBoardUpgrate->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pBoardUpgrate->setPosition(0.0f, 0.0f);
	m_pUpgradeBoard->addChild(_pBoardUpgrate, 0);

	JSHERO->readFile(m_nIndexHeroPicked);
	string _arSkillSpritePath[3] = { JSHERO->getPathMainImageSkill1(), JSHERO->getPathMainImageSkill2(), JSHERO->getPathMainImageSkill3() };
	string _arSkillName[3] = { JSHERO->getNameOfSkill_1(), JSHERO->getNameOfSkill_2(), JSHERO->getNameOfSkill_3() };
	MenuItemSprite *_arUpgrateSkill[3];
	float _arDuration[3] = { REF->getDurationSkill_1(), REF->getDurationSkill_2(), REF->getDurationSkill_3() };
	float _arCoolDown[3] = { REF->getCoolDownSkill_1(), REF->getCoolDownSkill_2(), REF->getCoolDownSkill_3() };

	int _arSkillLevel[3] = { REF->getLevelSkill_1(), REF->getLevelSkill_2(), REF->getLevelSkill_3() };
	for (int i = 0; i < 3; i++) {
		Layer *_pSkillInfo = new Layer();
		_pSkillInfo->setContentSize(Size(m_pUpgradeBoard->getContentSize().width * 0.8f, m_pUpgradeBoard->getContentSize().height * 0.23f));
		_pSkillInfo->setPosition(m_pUpgradeBoard->getContentSize().width * 0.1f, m_pUpgradeBoard->getContentSize().height * (0.57f - i * 0.23f));
		m_pUpgradeBoard->addChild(_pSkillInfo, 1);

		Sprite *_pSkillSprite = Sprite::create(_arSkillSpritePath[i]);
		_pSkillSprite->setScaleX(_pSkillInfo->getContentSize().height / _pSkillSprite->getContentSize().height * 0.9f);
		_pSkillSprite->setScaleY(_pSkillInfo->getContentSize().height / _pSkillSprite->getContentSize().height * 0.9f);
		_pSkillSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pSkillSprite->setPosition(0.0f, _pSkillInfo->getContentSize().height / 2);
		_pSkillInfo->addChild(_pSkillSprite, 0);

		/*Sprite *_SkillLevelSprite = Sprite::create("UI/UI_main_menu/red_circle.png");
		_SkillLevelSprite->setScale(_pSkillSprite->getContentSize().height / _SkillLevelSprite->getContentSize().height * 0.4f);
		_SkillLevelSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_SkillLevelSprite->setPosition(Vec2(_pSkillSprite->getContentSize().width, 0.0f));
		_pSkillSprite->addChild(_SkillLevelSprite, 1);
		Label *_pLevelSkillLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("LV.%d", _arSkillLevel[i]));
		_pLevelSkillLabel->setBMFontSize(_SkillLevelSprite->getContentSize().height * 0.6f);
		_pLevelSkillLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
		_pLevelSkillLabel->setPosition(Vec2(_SkillLevelSprite->getContentSize().width / 2, _SkillLevelSprite->getContentSize().height * 0.2f));
		_SkillLevelSprite->addChild(_pLevelSkillLabel, 1);*/

		Label *_pSkillNameLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", _arSkillName[i].c_str()));
		_pSkillNameLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.25f);
		_pSkillNameLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pSkillNameLabel->setPosition(Vec2(_pSkillInfo->getContentSize().height, _pSkillInfo->getContentSize().height * 0.71f));
		_pSkillInfo->addChild(_pSkillNameLabel, 1);

		Label *_pLevelLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Level :"));
		_pLevelLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f);
		_pLevelLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pLevelLabel->setPosition(Vec2(_pSkillInfo->getContentSize().height, _pSkillInfo->getContentSize().height * 0.49f));
		_pSkillInfo->addChild(_pLevelLabel, 1);

		Label *_pDurationLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Duration : "));
		_pDurationLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f);
		_pDurationLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pDurationLabel->setPosition(Vec2(_pSkillInfo->getContentSize().height, _pSkillInfo->getContentSize().height * 0.27f));
		_pSkillInfo->addChild(_pDurationLabel, 1);

		Label *_pDurationValueLabel = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", (int)_arDuration[i]));
		_pDurationValueLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f);
		_pDurationValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pDurationValueLabel->setPosition(Vec2(_pDurationLabel->getContentSize().width, 0.0f));
		_pDurationLabel->addChild(_pDurationValueLabel, 1);

		Label *_pCoolDownLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Cooldown : "));
		_pCoolDownLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f);
		_pCoolDownLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pCoolDownLabel->setPosition(Vec2(_pSkillInfo->getContentSize().height, _pSkillInfo->getContentSize().height * 0.05f));
		_pSkillInfo->addChild(_pCoolDownLabel, 1);

		Label *_pCoolDownValueLabel = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", (int)_arCoolDown[i]));
		_pCoolDownValueLabel->setBMFontSize(_pSkillInfo->getContentSize().height * 0.2f);
		_pCoolDownValueLabel->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pCoolDownValueLabel->setPosition(Vec2(_pCoolDownLabel->getContentSize().width, 0.0f));
		_pCoolDownLabel->addChild(_pCoolDownValueLabel, 1);

		Sprite *_pSkillLevelBar = Sprite::create("UI/UI_main_menu/level_skill_bar.png");
		_pSkillLevelBar->setScale(_pSkillInfo->getContentSize().width / _pSkillLevelBar->getContentSize().width * 0.25f);
		_pSkillLevelBar->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pSkillLevelBar->setPosition(Vec2(_pSkillInfo->getContentSize().width * 0.45f, _pSkillInfo->getContentSize().height * 0.49f));
		_pSkillInfo->addChild(_pSkillLevelBar, 1);
		for (int j = 0; j < _arSkillLevel[i]; j++) {
			Sprite *_pSkillLevelPoint = Sprite::create("UI/UI_main_menu/level_skill_point.png");
			_pSkillLevelPoint->setScale(_pSkillLevelBar->getContentSize().height / _pSkillLevelBar->getContentSize().height);
			_pSkillLevelPoint->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pSkillLevelPoint->setPosition(Vec2(_pSkillLevelBar->getContentSize().width * 0.1 * j, 0.0f));
			_pSkillLevelBar->addChild(_pSkillLevelPoint, 1);
		}

		Sprite *_CoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
		_CoinSprite->setScale(_pSkillInfo->getContentSize().height / _CoinSprite->getContentSize().height * 0.3f);
		_CoinSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_CoinSprite->setPosition(Vec2(_pSkillInfo->getContentSize().width, _pSkillInfo->getContentSize().height * 0.4f));
		_pSkillInfo->addChild(_CoinSprite, 1);

		int _nCost = 5000;

		Label *_pLabelCost = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", _nCost));
		_pLabelCost->setBMFontSize(_pSkillInfo->getContentSize().height * 0.25f);
		_pLabelCost->setAnchorPoint(Vec2(1.0f, 0.0f));
		_pLabelCost->setPosition(Vec2(_pSkillInfo->getContentSize().width * 0.9f, _pSkillInfo->getContentSize().height * 0.45f));
		_pSkillInfo->addChild(_pLabelCost, 1);

		auto _pUpdgrateNormal = Sprite::create("UI/UI_main_menu/btn_upgrade_1.png");
		auto _pUpdgrateSelected = Sprite::create("UI/UI_main_menu/btn_upgrade_3.png");
		_arUpgrateSkill[i] = MenuItemSprite::create(_pUpdgrateNormal, _pUpdgrateSelected, CC_CALLBACK_0(MenuLayer::buttonUpgradeSkillHandle, this, i));
		_arUpgrateSkill[i]->setScale(m_pUpgradeBoard->getContentSize().width / _arUpgrateSkill[i]->getContentSize().width * 0.2f);
		_arUpgrateSkill[i]->setAnchorPoint(Vec2(1.0f, 0.0f));
		_arUpgrateSkill[i]->setPosition(Vec2(m_pUpgradeBoard->getContentSize().width * 0.9f, m_pUpgradeBoard->getContentSize().height * (0.57f - i * 0.23f)));
		if (REF->getIsLockedHero()) {
			_arUpgrateSkill[i]->setEnabled(false);
			_arUpgrateSkill[i]->setNormalImage(Sprite::create("UI/UI_main_menu/btn_upgrade_2.png"));
		}

		if (_arSkillLevel[i] >= 10) {
			_CoinSprite->setVisible(false);
			_pLabelCost->setVisible(false);
			_arUpgrateSkill[i]->setVisible(false);

			Sprite *_pMaxUpgrate = Sprite::create("UI/UI_main_menu/icon_max.png");
			_pMaxUpgrate->setScale(_pSkillLevelBar->getContentSize().height / _pSkillLevelBar->getContentSize().height * 0.8f);
			_pMaxUpgrate->setAnchorPoint(Vec2(1.0f, 0.5f));
			_pMaxUpgrate->setPosition(Vec2(_pSkillInfo->getContentSize().width, _pSkillInfo->getContentSize().height * 0.4f));
			_pSkillInfo->addChild(_pMaxUpgrate, 1);
		}
	}

	m_pSkillBoardMenu = Menu::create(_arUpgrateSkill[0], _arUpgrateSkill[1], _arUpgrateSkill[2], NULL);
	m_pSkillBoardMenu->setContentSize(m_pUpgradeBoard->getContentSize());
	m_pSkillBoardMenu->setPosition(0.0f, 0.0f);
	m_pUpgradeBoard->addChild(m_pSkillBoardMenu, 2);
}

void MenuLayer::initQuestBoard(int p_nFocus) {
	m_pQuestBoard->removeAllChildrenWithCleanup(true);

	// board quest
	Sprite *_pQuestBoardBackground = Sprite::create("UI/UI_main_menu/UI_quest/board_quest.png");
	_pQuestBoardBackground->setScaleX(m_pQuestBoard->getContentSize().width / _pQuestBoardBackground->getContentSize().width);
	_pQuestBoardBackground->setScaleY(m_pQuestBoard->getContentSize().height / _pQuestBoardBackground->getContentSize().height);
	_pQuestBoardBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pQuestBoardBackground->setPosition(0.0f, 0.0f);
	m_pQuestBoard->addChild(_pQuestBoardBackground, 0);

	int _nNumberQuests = JSQUEST->getNumberQuest();
	int _nNumberQuestsVisiable = 3;
	ScrollView *_pQuestBoardZone = ScrollView::create();
	_pQuestBoardZone->setContentSize(Size(m_pQuestBoard->getContentSize().width * 0.85f, m_pQuestBoard->getContentSize().height * 0.7f));
	_pQuestBoardZone->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pQuestBoardZone->setPosition(Vec2(m_pQuestBoard->getContentSize().width * 0.5f, m_pQuestBoard->getContentSize().height * 0.45f));
	_pQuestBoardZone->setDirection(ScrollView::Direction::VERTICAL);
	_pQuestBoardZone->setBounceEnabled(true);
	_pQuestBoardZone->setTouchEnabled(true);
	_pQuestBoardZone->setScrollBarOpacity(0.0f);
	_pQuestBoardZone->setInnerContainerSize(Size(_pQuestBoardZone->getContentSize().width,
		_pQuestBoardZone->getContentSize().height * _nNumberQuests / _nNumberQuestsVisiable));
	m_pQuestBoard->addChild(_pQuestBoardZone, 1);

	float _fItemWidth = _pQuestBoardZone->getContentSize().width;
	float _fItemHeight = _pQuestBoardZone->getContentSize().height / _nNumberQuestsVisiable;
	float _fScrollViewPadding = _fItemHeight / 20;

	m_pQuestBoardMenu = Menu::create();
	m_pQuestBoardMenu->setContentSize(_pQuestBoardZone->getContentSize());
	m_pQuestBoardMenu->setPosition(0.0f, 0.0f);
	_pQuestBoardZone->addChild(m_pQuestBoardMenu, 2);

	m_pSpriteQuestAttention->setVisible(false);

	for (int i = 0; i < _nNumberQuests; i++) {
		Layer *_pQuestLayer = new Layer();
		_pQuestLayer->setContentSize(Size(_fItemWidth, _fItemHeight));
		_pQuestLayer->setPosition(0.0f, _fItemHeight * (_nNumberQuests - i - 1));
		_pQuestBoardZone->addChild(_pQuestLayer, 1);
		JSQUEST->readQuest(i);
		REF->readDataQuest(i);

		Label *_pLabelQuestName = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", JSQUEST->getQuestName().c_str()));
		_pLabelQuestName->setBMFontSize(_fItemHeight * 0.3f);
		_pLabelQuestName->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pLabelQuestName->setPosition(Vec2(_fItemWidth * 0.05f, _fItemHeight * 0.7f));
		_pQuestLayer->addChild(_pLabelQuestName, 1);

		Label *_pQuestDescription = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", JSQUEST->getQuestDescription().c_str()));
		_pQuestDescription->setBMFontSize(_fItemHeight * 0.2f);
		_pQuestDescription->setMaxLineWidth(_fItemWidth * 0.7f);
		_pQuestDescription->setLineHeight(_fItemHeight * 0.4f);
		_pQuestDescription->setAnchorPoint(Vec2(0.0f, 0.5f));
		_pQuestDescription->setPosition(Vec2(0.0f, _fItemHeight * 0.7f));
		_pQuestLayer->addChild(_pQuestDescription, 1);

		if (REF->getRewardedQuestTimes() >= JSQUEST->getLimitRequest()) {
			Sprite *_pQuestComplete = Sprite::create("UI/UI_main_menu/UI_quest/icon_completed.png");
			_pQuestComplete->setScale(_pQuestLayer->getContentSize().height / _pQuestComplete->getContentSize().height * 0.8f);
			_pQuestComplete->setAnchorPoint(Vec2(0.5f, 0.5f));
			_pQuestComplete->setPosition(Vec2(_pQuestLayer->getContentSize().width * 0.85f, _pQuestLayer->getContentSize().height * 0.5f));
			_pQuestLayer->addChild(_pQuestComplete, 1);
		}
		else {
			int _nNumber = REF->getNumberQuest();
			int _nComplete = JSQUEST->getCompleteRequest();
			int _nGoldReward = JSQUEST->getGoldReward();
			for (int j = 0; j < REF->getRewardedQuestTimes(); j++) {
				_nComplete *= JSQUEST->getStepRequest();
				_nGoldReward *= JSQUEST->getStepRequest();
			}
			Label *_pLabelGoldReward = Label::createWithBMFont("fonts/font_coin-export.fnt",
				StringUtils::format("%d", _nGoldReward));
			_pLabelGoldReward->setBMFontSize(_pQuestLayer->getContentSize().height * 0.25f);
			_pLabelGoldReward->setAnchorPoint(Vec2(0.65f, 0.0f));
			_pLabelGoldReward->setPosition(Vec2(_pQuestLayer->getContentSize().width * 0.85f, _pQuestLayer->getContentSize().height * 0.6f));
			_pQuestLayer->addChild(_pLabelGoldReward, 1);

			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
			_pCoinSprite->setScale(_pQuestLayer->getContentSize().height / _pCoinSprite->getContentSize().height * 0.3f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pCoinSprite->setPosition(Vec2(_pLabelGoldReward->getContentSize().width, 0.0f));
			_pLabelGoldReward->addChild(_pCoinSprite, 1);

			float _fScaleX = (float)(_nNumber < _nComplete ? _nNumber : _nComplete) / (float)_nComplete;
			if (_fScaleX < 1.0f) {
				Sprite *_pQuestBar = Sprite::create("UI/UI_main_menu/UI_quest/quest_progress_bar.png");
				_pQuestBar->setScaleX(_pQuestLayer->getContentSize().width / _pQuestBar->getContentSize().width * 0.25f);
				_pQuestBar->setScaleY(_pQuestLayer->getContentSize().height / _pQuestBar->getContentSize().height * 0.3f);
				_pQuestBar->setAnchorPoint(Vec2(0.5f, 1.0f));
				_pQuestBar->setPosition(Vec2(_pQuestLayer->getContentSize().width * 0.85f, _pQuestLayer->getContentSize().height * 0.5f));
				_pQuestLayer->addChild(_pQuestBar, 1);

				Label *_pLabelQuestStatus = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%d / %d", _nNumber < _nComplete ? _nNumber : _nComplete, _nComplete));
				_pLabelQuestStatus->setBMFontSize(_pQuestBar->getContentSize().height * 0.8f);
				_pLabelQuestStatus->setAlignment(TextHAlignment::RIGHT);
				_pLabelQuestStatus->setAnchorPoint(Vec2(0.5f, 0.0f));
				_pLabelQuestStatus->setPosition(Vec2(_pQuestBar->getContentSize().width / 2, _pQuestBar->getContentSize().height / 5));
				_pQuestBar->addChild(_pLabelQuestStatus, 2);

				Sprite *_pQuestPoint = Sprite::create("UI/UI_main_menu/UI_quest/quest_progress_point.png");
				_pQuestPoint->setScaleX(_pQuestBar->getContentSize().width / _pQuestPoint->getContentSize().width * _fScaleX);
				_pQuestPoint->setScaleY(_pQuestBar->getContentSize().height / _pQuestPoint->getContentSize().height);
				_pQuestPoint->setAnchorPoint(Vec2(0.0f, 0.0f));
				_pQuestPoint->setPosition(Vec2(0.0f, 0.0f));
				_pQuestBar->addChild(_pQuestPoint, 1);
			}
			else {
				m_pSpriteQuestAttention->setVisible(true);

				auto _pRewardNormal = Sprite::create("UI/UI_main_menu/UI_quest/btn_claim_1.png");
				auto _pRewardSelected = Sprite::create("UI/UI_main_menu/UI_quest/btn_claim_2.png");
				MenuItemSprite *_aQuestRewardButton = MenuItemSprite::create(_pRewardNormal, _pRewardSelected, CC_CALLBACK_0(MenuLayer::buttonRewardQuest, this, i));
				_aQuestRewardButton->setScaleX(_fItemWidth / _aQuestRewardButton->getContentSize().width * 0.2f);
				_aQuestRewardButton->setScaleY(_fItemHeight / _aQuestRewardButton->getContentSize().height * 0.4f);
				_aQuestRewardButton->setAnchorPoint(Vec2(0.5f, 1.0f));
				_aQuestRewardButton->setPosition(Vec2(_fItemWidth * 0.85f, _fItemHeight * (_nNumberQuests - i - 0.5f)));

				m_pQuestBoardMenu->addChild(_aQuestRewardButton, 1);

				ScaleBy *_pZoomOut = ScaleBy::create(0.5f, 0.9f);
				Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
				RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
				_aQuestRewardButton->runAction(_pZoomRepeat);

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
	Sprite *_pInfoBoard = Sprite::create("UI/UI_main_menu/board_information.png");
	_pInfoBoard->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pInfoBoard->getContentSize().width);
	_pInfoBoard->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pInfoBoard->getContentSize().height);
	_pInfoBoard->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pInfoBoard->setPosition(0.0f, 0.0f);
	m_pHeroInfoBoard->addChild(_pInfoBoard, 0);

	// character name
	string _sTempName = JSHERO->getName();
	Label *_pLabelNameHero = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", _sTempName.c_str()));
	_pLabelNameHero->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.07f);
	_pLabelNameHero->setAnchorPoint(Vec2(0.5f, 1.0f));
	_pLabelNameHero->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width / 2, m_pHeroInfoBoard->getContentSize().height * 0.95f));
	m_pHeroInfoBoard->addChild(_pLabelNameHero, 1);

	string _sHeroInfo = JSHERO->getInfor();
	Label *_pHeroDescription = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", _sHeroInfo.c_str()));
	_pHeroDescription->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	_pHeroDescription->setMaxLineWidth(m_pHeroInfoBoard->getContentSize().width * 0.7f);
	_pHeroDescription->setLineHeight(m_pHeroInfoBoard->getContentSize().height * 0.1f);
	_pHeroDescription->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pHeroDescription->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_pHeroInfoBoard->getContentSize().height * 0.7f));
	m_pHeroInfoBoard->addChild(_pHeroDescription, 1);

	// character current level label
	int _nCurrentLevel = REF->getCurrentLevel();
	Label *_pLabelCurrentLevel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Level %d", _nCurrentLevel));
	_pLabelCurrentLevel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	_pLabelCurrentLevel->setAnchorPoint(Vec2(0.5f, 1.0f));
	_pLabelCurrentLevel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_pHeroInfoBoard->getContentSize().height * 0.5f));
	m_pHeroInfoBoard->addChild(_pLabelCurrentLevel, 1);

	int _nHealthPoint = REF->getCurrentHealth();
	int _nBonusScore = REF->getBonusScore();
	int _nBonusGold = REF->getBonusGold();
	Label *_pCurrentInfoLabel = Label::createWithBMFont("fonts/font_normal-export.fnt",
		StringUtils::format("HP: %d   Score : %d%%   Gold: %d%%", _nHealthPoint, _nBonusScore, _nBonusGold));
	_pCurrentInfoLabel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	_pCurrentInfoLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
	_pCurrentInfoLabel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_pHeroInfoBoard->getContentSize().height * 0.43f));
	m_pHeroInfoBoard->addChild(_pCurrentInfoLabel, 1);

	Sprite *_pHPBackground = Sprite::create("UI/UI_main_menu/frame_hp.png");
	_pHPBackground->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pHPBackground->getContentSize().width * 0.2f);
	_pHPBackground->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pHPBackground->getContentSize().height * 0.05f);
	_pHPBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pHPBackground->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.1f, m_pHeroInfoBoard->getContentSize().height * 0.3f);
	m_pHeroInfoBoard->addChild(_pHPBackground, 0);

	Sprite *_pScoreBackground = Sprite::create("UI/UI_main_menu/frame_score.png");
	_pScoreBackground->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pScoreBackground->getContentSize().width * 0.3f);
	_pScoreBackground->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pScoreBackground->getContentSize().height * 0.05f);
	_pScoreBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pScoreBackground->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.3f, m_pHeroInfoBoard->getContentSize().height * 0.3f);
	m_pHeroInfoBoard->addChild(_pScoreBackground, 0);

	Sprite *_pGoldBackground = Sprite::create("UI/UI_main_menu/frame_gold.png");
	_pGoldBackground->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pGoldBackground->getContentSize().width * 0.3f);
	_pGoldBackground->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pGoldBackground->getContentSize().height * 0.05f);
	_pGoldBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
	_pGoldBackground->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.6f, m_pHeroInfoBoard->getContentSize().height * 0.3f);
	m_pHeroInfoBoard->addChild(_pGoldBackground, 0);

	if (_nCurrentLevel < JSHERO->getMaxLevel()) {
		// character next level label
		Label *_pLabelNextLevel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Level %d", _nCurrentLevel + 1));
		_pLabelNextLevel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
		_pLabelNextLevel->setAnchorPoint(Vec2(0.5f, 1.0f));
		_pLabelNextLevel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_pHeroInfoBoard->getContentSize().height * 0.35f));
		m_pHeroInfoBoard->addChild(_pLabelNextLevel, 1);

		Label *_pNextLevelInfoLabel = Label::createWithBMFont("fonts/font_normal-export.fnt",
			StringUtils::format("HP: %d   Score : %d%%   Gold: %d%%", _nHealthPoint + 1, _nBonusScore + 1, _nBonusGold + 1));
		_pNextLevelInfoLabel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
		_pNextLevelInfoLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
		_pNextLevelInfoLabel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.5f, m_pHeroInfoBoard->getContentSize().height * 0.3f));
		m_pHeroInfoBoard->addChild(_pNextLevelInfoLabel, 1);

		Sprite *_pHPBackground2 = Sprite::create("UI/UI_main_menu/frame_hp.png");
		_pHPBackground2->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pHPBackground2->getContentSize().width * 0.2f);
		_pHPBackground2->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pHPBackground2->getContentSize().height * 0.05f);
		_pHPBackground2->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pHPBackground2->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.1f, m_pHeroInfoBoard->getContentSize().height * 0.17f);
		m_pHeroInfoBoard->addChild(_pHPBackground2, 0);

		Sprite *_pScoreBackground2 = Sprite::create("UI/UI_main_menu/frame_score.png");
		_pScoreBackground2->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pScoreBackground2->getContentSize().width * 0.3f);
		_pScoreBackground2->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pScoreBackground2->getContentSize().height * 0.05f);
		_pScoreBackground2->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pScoreBackground2->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.3f, m_pHeroInfoBoard->getContentSize().height * 0.17f);
		m_pHeroInfoBoard->addChild(_pScoreBackground2, 0);

		Sprite *_pGoldBackground2 = Sprite::create("UI/UI_main_menu/frame_gold.png");
		_pGoldBackground2->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pGoldBackground2->getContentSize().width * 0.3f);
		_pGoldBackground2->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pGoldBackground2->getContentSize().height * 0.05f);
		_pGoldBackground2->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pGoldBackground2->setPosition(m_pHeroInfoBoard->getContentSize().width * 0.6f, m_pHeroInfoBoard->getContentSize().height * 0.17f);
		m_pHeroInfoBoard->addChild(_pGoldBackground2, 0);
	}
	else {
		Sprite *_pMaxUpgrate = Sprite::create("UI/UI_main_menu/icon_max.png");
		_pMaxUpgrate->setScale(m_pHeroInfoBoard->getContentSize().height / _pMaxUpgrate->getContentSize().height * 0.2f);
		_pMaxUpgrate->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pMaxUpgrate->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width / 2, m_pHeroInfoBoard->getContentSize().height * 0.2f));
		m_pHeroInfoBoard->addChild(_pMaxUpgrate, 1);
	}

	//// character level bar
	//Sprite *_pLevelBar = Sprite::create("UI/UI_main_menu/level_hero_bar.png");
	//_pLevelBar->setScale(m_pHeroInfoBoard->getContentSize().width / _pLevelBar->getContentSize().width * 0.6f);
	//_pLevelBar->setAnchorPoint(Vec2(0.0f, 1.0f));
	//_pLevelBar->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.3f, m_pHeroInfoBoard->getContentSize().height * 0.7f));
	//m_pHeroInfoBoard->addChild(_pLevelBar, 1);

	//// level point
	//int _nCurrentScore = REF->getCurrentScore();
	//int _nMaxScore = JSHERO->getMaxScoreLevelX(_nCurrentLevel);
	//float _fScaleX = (float)_nCurrentScore / (float)_nMaxScore;
	//m_pLevelPoint = Sprite::create("UI/UI_main_menu/level_hero_point.png");
	//m_pLevelPoint->setScaleX(_pLevelBar->getContentSize().width / m_pLevelPoint->getContentSize().width * _fScaleX);
	//m_pLevelPoint->setScaleY(_pLevelBar->getContentSize().height / m_pLevelPoint->getContentSize().height);
	//m_pLevelPoint->setAnchorPoint(Vec2(0.0f, 0.0f));
	//m_pLevelPoint->setPosition(Vec2(0.0f, 0.0f));
	//_pLevelBar->addChild(m_pLevelPoint, 1);

	//// health point label
	//int _nHealthPoint = REF->getCurrentHealth();
	//m_pHealthPointLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("HP: %d", _nHealthPoint));
	//m_pHealthPointLabel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	//m_pHealthPointLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	//m_pHealthPointLabel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.2f, m_pHeroInfoBoard->getContentSize().height * 0.61f));
	//m_pHeroInfoBoard->addChild(m_pHealthPointLabel, 1);

	//// bonus score
	//int _nBonusScore = REF->getBonusScore();
	//m_pBonusScoreLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Score +%d%%", _nBonusScore));
	//m_pBonusScoreLabel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	//m_pBonusScoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	//m_pBonusScoreLabel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.47f, m_pHeroInfoBoard->getContentSize().height * 0.61f));
	//m_pHeroInfoBoard->addChild(m_pBonusScoreLabel, 1);

	//// bonus Gold
	//int _nBonusGold = REF->getBonusGold();
	//m_pBonusGoldLabel = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Gold +%d%%", _nBonusGold));
	//m_pBonusGoldLabel->setBMFontSize(m_pHeroInfoBoard->getContentSize().height * 0.05f);
	//m_pBonusGoldLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	//m_pBonusGoldLabel->setPosition(Vec2(m_pHeroInfoBoard->getContentSize().width * 0.75f, m_pHeroInfoBoard->getContentSize().height * 0.61f));
	//m_pHeroInfoBoard->addChild(m_pBonusGoldLabel, 1);
}

void MenuLayer::initBottomHeroMenu() {
	m_pBottomHeroLayer->removeAllChildrenWithCleanup(true);

	// hero face frame
	auto _pHeroFrame = Sprite::create("UI/UI_main_menu/bottom_bar.png");
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
		m_arHeroButton[i]->setScale(_fHeroHeight / _pHeroSprite->getContentSize().height);
		m_arHeroButton[i]->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_arHeroButton[i]->setPosition(Vec2(_fXPositionCounter, 0.0f));

		_fXPositionCounter += _fStepPositionX;
	}
	_fXPositionCounter -= _fStepPositionX;
	// button try
	auto _pTryHeroNormal = Sprite::create("UI/UI_main_menu/btn_try.png");
	auto _pTryHeroSelected = Sprite::create("UI/UI_main_menu/btn_try.png");
	_pTryHeroSelected->setColor(Color3B(128, 128, 128));
	auto _aTryHeroButton = MenuItemSprite::create(_pTryHeroNormal, _pTryHeroSelected, CC_CALLBACK_0(MenuLayer::buttonTryHeroHandle, this));
	_aTryHeroButton->setScale(m_pBottomHeroLayer->getContentSize().height / _aTryHeroButton->getContentSize().height);
	_aTryHeroButton->setAnchorPoint(Vec2(0.5f, 0.5f));
	_aTryHeroButton->setPosition((m_fButtonStartPosition + _fXPositionCounter + m_arHeroButton[4]->getContentSize().width * m_arHeroButton[4]->getScale()) / 2, 0.0f);

	// button unlock
	auto _pUnlockNormal = Sprite::create("UI/UI_main_menu/btn_unlock.png");
	auto _pUnlockSelected = Sprite::create("UI/UI_main_menu/btn_unlock.png");
	_pUnlockSelected->setColor(Color3B(128, 128, 128));
	auto _aUnlockButton = MenuItemSprite::create(_pUnlockNormal, _pUnlockSelected, CC_CALLBACK_0(MenuLayer::buttonUnlockHeroHandle, this));
	_aUnlockButton->setScaleX(m_pBottomHeroLayer->getContentSize().width / _aUnlockButton->getContentSize().width * 0.26f);
	_aUnlockButton->setScaleY(m_pBottomHeroLayer->getContentSize().height / _aUnlockButton->getContentSize().height);
	_aUnlockButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aUnlockButton->setPosition(m_fButtonStartPosition, 0.0f);

	REF->pointToCurrentHero(m_nIndexHeroPicked);
	if (!REF->getIsLockedHero()) {
		_aUnlockButton->setNormalImage(Sprite::create("UI/UI_main_menu/btn_upgrade.png"));
		auto _pTemp = Sprite::create("UI/UI_main_menu/btn_upgrade.png");
		_pTemp->setColor(Color3B(128, 128, 128));
		_aUnlockButton->setSelectedImage(_pTemp);
		auto _pTemp2 = Sprite::create("UI/UI_main_menu/btn_upgrade.png");
		_pTemp2->setColor(Color3B(128, 128, 128));
		_aUnlockButton->setDisabledImage(_pTemp2);
		_aUnlockButton->setCallback(CC_CALLBACK_0(MenuLayer::buttonUpgradeHeroHandle, this));
		_aTryHeroButton->setVisible(false);
		if (REF->getCurrentLevel() == JSHERO->getMaxLevel()) {
			_aUnlockButton->setEnabled(false);

			Sprite *_pMaxUpgrate = Sprite::create("UI/UI_main_menu/icon_max.png");
			_pMaxUpgrate->setScaleX(_aUnlockButton->getContentSize().width / _pMaxUpgrate->getContentSize().width * 0.8f);
			_pMaxUpgrate->setScaleY(_aUnlockButton->getContentSize().height / _pMaxUpgrate->getContentSize().height * 0.8f);
			_pMaxUpgrate->setAnchorPoint(Vec2(0.5f, 0.5f));
			_pMaxUpgrate->setPosition(Vec2(_aUnlockButton->getContentSize().width / 2, _aUnlockButton->getContentSize().height / 2));
			_aUnlockButton->addChild(_pMaxUpgrate, 1);
		}
		else {
			float _nUpgradeCost = JSHERO->getGoldUpgradeLevelX(m_nIndexHeroPicked);
			for (int i = 1; i < REF->getCurrentLevel(); i++) {
				_nUpgradeCost *= 1.05f;
			}
			Label *_pLabelCost = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%d ", (int)(_nUpgradeCost)));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(0.6f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.5f, _aUnlockButton->getContentSize().height * 0.5f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
			_pCoinSprite->setScale(_aUnlockButton->getContentSize().height / _pCoinSprite->getContentSize().height * 0.3f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pCoinSprite->setPosition(Vec2(_pLabelCost->getContentSize().width, 0.0f));
			_pLabelCost->addChild(_pCoinSprite, 1);
		}
	}
	else {
		if (JSHERO->getGoldPrice() > 0) {
			Label *_pLabelCost = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%d ", JSHERO->getGoldPrice()));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(0.6f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.5f, _aUnlockButton->getContentSize().height * 0.5f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
			_pCoinSprite->setScale(_aUnlockButton->getContentSize().height / _pCoinSprite->getContentSize().height * 0.3f);
			_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pCoinSprite->setPosition(Vec2(_pLabelCost->getContentSize().width, 0.0f));
			_pLabelCost->addChild(_pCoinSprite, 1);
		}
		else if (JSHERO->getDiamondPrice() > 0) {
			Label *_pLabelCost = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%d ", JSHERO->getDiamondPrice()));
			_pLabelCost->setBMFontSize(_aUnlockButton->getContentSize().height * 0.3f);
			_pLabelCost->setAnchorPoint(Vec2(0.6f, 0.0f));
			_pLabelCost->setPosition(Vec2(_aUnlockButton->getContentSize().width * 0.5f, _aUnlockButton->getContentSize().height * 0.5f));
			_aUnlockButton->addChild(_pLabelCost, 1);

			Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/icon_diamond.png");
			_pDiamondSprite->setScale(_aUnlockButton->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.3f);
			_pDiamondSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
			_pDiamondSprite->setPosition(Vec2(_pLabelCost->getContentSize().width, 0.0f));
			_pLabelCost->addChild(_pDiamondSprite, 1);
		}
	}

	m_pBottomHeroMenu = Menu::create(m_arHeroButton[0], m_arHeroButton[1], m_arHeroButton[2], m_arHeroButton[3], m_arHeroButton[4], _aTryHeroButton, _aUnlockButton, NULL);
	m_pBottomHeroMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 27% screen height
	m_pBottomHeroMenu->setPosition(0.0f, 0.0f);
	m_pBottomHeroLayer->addChild(m_pBottomHeroMenu, 2);
}

void MenuLayer::backFunction() {
	int a = 0;
}

void MenuLayer::showMainMenu() {
	moveLayerViaDirection(m_pBottomMainLayer, 8);
	moveLayerViaDirection(m_pItemBoard, 4);
}

void MenuLayer::hideMainMenu() {
	moveLayerViaDirection(m_pBottomMainLayer, 2);
	moveLayerViaDirection(m_pItemBoard, 6);
}

void MenuLayer::showBlurScreen(int p_nOption) {
	m_pTopMenu->setEnabled(false);
	m_pBottomMainMenu->setEnabled(false);
	m_pItemBoardMenu->setEnabled(false);
	m_pSkillBoardMenu->setEnabled(false);
	m_pBottomHeroMenu->setEnabled(false);
	m_pQuestBoardMenu->setEnabled(false);
	m_pBlurScreen->setVisible(true);

	Sprite *_pBlurBlackLayer = Sprite::create("UI/toast.png");
	_pBlurBlackLayer->setScale(m_pBlurScreen->getContentSize().width / _pBlurBlackLayer->getContentSize().width,
		m_pBlurScreen->getContentSize().height / _pBlurBlackLayer->getContentSize().height);
	_pBlurBlackLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pBlurBlackLayer->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
	_pBlurBlackLayer->setOpacity(150);
	m_pBlurScreen->addChild(_pBlurBlackLayer, 0);

	// button close buy gold dialog
	auto _pCloseNormal = Sprite::create("UI/UI_Endgame/btn_close.png");
	auto _pCloseSelected = Sprite::create("UI/UI_Endgame/btn_close.png");
	_pCloseSelected->setColor(Color3B(128, 128, 128));
	auto _aCloseButton = MenuItemSprite::create(_pCloseNormal, _pCloseSelected, CC_CALLBACK_0(MenuLayer::hideBlurScreen, this));
	_aCloseButton->setScale(m_pBlurScreen->getContentSize().height / _aCloseButton->getContentSize().height * 0.1f);
	_aCloseButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	if (p_nOption == 0) {
		_aCloseButton->setAnchorPoint(Vec2(0.0f, 1.0f));
		_aCloseButton->setPosition(m_pBlurScreen->getContentSize().width * 0.25f,
			m_pBlurScreen->getContentSize().height * 0.75f);
	}
	else if (p_nOption == 1) {
		_aCloseButton->setPosition(m_pBlurScreen->getContentSize().width - m_pBlurScreen->getContentSize().height * 0.05f,
			m_pBlurScreen->getContentSize().height * 0.95f);
	}
	else if (p_nOption == 2) {
		_aCloseButton->setPosition(0.0f, 0.0f);
	}

	Menu *_pBuyGoldDialogMenu = Menu::create(_aCloseButton, NULL);
	_pBuyGoldDialogMenu->setContentSize(Size(m_pBlurScreen->getContentSize().width, m_pBlurScreen->getContentSize().height));
	_pBuyGoldDialogMenu->setPosition(0.0f, 0.0f);
	m_pBlurScreen->addChild(_pBuyGoldDialogMenu, 2);

	moveLayerViaDirection(m_pBlurScreen, 2);
}

void MenuLayer::hideBlurScreen() {
	m_pTopMenu->setEnabled(true);
	m_pBottomMainMenu->setEnabled(true);
	m_pItemBoardMenu->setEnabled(true);
	m_pSkillBoardMenu->setEnabled(true);
	m_pBottomHeroMenu->setEnabled(true);
	m_pQuestBoardMenu->setEnabled(true);

	moveLayerViaDirection(m_pBlurScreen, 8);
	runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
		m_pBlurScreen->removeAllChildrenWithCleanup(true);
		m_pBlurScreen->setVisible(false);
	}), nullptr));
}

void MenuLayer::buttonStartHandle() {
	REF->setLastPickHero(m_nIndexHeroSelected);
	auto _scene = SelectStageLayer::createScene(m_nIndexHeroSelected);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _scene));
}

void MenuLayer::buttonBackHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nMenuStatus == 0) {
		auto _aIntroScene = SceneIntro::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aIntroScene));
	}
	if (m_nMenuStatus == 1) {
		m_nMenuStatus = 0;
		moveLayerViaDirection(m_pQuestBoard, 4);
		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(0.0f, m_szVisibleSize.height * 0.2f)));
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
		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(0.0f, m_szVisibleSize.height * 0.2f)));
		}), nullptr));
		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			showMainMenu();
		}), nullptr));
	}
}

void MenuLayer::buttonAddLifeHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	showBlurScreen(0);

	Sprite *_pBuyGoldBackground = Sprite::create("UI/UI_main_menu/test_buy_pack_dialog.png");
	_pBuyGoldBackground->setScale(m_pBlurScreen->getContentSize().width / _pBuyGoldBackground->getContentSize().width * 0.5f,
		m_pBlurScreen->getContentSize().height / _pBuyGoldBackground->getContentSize().height * 0.5f);
	_pBuyGoldBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pBuyGoldBackground->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
	m_pBlurScreen->addChild(_pBuyGoldBackground, 1);

	// button buy
	auto _pBuyLifeNormal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyLifeSelected = Sprite::create("UI/UI_main_menu/btn_buy_2.png");
	auto _aBuyLifeButton = MenuItemSprite::create(_pBuyLifeNormal, _pBuyLifeSelected, CC_CALLBACK_0(MenuLayer::buttonBuyLifeHandle, this));
	_aBuyLifeButton->setScale(m_pBlurScreen->getContentSize().height / _aBuyLifeButton->getContentSize().height * 0.1f);
	_aBuyLifeButton->setAnchorPoint(Vec2(0.0f, 1.0f));
	_aBuyLifeButton->setPosition(m_pBlurScreen->getContentSize().width * 0.6f, m_pBlurScreen->getContentSize().height * 0.4f);

	Menu *_pBuyLifeDialogMenu = Menu::create(_aBuyLifeButton, NULL);
	_pBuyLifeDialogMenu->setContentSize(Size(m_pBlurScreen->getContentSize().width, m_pBlurScreen->getContentSize().height));
	_pBuyLifeDialogMenu->setPosition(0.0f, 0.0f);
	m_pBlurScreen->addChild(_pBuyLifeDialogMenu, 2);
}

void MenuLayer::buttonAddGoldHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	showBlurScreen(1);
	initBuyGoldAndDiamondBoard();

	CustomSpriteToBuyPack *_pBuyGoldPack[5];
	for (int i = 0; i < 5; i++) {
		JSMENU->readGoldPack(i);
		_pBuyGoldPack[i] = CustomSpriteToBuyPack::create("UI/UI_main_menu/pack_background.png", CC_CALLBACK_0(MenuLayer::buttonBuyCoinHandle, this, i), m_pPacksZone);
		_pBuyGoldPack[i]->setScale(m_pPacksZone->getContentSize().height / _pBuyGoldPack[i]->getContentSize().height * 0.9f);
		_pBuyGoldPack[i]->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pBuyGoldPack[i]->setPosition(Vec2(_pBuyGoldPack[i]->getContentSize().width * _pBuyGoldPack[i]->getScaleX() * (i + 0.5f) + m_pPacksZone->getContentSize().height * 0.05f * (i + 1),
			m_pPacksZone->getContentSize().height * 0.5f));
		m_pPacksZone->addChild(_pBuyGoldPack[i], 1);
		m_pPacksZone->setInnerContainerSize(Size(_pBuyGoldPack[i]->getContentSize().width * _pBuyGoldPack[i]->getScaleX() * 5 + m_pPacksZone->getContentSize().height * 0.05f * 6, m_pPacksZone->getContentSize().height));

		// gold pack name
		Label *_pLabelPackName = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", JSMENU->getGoldPackName().c_str()));
		_pLabelPackName->setBMFontSize(_pBuyGoldPack[i]->getContentSize().height * 0.2f);
		_pLabelPackName->setAnchorPoint(Vec2(0.5f, 0.0f));
		_pLabelPackName->setPosition(Vec2(_pBuyGoldPack[i]->getContentSize().width * 0.5f, _pBuyGoldPack[i]->getContentSize().height * 0.8f));
		_pBuyGoldPack[i]->addChild(_pLabelPackName, 1);

		// coin number
		Label *_pLabelCoinNumber = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format(" %d", JSMENU->getCoinPackNumberGold()));
		_pLabelCoinNumber->setBMFontSize(_pBuyGoldPack[i]->getContentSize().height * 0.1f);
		_pLabelCoinNumber->setAnchorPoint(Vec2(0.4f, 0.0f));
		_pLabelCoinNumber->setPosition(Vec2(_pBuyGoldPack[i]->getContentSize().width * 0.5f, _pBuyGoldPack[i]->getContentSize().height * 0.7f));
		_pBuyGoldPack[i]->addChild(_pLabelCoinNumber, 1);
		Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
		_pCoinSprite->setScale(_pBuyGoldPack[i]->getContentSize().height / _pCoinSprite->getContentSize().height * 0.1f);
		_pCoinSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_pCoinSprite->setPosition(Vec2(0.0f, 0.0f));
		_pLabelCoinNumber->addChild(_pCoinSprite, 1);

		// icon pack
		Sprite *_pPackSprite = Sprite::create(JSMENU->getIconGoldPackPath());
		_pPackSprite->setScale(_pBuyGoldPack[i]->getContentSize().height / _pPackSprite->getContentSize().height * 0.5f);
		_pPackSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pPackSprite->setPosition(Vec2(_pBuyGoldPack[i]->getContentSize().width * 0.5f, _pBuyGoldPack[i]->getContentSize().height * 0.45f));
		_pBuyGoldPack[i]->addChild(_pPackSprite, 1);

		// diamond cost
		Label *_pLabelDiamondCost = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%d ", JSMENU->getCoinPackDiamondPrice()));
		_pLabelDiamondCost->setBMFontSize(_pBuyGoldPack[i]->getContentSize().height * 0.15f);
		_pLabelDiamondCost->setAnchorPoint(Vec2(0.6f, 0.0f));
		_pLabelDiamondCost->setPosition(Vec2(_pBuyGoldPack[i]->getContentSize().width * 0.5f, _pBuyGoldPack[i]->getContentSize().height * 0.05f));
		_pBuyGoldPack[i]->addChild(_pLabelDiamondCost, 1);
		Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/icon_diamond.png");
		_pDiamondSprite->setScale(_pBuyGoldPack[i]->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.15f);
		_pDiamondSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pDiamondSprite->setPosition(Vec2(_pLabelDiamondCost->getContentSize().width, 0.0f));
		_pLabelDiamondCost->addChild(_pDiamondSprite, 1);
	}
}

void MenuLayer::buttonAddDiamondHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	showBlurScreen(1);
	initBuyGoldAndDiamondBoard();

	CustomSpriteToBuyPack *_pBuyDiamondPack[5];
	for (int i = 0; i < 5; i++) {
		JSMENU->readDiamondPack(i);
		_pBuyDiamondPack[i] = CustomSpriteToBuyPack::create("UI/UI_main_menu/pack_background.png", CC_CALLBACK_0(MenuLayer::buttonBuyDiamondHandle, this, i), m_pPacksZone);
		_pBuyDiamondPack[i]->setScale(m_pPacksZone->getContentSize().height / _pBuyDiamondPack[i]->getContentSize().height * 0.9f);
		_pBuyDiamondPack[i]->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pBuyDiamondPack[i]->setPosition(Vec2(_pBuyDiamondPack[i]->getContentSize().width * _pBuyDiamondPack[i]->getScaleX() * (i + 0.5f) + m_pPacksZone->getContentSize().height * 0.05f * (i + 1),
			m_pPacksZone->getContentSize().height * 0.5f));
		m_pPacksZone->addChild(_pBuyDiamondPack[i], 1);
		m_pPacksZone->setInnerContainerSize(Size(_pBuyDiamondPack[i]->getContentSize().width * _pBuyDiamondPack[i]->getScaleX() * 5 + m_pPacksZone->getContentSize().height * 0.05f * 6, m_pPacksZone->getContentSize().height));

		// gold pack name
		Label *_pLabelPackName = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%s", JSMENU->getDiamondPackName().c_str()));
		_pLabelPackName->setBMFontSize(_pBuyDiamondPack[i]->getContentSize().height * 0.2f);
		_pLabelPackName->setAnchorPoint(Vec2(0.5f, 0.0f));
		_pLabelPackName->setPosition(Vec2(_pBuyDiamondPack[i]->getContentSize().width * 0.5f, _pBuyDiamondPack[i]->getContentSize().height * 0.8f));
		_pBuyDiamondPack[i]->addChild(_pLabelPackName, 1);

		// diamond number
		Label *_pLabelDiamondNumber = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format(" %d", JSMENU->getDiamondPackNumberDiamond()));
		_pLabelDiamondNumber->setBMFontSize(_pBuyDiamondPack[i]->getContentSize().height * 0.1f);
		_pLabelDiamondNumber->setAnchorPoint(Vec2(0.4f, 0.0f));
		_pLabelDiamondNumber->setPosition(Vec2(_pBuyDiamondPack[i]->getContentSize().width * 0.5f, _pBuyDiamondPack[i]->getContentSize().height * 0.7f));
		_pBuyDiamondPack[i]->addChild(_pLabelDiamondNumber, 1);
		Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/icon_diamond.png");
		_pDiamondSprite->setScale(_pBuyDiamondPack[i]->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.1f);
		_pDiamondSprite->setAnchorPoint(Vec2(1.0f, 0.0f));
		_pDiamondSprite->setPosition(Vec2(0.0f, 0.0f));
		_pLabelDiamondNumber->addChild(_pDiamondSprite, 1);

		// icon pack
		Sprite *_pPackSprite = Sprite::create(JSMENU->getIconDiamondPackPath());
		_pPackSprite->setScale(_pBuyDiamondPack[i]->getContentSize().height / _pPackSprite->getContentSize().height * 0.5f);
		_pPackSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pPackSprite->setPosition(Vec2(_pBuyDiamondPack[i]->getContentSize().width * 0.5f, _pBuyDiamondPack[i]->getContentSize().height * 0.45f));
		_pBuyDiamondPack[i]->addChild(_pPackSprite, 1);

		// diamond cost
		Label *_pLabelDiamondCost = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("%d ", JSMENU->getDiamondPackMoneyPrice()));
		_pLabelDiamondCost->setBMFontSize(_pBuyDiamondPack[i]->getContentSize().height * 0.15f);
		_pLabelDiamondCost->setAnchorPoint(Vec2(0.6f, 0.0f));
		_pLabelDiamondCost->setPosition(Vec2(_pBuyDiamondPack[i]->getContentSize().width * 0.5f, _pBuyDiamondPack[i]->getContentSize().height * 0.05f));
		_pBuyDiamondPack[i]->addChild(_pLabelDiamondCost, 1);
		Sprite *_pMoneySprite = Sprite::create("UI/UI_main_menu/icon_diamond.png");
		_pMoneySprite->setScale(_pBuyDiamondPack[i]->getContentSize().height / _pMoneySprite->getContentSize().height * 0.15f);
		_pMoneySprite->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pMoneySprite->setPosition(Vec2(_pLabelDiamondCost->getContentSize().width, 0.0f));
		_pLabelDiamondCost->addChild(_pMoneySprite, 1);
	}
}

void MenuLayer::initBuyGoldAndDiamondBoard() {
	Sprite *_pBuyGoldBackground = Sprite::create("UI/UI_main_menu/test_buy_pack_dialog.png");
	_pBuyGoldBackground->setScale(m_pBlurScreen->getContentSize().width / _pBuyGoldBackground->getContentSize().width,
		m_pBlurScreen->getContentSize().height / _pBuyGoldBackground->getContentSize().height);
	_pBuyGoldBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	_pBuyGoldBackground->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
	m_pBlurScreen->addChild(_pBuyGoldBackground, 1);

	Label *_pDialogName = Label::createWithBMFont("fonts/font_normal-export.fnt", StringUtils::format("Available Packs"));
	_pDialogName->setBMFontSize(m_pBlurScreen->getContentSize().height * 0.1f);
	_pDialogName->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pDialogName->setPosition(Vec2(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.85f));
	m_pBlurScreen->addChild(_pDialogName, 1);

	Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_pCoinSprite->setScale(m_pBlurScreen->getContentSize().height / _pCoinSprite->getContentSize().height * 0.1f * 0.8f);
	_pCoinSprite->setAnchorPoint(Vec2(1.0f, 0.5f));
	_pCoinSprite->setPosition(Vec2(m_pBlurScreen->getContentSize().width * 0.375f, m_pBlurScreen->getContentSize().height * 0.15f));
	m_pBlurScreen->addChild(_pCoinSprite, 1);

	m_pLabelNumberGoldOnBuy = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", m_nCurrentGold));
	m_pLabelNumberGoldOnBuy->setBMFontSize(_pCoinSprite->getContentSize().height);
	m_pLabelNumberGoldOnBuy->setAlignment(TextHAlignment::RIGHT);
	m_pLabelNumberGoldOnBuy->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_pLabelNumberGoldOnBuy->setPosition(Vec2(0.0f, 0.0f));
	_pCoinSprite->addChild(m_pLabelNumberGoldOnBuy, 1);

	Sprite *_pDiamondSprite = Sprite::create("UI/UI_main_menu/icon_diamond.png");
	_pDiamondSprite->setScale(m_pBlurScreen->getContentSize().height / _pDiamondSprite->getContentSize().height * 0.1f * 0.8f);
	_pDiamondSprite->setAnchorPoint(Vec2(1.0f, 0.5f));
	_pDiamondSprite->setPosition(Vec2(m_pBlurScreen->getContentSize().width * 0.875f, m_pBlurScreen->getContentSize().height * 0.15f));
	m_pBlurScreen->addChild(_pDiamondSprite, 1);

	m_pLabelNumberDiamondOnBuy = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", m_nCurrentDiamond));
	m_pLabelNumberDiamondOnBuy->setBMFontSize(_pDiamondSprite->getContentSize().height);
	m_pLabelNumberDiamondOnBuy->setAlignment(TextHAlignment::RIGHT);
	m_pLabelNumberDiamondOnBuy->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_pLabelNumberDiamondOnBuy->setPosition(Vec2(0.0f, 0.0f));
	_pDiamondSprite->addChild(m_pLabelNumberDiamondOnBuy, 1);

	m_pPacksZone = ScrollView::create();
	m_pPacksZone->setContentSize(Size(m_pBlurScreen->getContentSize().width, m_pBlurScreen->getContentSize().height * 0.6f));
	m_pPacksZone->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_pPacksZone->setPosition(Vec2(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f));
	m_pPacksZone->setDirection(ScrollView::Direction::HORIZONTAL);
	m_pPacksZone->setBounceEnabled(true);
	m_pPacksZone->setTouchEnabled(true);
	m_pPacksZone->setScrollBarOpacity(0.0f);
	m_pBlurScreen->addChild(m_pPacksZone, 1);
}

void MenuLayer::buttonQuestHandle() {
	if (m_nMenuStatus != 1) {
		m_nMenuStatus = 1;
		hideMainMenu();

		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height * 0.1f)));
		}), nullptr));

		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pQuestBoard, 6);
		}), nullptr));
	}
}

void MenuLayer::buttonHeroesHandle() {
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nIndexHeroSelected != m_nIndexHeroPicked) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
	}
	initBottomHeroMenu();
	if (m_nMenuStatus != 2) {
		m_nMenuStatus = 2;
		hideMainMenu();

		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(m_szVisibleSize.width / 5, m_szVisibleSize.height * 0.2f)));
		}), nullptr));

		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pUpgradeBoard, 4);
			moveLayerViaDirection(m_pHeroInfoBoard, 6);
			moveLayerViaDirection(m_pBottomHeroLayer, 8);
		}), nullptr));
	}
}

void MenuLayer::buttonFreeCoinHandle() {
	if (REF->getFreeCoin() > 0) {
		REF->decreaseFreeCoin();
		if (REF->getFreeCoin() <= 0) {
			m_pSpriteFreeCoinAttention->setVisible(false);
		}
		// TODO : show ads and check view ads finish
		// after that, increase gold
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create("You've take free coin 3 times today, comeback on tomorrow to get more", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pGameControl->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonMoreGameHandle() {
	CustomLayerToToast *_pToast = CustomLayerToToast::create("Come to Pipo game to get more", TOAST_LONG);
	_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
	m_pGameControl->addChild(_pToast, 10);
	return;
}

void MenuLayer::buttonUpgradeSkillHandle(int p_nIndexSkill) {
	if (p_nIndexSkill == 0) {
		REF->increaseLevelSkill_1();
	}
	if (p_nIndexSkill == 1) {
		REF->increaseLevelSkill_2();
	}
	if (p_nIndexSkill == 2) {
		REF->increaseLevelSkill_3();
	}
	initUpgradeBoard();
}

void MenuLayer::buttonBuyItemHandle(int p_nIndexItem) {
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
			REF->increaseNumberItemHealth();
			m_arBuyItemButton[0]->setVisible(false);
			m_arSpriteItemMax[0]->setVisible(true);
			break;
		}
		case 1: {
			REF->increaseNumberItemBird();
			m_arBuyItemButton[1]->setVisible(false);
			m_arSpriteItemMax[1]->setVisible(true);
			break;
		}
		case 2: {
			REF->increaseNumberItemMagnet();
			m_arBuyItemButton[2]->setVisible(false);
			m_arSpriteItemMax[2]->setVisible(true);
			break;
		}
		case 3: {
			REF->increaseNumberItemDoubleGold();
			m_arBuyItemButton[3]->setVisible(false);
			m_arSpriteItemMax[3]->setVisible(true);
			break;
		}
		case 4: {
			REF->increaseNumberItemCoolDown();
			m_arBuyItemButton[4]->setVisible(false);
			m_arSpriteItemMax[4]->setVisible(true);
			break;
		}
		default: {
			break;
		}
		}
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create("You dont have enough gold, get more to buy.", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pBlurScreen->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonRewardQuest(int p_nQuestIndex) {
	// update gold by reward
	int _nGoldReward = JSQUEST->getGoldReward();
	for (int j = 0; j < REF->getRewardedQuestTimes(); j++) {
		_nGoldReward *= JSQUEST->getStepRequest();
	}
	m_nCurrentGold += _nGoldReward;

	REF->updateRewardedQuestTimes(p_nQuestIndex);
	REF->setUpGoldExplored(_nGoldReward);
	initQuestBoard(p_nQuestIndex);
	initTopMainMenu();
}

bool MenuLayer::createRequestToGoogle() {
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("http://www.google.com.vn");
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
		// CCLOG("%s", ("Response message: " + _sResult).c_str());
		m_nCurrentTimeFromGoogle = calTimeFromString(_sResult);
		initDailyRewardBoard();
	}
	else {
		// CCLOG("%s", ("Error " + to_string(p_pResponse->getResponseCode()) + " in request").c_str());
		CustomLayerToToast *_pToast = CustomLayerToToast::create("Turn on your internet connection to get daily reward information", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pGameControl->addChild(_pToast, 10);
	}
}

int MenuLayer::calTimeFromString(string p_sInputString) {
	int _nYear = (p_sInputString.at(37) - 48) * 10 + (p_sInputString.at(38) - 48) * 1;
	int _nMonth = 0;
	string _sMonth = p_sInputString.substr(31, 3);
	string _arMonths[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	for (int i = 0; i < 12; i++) {
		if (_sMonth.compare(_arMonths[i]) == 0) {
			_nMonth = i;
			break;
		}
	}
	int _nDay = (p_sInputString.at(28) - 48) * 10 + (p_sInputString.at(29) - 48) * 1;
	int _nHour = (p_sInputString.at(40) - 48) * 10 + (p_sInputString.at(41) - 48) * 1;
	int _nMinute = (p_sInputString.at(43) - 48) * 10 + (p_sInputString.at(44) - 48) * 1;
	int _nSecond = (p_sInputString.at(46) - 48) * 10 + (p_sInputString.at(47) - 48) * 1;

	tm _pTempTime;
	_pTempTime.tm_year = _nYear + 100;
	_pTempTime.tm_mon = _nMonth;
	_pTempTime.tm_mday = _nDay;
	_pTempTime.tm_hour = _nHour;
	_pTempTime.tm_min = _nMinute;
	_pTempTime.tm_sec = _nSecond;

	return mktime(&_pTempTime);
}

void MenuLayer::initDailyRewardBoard() {
	time_t _nLastDailyRewardTime = REF->getLastDailyRewardTime();
	time_t _nCurrentTimeFromGoogle = m_nCurrentTimeFromGoogle;
	tm *_pCurrentTimeFromGoogle = localtime(&_nCurrentTimeFromGoogle);
	_nCurrentTimeFromGoogle -= _pCurrentTimeFromGoogle->tm_hour * 3600 + _pCurrentTimeFromGoogle->tm_min * 60 + _pCurrentTimeFromGoogle->tm_sec;

	if (_nLastDailyRewardTime < _nCurrentTimeFromGoogle) { // if realtime from google.vn "greater than" last daily reward time "more than 1 day"
		REF->updateDailyRewardAvailable(true);
		REF->resetFreeCoin();
		REF->updateTimeFromGoogle(_nCurrentTimeFromGoogle);
	}
	if (REF->getDailyRewardAvailable()) { // if daily reward is available
		showBlurScreen(2); // open daily reward

		Sprite *_pDailyRewardBackground = Sprite::create("UI/UI_main_menu/DailyReward/test_daily_reward.png");
		_pDailyRewardBackground->setScale(m_pBlurScreen->getContentSize().width / _pDailyRewardBackground->getContentSize().width * 0.6f,
			m_pBlurScreen->getContentSize().height / _pDailyRewardBackground->getContentSize().height * 0.6f);
		_pDailyRewardBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
		_pDailyRewardBackground->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.5f);
		m_pBlurScreen->addChild(_pDailyRewardBackground, 1);

		// button reward
		auto _pRewardNormal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
		auto _pRewardSelected = Sprite::create("UI/UI_main_menu/btn_buy_2.png");
		auto _aRewardButton = MenuItemSprite::create(_pRewardNormal, _pRewardSelected, CC_CALLBACK_0(MenuLayer::buttonDailyRewardHandle, this));
		_aRewardButton->setScale(m_pBlurScreen->getContentSize().height / _aRewardButton->getContentSize().height * 0.1f);
		_aRewardButton->setAnchorPoint(Vec2(0.5f, 0.0f));
		_aRewardButton->setPosition(m_pBlurScreen->getContentSize().width * 0.5f, m_pBlurScreen->getContentSize().height * 0.2f);

		Menu *_pDailyRewardDialogMenu = Menu::create(_aRewardButton, NULL);
		_pDailyRewardDialogMenu->setContentSize(Size(m_pBlurScreen->getContentSize().width, m_pBlurScreen->getContentSize().height));
		_pDailyRewardDialogMenu->setPosition(0.0f, 0.0f);
		m_pBlurScreen->addChild(_pDailyRewardDialogMenu, 2);
	}
}

void MenuLayer::buttonDailyRewardHandle() {
	hideBlurScreen();

	//
	JSQUEST->readDailyReward(REF->getDailyRewardCounter());
	string a = JSQUEST->getDailyRewardType();
	if (JSQUEST->getDailyRewardType().compare("gold") == 0) {
		m_nCurrentGold += JSQUEST->getDailyRewardNumber();
		REF->setUpGoldExplored(JSQUEST->getDailyRewardNumber());
	}
	if (JSQUEST->getDailyRewardType().compare("play_turn") == 0) {
		m_nLifeNumber += JSQUEST->getDailyRewardNumber();
		REF->setUpLife(JSQUEST->getDailyRewardNumber());
	}
	if (JSQUEST->getDailyRewardType().compare("item_bird") == 0) {
		m_arNumberItemOwning[1] += JSQUEST->getDailyRewardNumber();
		REF->increaseNumberItemBird();
		m_arBuyItemButton[1]->setVisible(false);
		m_arSpriteItemMax[1]->setVisible(true);
	}
	if (JSQUEST->getDailyRewardType().compare("diamond") == 0) {
		m_nCurrentDiamond += JSQUEST->getDailyRewardNumber();
		REF->setUpDiamondBuy(JSQUEST->getDailyRewardNumber());
	}
	initTopMainMenu();
	REF->increaseDailyRewardCounter();
	REF->updateDailyRewardAvailable(false);
}

void MenuLayer::buttonBuyLifeHandle() {
	if (m_nCurrentDiamond >= 5) {
		m_nCurrentDiamond -= 5;
		m_nLifeNumber += 5;
		REF->setDownDiamond(5);
		REF->setUpLife(5);
		initTopMainMenu();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create("You dont have enough diamond, get more to buy.", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pBlurScreen->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonBuyCoinHandle(int p_nIndexCoinPack) {
	JSMENU->readGoldPack(p_nIndexCoinPack);
	if (JSMENU->getCoinPackDiamondPrice() <= m_nCurrentDiamond) {
		m_nCurrentDiamond -= JSMENU->getCoinPackDiamondPrice();
		m_nCurrentGold += JSMENU->getCoinPackNumberGold();
		m_pLabelNumberDiamondOnBuy->setString((String::createWithFormat("%d", m_nCurrentDiamond))->getCString());
		m_pLabelNumberGoldOnBuy->setString((String::createWithFormat("%d", m_nCurrentGold))->getCString());
		REF->setDownDiamond(JSMENU->getCoinPackDiamondPrice());
		REF->setUpGoldExplored(JSMENU->getCoinPackNumberGold());
		initTopMainMenu();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create("You dont have enough diamond, get more to buy.", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pBlurScreen->addChild(_pToast, 10);
	}
}

void MenuLayer::buttonBuyDiamondHandle(int p_nIndexDiamondPack) {
	JSMENU->readDiamondPack(p_nIndexDiamondPack);
	if (false) {
		return;
	}
	m_nCurrentDiamond += JSMENU->getDiamondPackNumberDiamond();
	m_pLabelNumberDiamondOnBuy->setString((String::createWithFormat("%d", m_nCurrentDiamond))->getCString());
	REF->setUpDiamondBuy(JSMENU->getDiamondPackNumberDiamond());
	initTopMainMenu();

	REF->setUpNumberQuest(7, JSMENU->getDiamondPackNumberDiamond());
	initQuestBoard(0);
}

void MenuLayer::buttonPickHeroHandle(int p_nIndexHero) {
	if (m_nIndexHeroPicked == p_nIndexHero) {
		return;
	}
	m_nIndexHeroPicked = p_nIndexHero;
	REF->pointToCurrentHero(m_nIndexHeroPicked);
	if (!REF->getIsLockedHero()) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
		REF->setSelectedHero(m_nIndexHeroPicked);
	}
	initSceneLayer();
	initBottomHeroMenu();
	initHeroInfoBoard();
	initUpgradeBoard();
}

void MenuLayer::buttonTryHeroHandle() {
	auto _scene = LoadingLayer::createScene(1, 1, m_nIndexHeroPicked);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _scene));
}

void MenuLayer::buttonUnlockHeroHandle() {
	REF->pointToCurrentHero(m_nIndexHeroPicked - 1); // point to pre-hero
	int _nPreHeroLevel = REF->getCurrentLevel(); // get level of pre-hero
	if (_nPreHeroLevel < m_nIndexHeroPicked * 20) {
		REF->pointToCurrentHero(m_nIndexHeroPicked);
		string _arTipToUnlockHero[5] = {
			"This hero is unlocked!",
			"Get Johan to level 20 to unlock Hana",
			"Get Hana to level 40 to unlock Kim",
			"Get Kim to level 60 to unlock Athur",
			"Get Athur to level 80 to unlock Robin"
		};
		CustomLayerToToast *_pToast = CustomLayerToToast::create(_arTipToUnlockHero[m_nIndexHeroPicked], TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pGameControl->addChild(_pToast, 10);

		return;
	}
	if (m_nCurrentGold >= JSHERO->getGoldPrice() && m_nCurrentDiamond >= JSHERO->getDiamondPrice()) {
		auto _aAnimSprite = Sprite::create("UI/UI_main_menu/effect_unlock_2.png");
		auto _aCache = AnimationCache::getInstance();
		Vector<SpriteFrame*> _arFrames(2);
		_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/effect_unlock_2.png",
			Rect(0, 0, _aAnimSprite->getContentSize().width, _aAnimSprite->getContentSize().height)));
		_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/effect_unlock_1.png",
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
}

void MenuLayer::buttonUpgradeHeroHandle() {
	float _nUpgradeCost = JSHERO->getGoldUpgradeLevelX(m_nIndexHeroPicked);
	for (int i = 0; i < REF->getCurrentLevel(); i++) {
		_nUpgradeCost *= 1.05f;
	}
	if (m_nCurrentGold >= _nUpgradeCost) {
		m_nCurrentGold -= _nUpgradeCost;
		REF->setDownGold(_nUpgradeCost);
		REF->increaseLevel();
		initTopMainMenu();
		initBottomHeroMenu();
		initHeroInfoBoard();
	}
	else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create("You dont have enough gold, get more to upgrade.", TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		m_pGameControl->addChild(_pToast, 10);
	}
}

void MenuLayer::createLayerViaInput(Layer *p_pLayer, Size p_v2Size, Vec2 p_v2Position) {
	p_pLayer = Layer::create();
	p_pLayer->setContentSize(p_v2Size);
	p_pLayer->setPosition(p_v2Position);
	m_pGameControl->addChild(p_pLayer, 1);
}

void MenuLayer::moveLayerViaDirection(Layer *p_pLayer, int p_nDirection) {
	switch (p_nDirection) {
	case 2:
		p_pLayer->runAction(MoveBy::create(0.3f, Vec2(0.0f, -p_pLayer->getContentSize().height)));
		break;
	case 4:
		p_pLayer->runAction(MoveBy::create(0.3f, Vec2(-p_pLayer->getContentSize().width, 0.0f)));
		break;
	case 6:
		p_pLayer->runAction(MoveBy::create(0.3f, Vec2(p_pLayer->getContentSize().width, 0.0f)));
		break;
	case 8:
		p_pLayer->runAction(MoveBy::create(0.3f, Vec2(0.0f, p_pLayer->getContentSize().height)));
		break;
	default:
		break;
	}
}