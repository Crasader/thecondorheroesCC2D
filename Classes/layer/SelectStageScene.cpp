#include "SelectStageScene.h"
#include "LoadingLayer.h"
#include "MenuScene.h"
#include "manager/RefManager.h"


Scene * SelectStageLayer::createScene(int charId)
{
	auto scene = Scene::create();
	auto layer = SelectStageLayer::create(charId);
	layer->setName("selectLayer");
	scene->addChild(layer);
	return scene;
}

bool SelectStageLayer::init(int charId)
{
	AudioManager::playMusic(MUSIC_MENU);
	auto originXY = Director::getInstance()->getVisibleOrigin();
	auto screenSize = Director::getInstance()->getVisibleSize();
	tmxMap = TMXTiledMap::create("UI/Select_Stage/config_map.tmx");
	tmxMap->setPosition(Vec2::ZERO);

	auto scaleY = screenSize.height / tmxMap->getContentSize().height;
	tmxMap->setScale(scaleY);


	scrollView = ui::ScrollView::create();
	scrollView->setContentSize(Size(screenSize.width, screenSize.height));
	scrollView->setInnerContainerSize(Size(tmxMap->getBoundingBox().size.width, screenSize.height));
	scrollView->setPosition(originXY);
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
	scrollView->setTouchEnabled(true);
	scrollView->setBounceEnabled(false);
	scrollView->setScrollBarOpacity(0);
	addChild(scrollView);

	scrollView->addChild(tmxMap);

	auto grMapBtn = tmxMap->getObjectGroup("map");

	int currentStageUnlocked = REF->getCurrentStageUnlocked();
	int currentMapUnlocked = REF->getCurrentMapUnLocked();
	int lastMapId = REF->getLastMapIdPlay();
	int nextMapId = lastMapId < 12 ? lastMapId + 1 : 12;

	character_point = Sprite::create(JSHERO->getSelectCharacterPoint());
	character_point->setAnchorPoint(Vec2(0.5f, 0));
	character_point->setScale(screenSize.height / 8.5f / character_point->getContentSize().width);
	auto moveUp = MoveBy::create(0.3f, Vec2(0, character_point->getBoundingBox().size.height * 0.04f));
	auto scaleUp = ScaleBy::create(0.3f, 1.04f);
	auto seq = Sequence::createWithTwoActions(Spawn::create(EaseInOut::create(moveUp, 2), scaleUp, nullptr), 
											Spawn::create(EaseInOut::create(moveUp->reverse(), 2), scaleUp->reverse(), nullptr));
	character_point->runAction(RepeatForever::create(seq));
	scrollView->addChild(character_point, 2);

	Menu* menu = Menu::create();
	for (auto child : grMapBtn->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * scaleY, mObject["y"].asFloat() * scaleY);

		// get mapId on tmx map
		int id = mObject["id"].asInt();
		
		if (id == lastMapId) {
			character_point->setPosition(origin.x, origin.y + character_point->getBoundingBox().size.height * 0.25f);
			float percent = origin.x / tmxMap->getBoundingBox().size.width * 100.0f;
			if(percent > 15.0f)
				scrollView->scrollToPercentHorizontal(percent, 1.0f, true);
		}

		
		if (id == nextMapId) {
			nextMapPos = Point(origin.x, origin.y + character_point->getBoundingBox().size.height * 0.25f);
		}

		MenuItemSprite* mapBtn;
		int stage = mObject["stage"].asInt();
		int mapId = mObject["mapId"].asInt();


		Sprite* un_locked = Sprite::create("UI/Select_Stage/level_on.png");
		Sprite* un_locked_press = Sprite::create("UI/Select_Stage/level_on.png");
		un_locked_press->setColor(Color3B(128, 128, 128));

		Sprite* locked = Sprite::create("UI/Select_Stage/level_off.png");

		if (stage < currentStageUnlocked) {
			mapBtn = MenuItemSprite::create(un_locked, un_locked_press,
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, id, stage, mapId, charId));
		}
		else if (stage == currentStageUnlocked) {
			if (mapId <= currentMapUnlocked) {
				mapBtn = MenuItemSprite::create(un_locked, un_locked_press,
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, id, stage, mapId, charId));
			}
			else {
				mapBtn = MenuItemSprite::create(locked, locked,
					CC_CALLBACK_0(SelectStageLayer::doNothing, this));
			}

		}
		else {
			mapBtn = MenuItemSprite::create(locked, locked,
				CC_CALLBACK_0(SelectStageLayer::doNothing, this));
		}

		auto lblID = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("%i", id), TextHAlignment::CENTER);
		lblID->setBMFontSize(mapBtn->getContentSize().height / 1.8f);
		lblID->setAnchorPoint(Vec2(0.5f, 0.07f));
		lblID->setPosition(mapBtn->getContentSize() / 2);

		if (!mObject["boss_order"].isNull()) {
			int boss_order = mObject["boss_order"].asInt();
			auto b = bossSprite(boss_order);
			b->setPosition(mapBtn->getContentSize().width, 0);
			mapBtn->addChild(b);
		}

		mapBtn->addChild(lblID);
		mapBtn->setPosition(origin);
		mapBtn->setScale(screenSize.height / 9 / mapBtn->getContentSize().width);

		menu->addChild(mapBtn);
	}
	
	menu->setPosition(Vec2::ZERO);
	scrollView->addChild(menu);


	initData();

	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = CC_CALLBACK_2(SelectStageLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);

	m_pTimeCounter->schedule([&](float dt) {
		time_t _nCurrentTime = time(0);
		if (m_nLifeNumber < 5) {
			int _nDeltaTime = _nCurrentTime - m_nTimeAnchor;
			if (_nDeltaTime >= 300) {
				m_nTimeAnchor = _nCurrentTime;
				m_nLifeNumber += 1;
				initTopMainMenu();
				REF->setUpLife(1);
				REF->resetAnchorTime();
				return;
			}
			else {
				if(!m_pTimeCounter->isVisible())
					m_pTimeCounter->setVisible(true);
				int _nMinute = (300 - _nDeltaTime) / 60;
				int _nSecond = (300 - _nDeltaTime) % 60;
				m_pTimeCounter->setString(StringUtils::format(_nSecond < 10 ? "%i:0%i" : "%i:%i", _nMinute, _nSecond));
			}
		}
		else if (m_pTimeCounter->isVisible())
		{
			m_pTimeCounter->setVisible(false);
		}
	}, 0.1f, "key");
	return true;
}

