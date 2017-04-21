#include "MenuScene.h"
#include "GameScene.h"
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/bg.plist");
	log("%i", JSHERO->getMaxScoreLevelX(1));


	initInputData();

	m_pGameBackground = new Layer(); // layer 1 : background
	this->addChild(m_pGameBackground, 1);
	initBackgroundLayer();

	m_pGameScene = new Layer(); // layer 2 : scene
	this->addChild(m_pGameScene, 2);
	float _fScale = m_szVisibleSize.height / 2;
	m_pGameScene->setContentSize(Size(m_szVisibleSize.width * 0.6f, m_szVisibleSize.height * 0.65f));
	m_pGameScene->setPosition(0.0f, m_szVisibleSize.height * 0.2f);
	m_pPreviewHero0 = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_pPreviewHero1 = new SkeletonAnimation("UI/UI_main_menu/PreviewCoLong/s_CoCo.json",
		"UI/UI_main_menu/PreviewCoLong/s_CoCo.atlas", m_pGameScene->getContentSize().height / 650);
	m_pPreviewHero2 = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_pPreviewHero3 = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_pPreviewHero4 = new SkeletonAnimation("UI/UI_main_menu/PreviewDuongQua/s_DuongQua.json",
		"UI/UI_main_menu/PreviewDuongQua/s_DuongQua.atlas", m_pGameScene->getContentSize().height / 650);
	m_pPreviewHero0->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_pPreviewHero1->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_pPreviewHero2->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_pPreviewHero3->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	m_pPreviewHero4->setPosition(Vec2(m_pGameScene->getContentSize().width / 2, 0.0f));
	initSceneLayer();

	m_pGameControl = new Layer(); // layer 3 : control
	this->addChild(m_pGameControl, 3);
	initControlLayer();

	//this->scheduleUpdate();

	return true;
}

void MenuLayer::initInputData() {
	m_nIndexHeroSelected = m_nIndexHeroPicked = REF->getSelectedHero();
	m_nGold = REF->getGoldExplored();
	// = REF->getSelectedDiamond(); // dung de chi so kim cuong
}

void MenuLayer::initBackgroundLayer() {
	// background
	auto _aMainMenuBackground = Sprite::create("UI/UI_main_menu/bg_1.png");
	_aMainMenuBackground->setScaleX(m_szVisibleSize.width / _aMainMenuBackground->getContentSize().width); // full screen size width
	_aMainMenuBackground->setScaleY(m_szVisibleSize.height / _aMainMenuBackground->getContentSize().height); // full screen size height
	_aMainMenuBackground->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2)); // center screen
	m_pGameBackground->addChild(_aMainMenuBackground, 1);
}

void MenuLayer::initSceneLayer() {
	m_pGameScene->removeAllChildrenWithCleanup(true);

	switch (m_nIndexHeroSelected) {
	case 0:
		m_pPreviewHero0->setAnimation(0, "appear", false);
		m_pPreviewHero0->addAnimation(0, "idle", true);
		m_pGameScene->addChild(m_pPreviewHero0, 1);
		break;
	case 1:
		m_pPreviewHero1->setAnimation(0, "appear", false);
		m_pPreviewHero1->addAnimation(0, "idle", true);
		m_pGameScene->addChild(m_pPreviewHero1, 1);
		break;
	case 2:
		m_pPreviewHero2->setAnimation(0, "appear", false);
		m_pPreviewHero2->addAnimation(0, "idle", true);
		m_pGameScene->addChild(m_pPreviewHero2, 1);
		break;
	case 3:
		m_pPreviewHero3->setAnimation(0, "appear", false);
		m_pPreviewHero3->addAnimation(0, "idle", true);
		m_pGameScene->addChild(m_pPreviewHero3, 1);
		break;
	case 4:
		m_pPreviewHero4->setAnimation(0, "appear", false);
		m_pPreviewHero4->addAnimation(0, "idle", true);
		m_pGameScene->addChild(m_pPreviewHero4, 1);
		break;
	default:
		break;
	}
}

void MenuLayer::initControlLayer() {
	initTopMainMenu();
	initBottomMainMenu();
	initUpgrateBoard();
	initItemBoard();
	initHeroInfoBoard();

	// bottom hero menu
	m_pBottomHeroMenu = Layer::create();
	m_pBottomHeroMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 25% screen height
	m_pBottomHeroMenu->setPosition(0.0f, -m_pBottomHeroMenu->getContentSize().height / 2);
	m_pGameControl->addChild(m_pBottomHeroMenu, 1);
	initBottomHeroMenu();

	showMainMenu();
}

void MenuLayer::initTopMainMenu() {
	// top menu
	m_pTopMainMenu = Layer::create();
	m_pTopMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMainMenu->setPosition(m_szVisibleSize.width / 2, m_szVisibleSize.height + m_pTopMainMenu->getContentSize().height / 2);
	m_pGameControl->addChild(m_pTopMainMenu, 1);

	float _fPadding = m_pTopMainMenu->getContentSize().width / 50;
	float _fXPositionCounter = -m_pTopMainMenu->getContentSize().width / 2 + _fPadding / 2;

	// button back
	auto _pBackNormal = Sprite::create("UI/UI_main_menu/btn_back.png");
	auto _pBackSelected = Sprite::create("UI/UI_main_menu/btn_back.png");
	_pBackSelected->setColor(Color3B(128, 128, 128));
	auto _aBackButton = MenuItemSprite::create(_pBackNormal, _pBackSelected, CC_CALLBACK_0(MenuLayer::buttonBackHanle, this));
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

	_fXPositionCounter += 0.0f;

	// add life button
	auto _pAddLifeNormal = Sprite::create("UI/UI_main_menu/btn_add_life.png");
	auto _pAddLifeSelected = Sprite::create("UI/UI_main_menu/btn_add_life.png");
	_pAddLifeSelected->setColor(Color3B(128, 128, 128));
	auto _aAddLifeButton = MenuItemSprite::create(_pAddLifeNormal, _pAddLifeSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
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

	Label *_pLabelNumberGold;
	_pLabelNumberGold = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", m_nGold));
	_pLabelNumberGold->setBMFontSize(_pMoneyFrame->getBoundingBox().size.height * 0.8f);
	_pLabelNumberGold->setColor(Color3B::YELLOW);
	_pLabelNumberGold->setAnchorPoint(Vec2(1.0f, 0.0f));
	_pLabelNumberGold->setPosition(Vec2(_pMoneyFrame->getContentSize().width * _pMoneyFrame->getScaleX() * 0.9f, 0.0f));
	_pMoneyFrame->addChild(_pLabelNumberGold, 1);

	_fXPositionCounter += 0.0f;

	// add money button
	auto _pAddMoneyNormal = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	auto _pAddMoneySelected = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	_pAddMoneySelected->setColor(Color3B(128, 128, 128));
	auto _aAddMoneyButton = MenuItemSprite::create(_pAddMoneyNormal, _pAddMoneySelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
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

	_fXPositionCounter += 0.0f;

	// add diamond button
	auto _pAddDiamondNormal = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	auto _pAddDiamondSelected = Sprite::create("UI/UI_main_menu/btn_add_money_diamond.png");
	_pAddDiamondSelected->setColor(Color3B(128, 128, 128));
	auto _aAddDiamondButton = MenuItemSprite::create(_pAddDiamondNormal, _pAddDiamondSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aAddDiamondButton->setScale(_pDiamondFrame->getContentSize().width * _pLifeFrame->getScaleX() / _aAddDiamondButton->getContentSize().width);
	_aAddDiamondButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aAddDiamondButton->setPosition(_fXPositionCounter, 0.0f);

	auto _aTopMenu = Menu::create(_aBackButton, _aAddLifeButton, _aAddMoneyButton, _aAddDiamondButton, NULL);
	_aTopMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	_aTopMenu->setPosition(0.0f, 0.0f);
	m_pTopMainMenu->addChild(_aTopMenu, 2);
}

void MenuLayer::initBottomMainMenu() {
	// bottom main menu
	m_pBottomMainMenu = Layer::create();
	m_pBottomMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 25% screen height
	m_pBottomMainMenu->setPosition(0.0f, -m_pBottomMainMenu->getContentSize().height / 2);
	m_pGameControl->addChild(m_pBottomMainMenu, 1);

	float _fPadding = 0.0f; // m_szVisibleSize.width / 50;
	float _fXPositionCounter = _fPadding / 2;

	// button quest
	auto _pQuestNormal = Sprite::create("UI/UI_main_menu/btn_quest.png");
	auto _pQuestSelected = Sprite::create("UI/UI_main_menu/btn_quest.png");
	_pQuestSelected->setColor(Color3B(128, 128, 128));
	auto _aQuestButton = MenuItemSprite::create(_pQuestNormal, _pQuestSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aQuestButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aQuestButton->getContentSize().width * 0.1f);
	_aQuestButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aQuestButton->getContentSize().height);
	_aQuestButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aQuestButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aQuestButton->getContentSize().width * _aQuestButton->getScaleX() + _fPadding;

	// button hero
	auto _pHeroNormal = Sprite::create("UI/UI_main_menu/btn_heroes.png");
	auto _pHeroSelected = Sprite::create("UI/UI_main_menu/btn_heroes.png");
	_pHeroSelected->setColor(Color3B(128, 128, 128));
	auto _aHeroButton = MenuItemSprite::create(_pHeroNormal, _pHeroSelected, CC_CALLBACK_0(MenuLayer::buttonHeroesHandle, this));
	_aHeroButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aHeroButton->getContentSize().width * 0.1f);
	_aHeroButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aHeroButton->getContentSize().height);
	_aHeroButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aHeroButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aHeroButton->getContentSize().width * _aHeroButton->getScaleX() + _fPadding;

	// button shop
	auto _pShopNormal = Sprite::create("UI/UI_main_menu/btn_shop.png");
	auto _pShopSelected = Sprite::create("UI/UI_main_menu/btn_shop.png");
	_pShopSelected->setColor(Color3B(128, 128, 128));
	auto _aShopButton = MenuItemSprite::create(_pShopNormal, _pShopSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aShopButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aShopButton->getContentSize().width * 0.1f);
	_aShopButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aShopButton->getContentSize().height);
	_aShopButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aShopButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aShopButton->getContentSize().width * _aShopButton->getScaleX() + _fPadding;

	// button leaderboard
	auto _pLeaderBoardNormal = Sprite::create("UI/UI_main_menu/btn_leaderboard.png");
	auto _pLeaderBoardSelected = Sprite::create("UI/UI_main_menu/btn_leaderboard.png");
	_pLeaderBoardSelected->setColor(Color3B(128, 128, 128));
	auto _aLeaderBoardButton = MenuItemSprite::create(_pLeaderBoardNormal, _pLeaderBoardSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aLeaderBoardButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aLeaderBoardButton->getContentSize().width * 0.1f);
	_aLeaderBoardButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aLeaderBoardButton->getContentSize().height);
	_aLeaderBoardButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aLeaderBoardButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aLeaderBoardButton->getContentSize().width * _aLeaderBoardButton->getScaleX() + _fPadding;

	// button free coin
	auto _pFreeCoinNormal = Sprite::create("UI/UI_main_menu/btn_free_coin.png");
	auto _pFreeCoinSelected = Sprite::create("UI/UI_main_menu/btn_free_coin.png");
	_pFreeCoinSelected->setColor(Color3B(128, 128, 128));
	auto _aFreeCoinButton = MenuItemSprite::create(_pFreeCoinNormal, _pFreeCoinSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aFreeCoinButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aFreeCoinButton->getContentSize().width * 0.1f);
	_aFreeCoinButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aFreeCoinButton->getContentSize().height);
	_aFreeCoinButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aFreeCoinButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aFreeCoinButton->getContentSize().width * _aFreeCoinButton->getScaleX() + _fPadding;

	// button setting
	auto _pSettingNormal = Sprite::create("UI/UI_main_menu/btn_setting.png");
	auto _pSettingSelected = Sprite::create("UI/UI_main_menu/btn_setting.png");
	_pSettingSelected->setColor(Color3B(128, 128, 128));
	auto _aSettingButton = MenuItemSprite::create(_pSettingNormal, _pSettingSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aSettingButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aSettingButton->getContentSize().width * 0.1f);
	_aSettingButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aSettingButton->getContentSize().height);
	_aSettingButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aSettingButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aSettingButton->getContentSize().width * _aSettingButton->getScaleX() + _fPadding;

	// button more game
	auto _pMoreGameNormal = Sprite::create("UI/UI_main_menu/btn_more_games.png");
	auto _pMoreGameSelected = Sprite::create("UI/UI_main_menu/btn_more_games.png");
	_pMoreGameSelected->setColor(Color3B(128, 128, 128));
	auto _aMoreGameButton = MenuItemSprite::create(_pMoreGameNormal, _pMoreGameSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aMoreGameButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aMoreGameButton->getContentSize().width * 0.1f);
	_aMoreGameButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aMoreGameButton->getContentSize().height);
	_aMoreGameButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aMoreGameButton->setPosition(_fXPositionCounter, 0.0f);

	_fXPositionCounter += _aMoreGameButton->getContentSize().width * _aMoreGameButton->getScaleX() + _fPadding
		+ m_pBottomMainMenu->getContentSize().width * 0.02f;

	// button start
	auto _pStartNormal = Sprite::create("UI/UI_main_menu/btn_start.png");
	auto _pStartSelected = Sprite::create("UI/UI_main_menu/btn_start.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_0(MenuLayer::buttonStartHandle, this));
	_aStartButton->setScaleX(m_pBottomMainMenu->getContentSize().width / _aStartButton->getContentSize().width * 0.26f);
	_aStartButton->setScaleY(m_pBottomMainMenu->getContentSize().height / _aStartButton->getContentSize().height * 0.8f);
	_aStartButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aStartButton->setPosition(_fXPositionCounter, 0.0f);

	m_fButtonStartPosition = _fXPositionCounter;

	auto _aBottomMenu = Menu::create(_aQuestButton, _aHeroButton, _aShopButton, _aLeaderBoardButton, _aFreeCoinButton,
		_aSettingButton, _aMoreGameButton, _aStartButton, NULL);
	_aBottomMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.27f)); // fill screen width, 27% screen height
	_aBottomMenu->setPosition(0.0f, 0.0f);
	m_pBottomMainMenu->addChild(_aBottomMenu, 2);
}