SelectStageLayer * SelectStageLayer::create(int charId)
{
	SelectStageLayer *pRet = new(std::nothrow) SelectStageLayer();
	if (pRet && pRet->init(charId))
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

void SelectStageLayer::moveAva()
{
	int lastMapId = REF->getLastMapIdPlay();
	if (lastMapId < 12) {
		auto actionMove = MoveTo::create(1.2f, nextMapPos);
		auto scroll = CallFunc::create([&]() {
			float percent = nextMapPos.x / tmxMap->getBoundingBox().size.width * 100.0f;
			if (percent > 15.0f)
				scrollView->scrollToPercentHorizontal(percent, 4.3f, true);
		});
		character_point->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), Spawn::createWithTwoActions(actionMove, scroll)));
		REF->setLastMapId(lastMapId + 1);
	}
}

void SelectStageLayer::gotoPlay(int id, int stage, int map, int charId)
{
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nLifeNumber > 0) {
		REF->setLastMapId(id);
		m_nLifeNumber--;
		REF->setDownLife(1);
		REF->resetAnchorTime();
		initTopMainMenu();
		AudioManager::stopMusic();
		auto _aScene = LoadingLayer::createScene(stage, map, charId);
		Director::getInstance()->replaceScene(_aScene);
	} else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(1), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		addChild(_pToast, 10);
	}
}


Sprite* SelectStageLayer::bossSprite(int order)
{
	Sprite* boss;
	switch (order)
	{
	case 1:
		boss = Sprite::create("UI/Select_Stage/boss_TTK_off.png");
		break;

	case 2:
		boss = Sprite::create("UI/Select_Stage/boss_LMS_off.png");
		break;

	case 3:
		boss = Sprite::create("UI/Select_Stage/boss_KLPV_off.png");
		break;
	default:
		break;
	}

	boss->setAnchorPoint(Vec2::ZERO);
	boss->setScale(boss->getScale() * 1.5f);
	auto scaleUp = ScaleBy::create(0.8f, 1.04f);
	auto seq = Sequence::createWithTwoActions(EaseInOut::create(scaleUp, 2), EaseInOut::create(scaleUp->reverse(), 2));
	boss->runAction(RepeatForever::create(seq));

	return boss;
}