void MenuLayer::initItemBoard() {
	// right menu
	m_pItemBoard = Layer::create();
	m_pItemBoard->setContentSize(Size(m_szVisibleSize.width * 0.45f, m_szVisibleSize.height * 0.62f)); // 35% screen width, 62% screen height
	m_pItemBoard->setPosition(m_szVisibleSize.width + m_pItemBoard->getContentSize().width / 2,
		m_szVisibleSize.height * 0.25);
	m_pGameControl->addChild(m_pItemBoard, 1);

	// board item
	Sprite *_pItemBoard = Sprite::create("UI/UI_main_menu/board_item.png");
	_pItemBoard->setScaleX(m_pItemBoard->getContentSize().width / _pItemBoard->getContentSize().width);
	_pItemBoard->setScaleY(m_pItemBoard->getContentSize().height / _pItemBoard->getContentSize().height);
	_pItemBoard->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pItemBoard->setPosition(0.0f, 0.0f);
	m_pItemBoard->addChild(_pItemBoard, 0);

	int _nNumberItems = 5;
	int _nNumberItemsVisiable = 3;
	// scroll view
	ScrollView *_pItemScrollView = ScrollView::create();
	_pItemScrollView->setContentSize(Size(_pItemBoard->getContentSize().width * _pItemBoard->getScaleX() * 0.8f,
		_pItemBoard->getContentSize().height * _pItemBoard->getScaleY() * 0.7f));
	_pItemScrollView->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pItemScrollView->setPosition(Vec2(0.0f, _pItemBoard->getContentSize().height * _pItemBoard->getScaleY() * 0.1f));
	_pItemScrollView->setDirection(ScrollView::Direction::VERTICAL);
	_pItemScrollView->setBounceEnabled(true);
	_pItemScrollView->setTouchEnabled(true);
	_pItemScrollView->setScrollBarOpacity(0.0f);
	_pItemScrollView->setInnerContainerSize(Size(_pItemScrollView->getContentSize().width,
		_pItemScrollView->getContentSize().height * _nNumberItems / _nNumberItemsVisiable));
	m_pItemBoard->addChild(_pItemScrollView, 1);

	int _nItemIndexCounter = 0;
	float _fItemWidth = _pItemScrollView->getContentSize().width;
	float _fItemHeight = _pItemScrollView->getContentSize().height / _nNumberItemsVisiable;
	float _fScrollViewPadding = _fItemHeight / 20;

	// item 1 : health
	Sprite *_ItemSprite1 = Sprite::create("UI/UI_main_menu/item1_health.png");
	_ItemSprite1->setScale(_fItemHeight / _ItemSprite1->getContentSize().height);
	_ItemSprite1->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemSprite1->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_ItemSprite1, 1);

	Sprite *_CoinSprite1 = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_CoinSprite1->setScale(_fItemHeight / _CoinSprite1->getContentSize().height * 0.4f);
	_CoinSprite1->setAnchorPoint(Vec2(1.0f, 1.0f));
	_CoinSprite1->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_CoinSprite1, 1);

	auto _pBuyItem1Normal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyItem1Selected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
	_pBuyItem1Selected->setColor(Color3B(128, 128, 128));
	auto _aBuyItem1Button = MenuItemSprite::create(_pBuyItem1Normal, _pBuyItem1Selected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aBuyItem1Button->setScaleX(_fItemWidth / _aBuyItem1Button->getContentSize().width * 0.3f);
	_aBuyItem1Button->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aBuyItem1Button->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1)));

	Sprite *_ItemLine1 = Sprite::create("UI/UI_main_menu/line.png");
	_ItemLine1->setScale(_fItemWidth / _ItemLine1->getContentSize().width);
	_ItemLine1->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemLine1->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1) + _fScrollViewPadding));
	_pItemScrollView->addChild(_ItemLine1, 1);

	_nItemIndexCounter++;

	// item 2 : health
	Sprite *_ItemSprite2 = Sprite::create("UI/UI_main_menu/item2_bird.png");
	_ItemSprite2->setScale(_fItemHeight / _ItemSprite2->getContentSize().height);
	_ItemSprite2->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemSprite2->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_ItemSprite2, 1);

	Sprite *_CoinSprite2 = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_CoinSprite2->setScale(_fItemHeight / _CoinSprite2->getContentSize().height * 0.4f);
	_CoinSprite2->setAnchorPoint(Vec2(1.0f, 1.0f));
	_CoinSprite2->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_CoinSprite2, 1);

	auto _pBuyItem2Normal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyItem2Selected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
	_pBuyItem2Selected->setColor(Color3B(128, 128, 128));
	auto _aBuyItem2Button = MenuItemSprite::create(_pBuyItem2Normal, _pBuyItem2Selected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aBuyItem2Button->setScaleX(_fItemWidth / _aBuyItem2Button->getContentSize().width * 0.3f);
	_aBuyItem2Button->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aBuyItem2Button->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1)));

	Sprite *_ItemLine2 = Sprite::create("UI/UI_main_menu/line.png");
	_ItemLine2->setScale(_fItemWidth / _ItemLine2->getContentSize().width);
	_ItemLine2->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemLine2->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1) + _fScrollViewPadding));
	_pItemScrollView->addChild(_ItemLine2, 1);

	_nItemIndexCounter++;

	// item 3 : magnet
	Sprite *_ItemSprite3 = Sprite::create("UI/UI_main_menu/item3_magnet.png");
	_ItemSprite3->setScale(_fItemHeight / _ItemSprite3->getContentSize().height);
	_ItemSprite3->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemSprite3->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_ItemSprite3, 1);

	Sprite *_CoinSprite3 = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_CoinSprite3->setScale(_fItemHeight / _CoinSprite3->getContentSize().height * 0.4f);
	_CoinSprite3->setAnchorPoint(Vec2(1.0f, 1.0f));
	_CoinSprite3->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_CoinSprite3, 1);

	auto _pBuyItem3Normal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyItem3Selected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
	_pBuyItem3Selected->setColor(Color3B(128, 128, 128));
	auto _aBuyItem3Button = MenuItemSprite::create(_pBuyItem3Normal, _pBuyItem3Selected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aBuyItem3Button->setScaleX(_fItemWidth / _aBuyItem3Button->getContentSize().width * 0.3f);
	_aBuyItem3Button->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aBuyItem3Button->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1)));

	Sprite *_ItemLine3 = Sprite::create("UI/UI_main_menu/line.png");
	_ItemLine3->setScale(_fItemWidth / _ItemLine3->getContentSize().width);
	_ItemLine3->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemLine3->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1) + _fScrollViewPadding));
	_pItemScrollView->addChild(_ItemLine3, 1);

	_nItemIndexCounter++;

	// item 4 : double coin
	Sprite *_ItemSprite4 = Sprite::create("UI/UI_main_menu/item4_doublecoin.png");
	_ItemSprite4->setScale(_fItemHeight / _ItemSprite4->getContentSize().height);
	_ItemSprite4->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemSprite4->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_ItemSprite4, 1);

	Sprite *_CoinSprite4 = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_CoinSprite4->setScale(_fItemHeight / _CoinSprite4->getContentSize().height * 0.4f);
	_CoinSprite4->setAnchorPoint(Vec2(1.0f, 1.0f));
	_CoinSprite4->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_CoinSprite4, 1);

	auto _pBuyItem4Normal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyItem4Selected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
	_pBuyItem4Selected->setColor(Color3B(128, 128, 128));
	auto _aBuyItem4Button = MenuItemSprite::create(_pBuyItem4Normal, _pBuyItem4Selected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aBuyItem4Button->setScaleX(_fItemWidth / _aBuyItem4Button->getContentSize().width * 0.3f);
	_aBuyItem4Button->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aBuyItem4Button->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1)));

	Sprite *_ItemLine4 = Sprite::create("UI/UI_main_menu/line.png");
	_ItemLine4->setScale(_fItemWidth / _ItemLine4->getContentSize().width);
	_ItemLine4->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemLine4->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1) + _fScrollViewPadding));
	_pItemScrollView->addChild(_ItemLine4, 1);

	_nItemIndexCounter++;

	// item 5 : cooldown
	Sprite *_ItemSprite5 = Sprite::create("UI/UI_main_menu/item5_cooldown.png");
	_ItemSprite5->setScale(_fItemHeight / _ItemSprite5->getContentSize().height);
	_ItemSprite5->setAnchorPoint(Vec2(0.0f, 1.0f));
	_ItemSprite5->setPosition(Vec2(0.0f, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_ItemSprite5, 1);

	Sprite *_CoinSprite5 = Sprite::create("UI/UI_main_menu/icon_money_small.png");
	_CoinSprite5->setScale(_fItemHeight / _CoinSprite5->getContentSize().height * 0.4f);
	_CoinSprite5->setAnchorPoint(Vec2(1.0f, 1.0f));
	_CoinSprite5->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter)));
	_pItemScrollView->addChild(_CoinSprite5, 1);

	auto _pBuyItem5Normal = Sprite::create("UI/UI_main_menu/btn_buy_1.png");
	auto _pBuyItem5Selected = Sprite::create("UI/UI_main_menu/btn_buy_3.png");
	_pBuyItem5Selected->setColor(Color3B(128, 128, 128));
	auto _aBuyItem5Button = MenuItemSprite::create(_pBuyItem5Normal, _pBuyItem5Selected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aBuyItem5Button->setScaleX(_fItemWidth / _aBuyItem5Button->getContentSize().width * 0.3f);
	_aBuyItem5Button->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aBuyItem5Button->setPosition(Vec2(_fItemWidth, _fItemHeight * (_nNumberItems - _nItemIndexCounter - 1)));

	auto _aItemBoard = Menu::create(_aBuyItem1Button, _aBuyItem2Button, _aBuyItem3Button, _aBuyItem4Button, _aBuyItem5Button, NULL);
	_aItemBoard->setContentSize(_pItemScrollView->getContentSize());
	_aItemBoard->setPosition(0.0f, 0.0f);
	_pItemScrollView->addChild(_aItemBoard, 2);
}

void MenuLayer::initUpgrateBoard() {
	// left menu
	m_pUpgrateBoard = Layer::create();
	m_pUpgrateBoard->setContentSize(Size(m_szVisibleSize.width * 0.35f, m_szVisibleSize.height * 0.65f)); // 35% screen width, 65% screen height
	m_pUpgrateBoard->setPosition(-m_pUpgrateBoard->getContentSize().width / 2, m_szVisibleSize.height * 0.22f);
	m_pGameControl->addChild(m_pUpgrateBoard, 1);

	// board upgrate
	Sprite *_pBoardUpgrate = Sprite::create("UI/UI_main_menu/board_skill_upgrade.png");
	_pBoardUpgrate->setScaleX(m_pUpgrateBoard->getContentSize().width / _pBoardUpgrate->getContentSize().width);
	_pBoardUpgrate->setScaleY(m_pUpgrateBoard->getContentSize().height / _pBoardUpgrate->getContentSize().height);
	_pBoardUpgrate->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pBoardUpgrate->setPosition(0.0f, 0.0f);
	m_pUpgrateBoard->addChild(_pBoardUpgrate, 0);
}

void MenuLayer::initHeroInfoBoard() {
	// left menu
	m_pHeroInfoBoard = Layer::create();
	m_pHeroInfoBoard->setContentSize(Size(m_szVisibleSize.width * 0.3f, m_szVisibleSize.height * 0.6f)); // 30% screen width, 60% screen height
	m_pHeroInfoBoard->setPosition(m_szVisibleSize.width + m_pHeroInfoBoard->getContentSize().width / 2, m_szVisibleSize.height * 0.27f);
	m_pGameControl->addChild(m_pHeroInfoBoard, 1);

	// board upgrate
	Sprite *_pInfoBoard = Sprite::create("UI/UI_main_menu/board_information.png");
	_pInfoBoard->setScaleX(m_pHeroInfoBoard->getContentSize().width / _pInfoBoard->getContentSize().width);
	_pInfoBoard->setScaleY(m_pHeroInfoBoard->getContentSize().height / _pInfoBoard->getContentSize().height);
	_pInfoBoard->setAnchorPoint(Vec2(0.5f, 0.0f));
	_pInfoBoard->setPosition(0.0f, 0.0f);
	m_pHeroInfoBoard->addChild(_pInfoBoard, 0);
}