void SelectStageLayer::goBack()
{
	AudioManager::playSound(SOUND_BTCLICK);
	this->removeAllChildrenWithCleanup(true);
	auto _aScene = MenuLayer::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene));
}

void SelectStageLayer::doNothing()
{
}

void SelectStageLayer::buttonAddLifeHandle()
{
	AudioManager::playSound(SOUND_BTCLICK);
	m_nLifeNumber += 1;
	REF->setUpLife(1);
	initTopMainMenu();
}

void SelectStageLayer::buttonAddGoldHandle()
{
	AudioManager::playSound(SOUND_BTCLICK);
	REF->setUpGoldExplored(10000);
	initTopMainMenu();
}

void SelectStageLayer::buttonAddDiamondHandle()
{
	AudioManager::playSound(SOUND_BTCLICK);
	REF->setUpDiamondBuy(100);
	initTopMainMenu();
}

void SelectStageLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		auto _aScene = MenuLayer::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene));
	}
}


void SelectStageLayer::initData()
{
	m_nLifeNumber = REF->getNumberOfLife();
	m_nTimeAnchor = REF->getAnchorTime();
	m_pTopMainMenu = Layer::create();
	m_pTopMainMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	m_pTopMainMenu->setPosition(Director::getInstance()->getVisibleOrigin().x + m_szVisibleSize.width / 2, 
								m_szVisibleSize.height + m_pTopMainMenu->getContentSize().height / 2);
	addChild(m_pTopMainMenu, 1);
	initTopMainMenu();

	m_pTopMainMenu->runAction(MoveBy::create(0.3f, Vec2(0.0f, -m_pTopMainMenu->getContentSize().height)));
}


void SelectStageLayer::initTopMainMenu() {
	m_pTopMainMenu->removeAllChildrenWithCleanup(true);

	float _fPadding = m_pTopMainMenu->getContentSize().width / 50;
	float _fXPositionCounter = -m_pTopMainMenu->getContentSize().width / 2 + _fPadding / 2;

	// button back
	auto _pBackNormal = Sprite::create("UI/UI_main_menu/btn_back.png");
	auto _pBackSelected = Sprite::create("UI/UI_main_menu/btn_back.png");
	_pBackSelected->setColor(Color3B(128, 128, 128));
	auto _aBackButton = MenuItemSprite::create(_pBackNormal, _pBackSelected, CC_CALLBACK_0(SelectStageLayer::goBack, this));
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
	auto _aAddLifeButton = MenuItemSprite::create(_pAddLifeNormal, _pAddLifeSelected, CC_CALLBACK_0(SelectStageLayer::buttonAddLifeHandle, this));
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

	
	Label *_pLabelNumberGold = Label::createWithBMFont("fonts/font_coin-export.fnt", StringUtils::format("%i", REF->getGoldExplored()));
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
	auto _aAddMoneyButton = MenuItemSprite::create(_pAddMoneyNormal, _pAddMoneySelected, CC_CALLBACK_0(SelectStageLayer::buttonAddGoldHandle, this));
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

	
	Label *_pLabelNumberDiamond = Label::createWithBMFont("fonts/font_diamond-export.fnt", StringUtils::format("%i", REF->getDiamondBuy()));
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
	auto _aAddDiamondButton = MenuItemSprite::create(_pAddDiamondNormal, _pAddDiamondSelected, CC_CALLBACK_0(SelectStageLayer::buttonAddDiamondHandle, this));
	_aAddDiamondButton->setScale(_pDiamondFrame->getContentSize().width * _pLifeFrame->getScaleX() / _aAddDiamondButton->getContentSize().width);
	_aAddDiamondButton->setAnchorPoint(Vec2(0.0f, 0.5f));
	_aAddDiamondButton->setPosition(_fXPositionCounter, 0.0f);

	auto _aTopMenu = Menu::create(_aBackButton, _aAddLifeButton, _aAddMoneyButton, _aAddDiamondButton, nullptr);
	_aTopMenu->setContentSize(Size(m_szVisibleSize.width, m_szVisibleSize.height * 0.1f)); // fill screen width, 10% screen height
	_aTopMenu->setPosition(0.0f, 0.0f);
	m_pTopMainMenu->addChild(_aTopMenu, 2);
}