void MenuLayer::initBottomHeroMenu() {
	m_pBottomHeroMenu->removeAllChildrenWithCleanup(true);

	// hero face frame
	auto _pHeroFrame = Sprite::create("UI/UI_main_menu/bottom_bar.png");
	_pHeroFrame->setScaleX(m_pBottomHeroMenu->getContentSize().width / _pHeroFrame->getContentSize().width * 0.9f);
	_pHeroFrame->setScaleY(m_pBottomHeroMenu->getContentSize().height / _pHeroFrame->getContentSize().height);
	_pHeroFrame->setAnchorPoint(Vec2(0.0f, 0.5f));
	_pHeroFrame->setPosition(m_szVisibleSize.width / 50, 0.0f);
	m_pBottomHeroMenu->addChild(_pHeroFrame, 0);

	float _fHeroHeight = m_pBottomHeroMenu->getContentSize().height;
	float _fScrollViewPadding = 0.0f; // m_pBottomHeroMenu->getContentSize().height / 50.0f;
	float _fXPositionCounter = _fScrollViewPadding;

	// hero 1 : duong qua
	JSHERO->readFile(0);
	Sprite *_pHeroSprite1;
	if (JSHERO->getIsLocked()) {
		_pHeroSprite1 = Sprite::create("UI/UI_main_menu/icon_hero_dq_3.png");
		if (m_nIndexHeroSelected == 0) {
			_pHeroSprite1 = Sprite::create("UI/UI_main_menu/icon_hero_dq_4.png");
		}
	}
	else if (m_nIndexHeroSelected == 0) {
		_pHeroSprite1 = Sprite::create("UI/UI_main_menu/icon_hero_dq_1.png");
	}
	else {
		_pHeroSprite1 = Sprite::create("UI/UI_main_menu/icon_hero_dq_2.png");
	}
	m_pHeroButton1 = MenuItemSprite::create(_pHeroSprite1, _pHeroSprite1, CC_CALLBACK_0(MenuLayer::pickHeroFirst, this));
	m_pHeroButton1->setScale(_fHeroHeight / _pHeroSprite1->getContentSize().height);
	m_pHeroButton1->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pHeroButton1->setPosition(Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pHeroSprite1->getContentSize().width * _pHeroSprite1->getScaleX() + _fScrollViewPadding;

	// hero 2 : co long
	JSHERO->readFile(1);
	Sprite *_pHeroSprite2;
	if (JSHERO->getIsLocked()) {
		_pHeroSprite2 = Sprite::create("UI/UI_main_menu/icon_hero_cc_3.png");
		if (m_nIndexHeroSelected == 1) {
			_pHeroSprite2 = Sprite::create("UI/UI_main_menu/icon_hero_cc_4.png");
		}
	}
	else if (m_nIndexHeroSelected == 1) {
		_pHeroSprite2 = Sprite::create("UI/UI_main_menu/icon_hero_cc_1.png");
	}
	else {
		_pHeroSprite2 = Sprite::create("UI/UI_main_menu/icon_hero_cc_2.png");
	}
	m_pHeroButton2 = MenuItemSprite::create(_pHeroSprite2, _pHeroSprite2, CC_CALLBACK_0(MenuLayer::pickHeroSecond, this));
	m_pHeroButton2->setScale(_fHeroHeight / _pHeroSprite2->getContentSize().height);
	m_pHeroButton2->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pHeroButton2->setPosition(Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pHeroSprite2->getContentSize().width * _pHeroSprite2->getScaleX() + _fScrollViewPadding;

	// hero 3 : hoang dung
	JSHERO->readFile(2);
	Sprite *_pHeroSprite3;
	if (JSHERO->getIsLocked()) {
		_pHeroSprite3 = Sprite::create("UI/UI_main_menu/icon_hero_hd_3.png");
		if (m_nIndexHeroSelected == 2) {
			_pHeroSprite3 = Sprite::create("UI/UI_main_menu/icon_hero_hd_4.png");
		}
	}
	else if (m_nIndexHeroSelected == 2) {
		_pHeroSprite3 = Sprite::create("UI/UI_main_menu/icon_hero_hd_1.png");
	}
	else {
		_pHeroSprite3 = Sprite::create("UI/UI_main_menu/icon_hero_hd_2.png");
	}
	m_pHeroButton3 = MenuItemSprite::create(_pHeroSprite3, _pHeroSprite3, CC_CALLBACK_0(MenuLayer::pickHeroThird, this));
	m_pHeroButton3->setScale(_fHeroHeight / _pHeroSprite3->getContentSize().height);
	m_pHeroButton3->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pHeroButton3->setPosition(Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pHeroSprite3->getContentSize().width * _pHeroSprite3->getScaleX() + _fScrollViewPadding;

	// hero 4 : quach tinh
	JSHERO->readFile(3);
	Sprite *_pHeroSprite4;
	if (JSHERO->getIsLocked()) {
		_pHeroSprite4 = Sprite::create("UI/UI_main_menu/icon_hero_qt_3.png");
		if (m_nIndexHeroSelected == 3) {
			_pHeroSprite4 = Sprite::create("UI/UI_main_menu/icon_hero_qt_4.png");
		}
	}
	else if (m_nIndexHeroSelected == 3) {
		_pHeroSprite4 = Sprite::create("UI/UI_main_menu/icon_hero_qt_1.png");
	}
	else {
		_pHeroSprite4 = Sprite::create("UI/UI_main_menu/icon_hero_qt_2.png");
	}
	m_pHeroButton4 = MenuItemSprite::create(_pHeroSprite4, _pHeroSprite4, CC_CALLBACK_0(MenuLayer::pickHeroFourth, this));
	m_pHeroButton4->setScale(_fHeroHeight / _pHeroSprite4->getContentSize().height);
	m_pHeroButton4->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pHeroButton4->setPosition(Vec2(_fXPositionCounter, 0.0f));

	_fXPositionCounter += _pHeroSprite4->getContentSize().width * _pHeroSprite4->getScaleX() + _fScrollViewPadding;

	// hero 5 : hoang duoc su
	JSHERO->readFile(4);
	Sprite *_pHeroSprite5;
	if (JSHERO->getIsLocked()) {
		_pHeroSprite5 = Sprite::create("UI/UI_main_menu/icon_hero_hds_3.png");
		if (m_nIndexHeroSelected == 4) {
			_pHeroSprite5 = Sprite::create("UI/UI_main_menu/icon_hero_hds_4.png");
		}
	}
	else if (m_nIndexHeroSelected == 4) {
		_pHeroSprite5 = Sprite::create("UI/UI_main_menu/icon_hero_hds_1.png");
	}
	else {
		_pHeroSprite5 = Sprite::create("UI/UI_main_menu/icon_hero_hds_2.png");
	}
	m_pHeroButton5 = MenuItemSprite::create(_pHeroSprite5, _pHeroSprite5, CC_CALLBACK_0(MenuLayer::pickHeroFifth, this));
	m_pHeroButton5->setScale(_fHeroHeight / _pHeroSprite5->getContentSize().height);
	m_pHeroButton5->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_pHeroButton5->setPosition(Vec2(_fXPositionCounter, 0.0f));

	// button try
	auto _pTryHeroNormal = Sprite::create("UI/UI_main_menu/btn_try.png");
	auto _pTryHeroSelected = Sprite::create("UI/UI_main_menu/btn_try.png");
	_pTryHeroSelected->setColor(Color3B(128, 128, 128));
	auto _aTryHeroButton = MenuItemSprite::create(_pTryHeroNormal, _pTryHeroSelected, CC_CALLBACK_0(MenuLayer::backFunction, this));
	_aTryHeroButton->setScaleX(_pHeroFrame->getContentSize().width / _aTryHeroButton->getContentSize().width * 0.1f);
	_aTryHeroButton->setScaleY(_pHeroFrame->getContentSize().height / _aTryHeroButton->getContentSize().height * 0.8f);
	_aTryHeroButton->setAnchorPoint(Vec2(1.0f, 0.5f));
	_aTryHeroButton->setPosition(m_fButtonStartPosition - m_szVisibleSize.width / 20.0f, 0.0f);

	// button unlock
	auto _pUnlockNormal = Sprite::create("UI/UI_main_menu/btn_unlock.png");
	auto _pUnlockSelected = Sprite::create("UI/UI_main_menu/btn_unlock.png");
	_pUnlockSelected->setColor(Color3B(128, 128, 128));
	auto _aUnlockButton = MenuItemSprite::create(_pUnlockNormal, _pUnlockSelected, CC_CALLBACK_0(MenuLayer::unlockButtonHandle, this));
	_aUnlockButton->setScaleX(m_pBottomHeroMenu->getContentSize().width / _aUnlockButton->getContentSize().width * 0.26f);
	_aUnlockButton->setScaleY(m_pBottomHeroMenu->getContentSize().height / _aUnlockButton->getContentSize().height);
	_aUnlockButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aUnlockButton->setPosition(m_fButtonStartPosition, 0.0f);

	JSHERO->readFile(m_nIndexHeroSelected);
	if (!JSHERO->getIsLocked()) {
		_aUnlockButton->setNormalImage(Sprite::create("UI/UI_main_menu/btn_select.png"));
		auto _pTemp = Sprite::create("UI/UI_main_menu/btn_select.png");
		_pTemp->setColor(Color3B(128, 128, 128));
		_aUnlockButton->setSelectedImage(_pTemp);
		_aUnlockButton->setCallback(CC_CALLBACK_0(MenuLayer::selectButtonHandle, this));
	}
	else {
		// sprite coin to unlock
		Sprite *_pCoinSprite = Sprite::create("UI/UI_main_menu/icon_money_big.png");
		_pCoinSprite->setScale(_aUnlockButton->getContentSize().height * _aUnlockButton->getScaleY() / _pCoinSprite->getContentSize().height);
		_pCoinSprite->setAnchorPoint(Vec2(0.0f, 0.0f));
		_pCoinSprite->setPosition(Vec2(_aUnlockButton->getContentSize().width * _aUnlockButton->getScaleX() * 0.8f, 0.0f));
		_aUnlockButton->addChild(_pCoinSprite, 1);
	}

	auto _aBottomMenu = Menu::create(m_pHeroButton1, m_pHeroButton2, m_pHeroButton3, m_pHeroButton4, m_pHeroButton5, _aTryHeroButton, _aUnlockButton, NULL);
	_aBottomMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.25f)); // fill screen width, 27% screen height
	_aBottomMenu->setPosition(0.0f, 0.0f);
	m_pBottomHeroMenu->addChild(_aBottomMenu, 2);
}

void MenuLayer::showMainMenu() {
	moveLayerViaDirection(m_pTopMainMenu, 2);
	moveLayerViaDirection(m_pBottomMainMenu, 8);
	moveLayerViaDirection(m_pItemBoard, 4);
}

void MenuLayer::buttonStartHandle() {
	auto _aScene = GameScene::createScene(3, 1, m_nIndexHeroSelected);
	REF->pointToCurrentHero(m_nIndexHeroSelected);
//    auto _aScene = SelectStageScene::createScene();
    Director::getInstance()->replaceScene(_aScene);
//	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene)); // delay 1s before replace scene
}

void MenuLayer::buttonBackHanle() {
	if (m_nIndexHeroSelected != m_nIndexHeroPicked) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
		initSceneLayer();
	}
	if (m_bIsHeroLayout == true) {
		m_bIsHeroLayout = false;
		moveLayerViaDirection(m_pUpgrateBoard, 4);
		moveLayerViaDirection(m_pHeroInfoBoard, 6);
		moveLayerViaDirection(m_pBottomHeroMenu, 2);
		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(0.0f, m_szVisibleSize.height * 0.2f)));
		}), nullptr));
		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pBottomMainMenu, 8);
			moveLayerViaDirection(m_pItemBoard, 4);
		}), nullptr));
	}
	else {
		// TODO : show dialog confirm quit game
	}
}

void MenuLayer::backFunction() {

}

void MenuLayer::buttonHeroesHandle() {
	if (m_nIndexHeroSelected != m_nIndexHeroPicked) {
		m_nIndexHeroSelected = m_nIndexHeroPicked;
	}
	initBottomHeroMenu();
	if (m_bIsHeroLayout == false) {
		m_bIsHeroLayout = true;
		moveLayerViaDirection(m_pBottomMainMenu, 2);
		moveLayerViaDirection(m_pItemBoard, 6);

		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&]() {
			m_pGameScene->runAction(MoveTo::create(0.3f, Vec2(m_szVisibleSize.width / 4, m_szVisibleSize.height * 0.2f)));
		}), nullptr));

		runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([&]() {
			moveLayerViaDirection(m_pUpgrateBoard, 6);
			moveLayerViaDirection(m_pHeroInfoBoard, 4);
			moveLayerViaDirection(m_pBottomHeroMenu, 8);
		}), nullptr));
	}
}

void MenuLayer::pickHeroFirst() {
	m_nIndexHeroSelected = 0;
	initSceneLayer();
	initBottomHeroMenu();
}

void MenuLayer::pickHeroSecond() {
	m_nIndexHeroSelected = 1;
	initSceneLayer();
	initBottomHeroMenu();
}

void MenuLayer::pickHeroThird() {
	m_nIndexHeroSelected = 2;
	initSceneLayer();
	initBottomHeroMenu();
}

void MenuLayer::pickHeroFourth() {
	m_nIndexHeroSelected = 3;
	initSceneLayer();
	initBottomHeroMenu();
}

void MenuLayer::pickHeroFifth() {
	m_nIndexHeroSelected = 4;
	initSceneLayer();
	initBottomHeroMenu();
}

void MenuLayer::tryButtonHandle() {

}

void MenuLayer::unlockButtonHandle() {
	auto _aAnimSprite = Sprite::create("UI/UI_main_menu/effect_unlock_2.png");
	auto _aCache = AnimationCache::getInstance();
	Vector<SpriteFrame*> _arFrames(2);
	_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/effect_unlock_2.png",
		Rect(0, 0, _aAnimSprite->getContentSize().width, _aAnimSprite->getContentSize().height)));
	_arFrames.pushBack(SpriteFrame::create("UI/UI_main_menu/effect_unlock_1.png",
		Rect(0, 0, _aAnimSprite->getContentSize().width, _aAnimSprite->getContentSize().height)));
	_aCache->addAnimation(Animation::createWithSpriteFrames(_arFrames, 0.1f), "bum");
	_aAnimSprite->runAction(Sequence::create(Animate::create(_aCache->getAnimation("bum")), NULL));

	switch (m_nIndexHeroSelected) {
	case 0:
		_aAnimSprite->setPosition(m_pHeroButton1->getContentSize().width / 2, m_pHeroButton1->getContentSize().height / 2);
		m_pHeroButton1->addChild(_aAnimSprite, 1);
		break;
	case 1:
		_aAnimSprite->setPosition(m_pHeroButton2->getContentSize().width / 2, m_pHeroButton2->getContentSize().height / 2);
		m_pHeroButton2->addChild(_aAnimSprite, 1);
		break;
	case 2:
		_aAnimSprite->setPosition(m_pHeroButton3->getContentSize().width / 2, m_pHeroButton3->getContentSize().height / 2);
		m_pHeroButton3->addChild(_aAnimSprite, 1);
		break;
	case 3:
		_aAnimSprite->setPosition(m_pHeroButton4->getContentSize().width / 2, m_pHeroButton4->getContentSize().height / 2);
		m_pHeroButton4->addChild(_aAnimSprite, 1);
		break;
	case 4:
		_aAnimSprite->setPosition(m_pHeroButton5->getContentSize().width / 2, m_pHeroButton5->getContentSize().height / 2);
		m_pHeroButton5->addChild(_aAnimSprite, 1);
		break;
	default:
		break;
	}
	//JSHERO->writerBool(m_nIndexHeroSelected, "isLocked", false);
	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() { initBottomHeroMenu(); }), nullptr));
}

void MenuLayer::selectButtonHandle() {
	m_nIndexHeroPicked = m_nIndexHeroSelected;
	REF->pointToCurrentHero(m_nIndexHeroPicked);
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
