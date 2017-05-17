#include "SelectStageScene.h"
#include "LoadingLayer.h"
#include "MenuScene.h"
#include "manager/RefManager.h"


Scene * SelectStageLayer::createScene(int charId)
{
	auto scene = Scene::create();
	auto layer = SelectStageLayer::create(charId);

	scene->addChild(layer);
	return scene;
}

bool SelectStageLayer::init(int charId)
{
	AudioManager::playMusic(MUSIC_MENU);
	auto originXY = Director::getInstance()->getVisibleOrigin();
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto tmxMap = TMXTiledMap::create("UI/Select_Stage/config_map.tmx");
	tmxMap->setPosition(Vec2::ZERO);

	auto scaleY = screenSize.height / tmxMap->getContentSize().height;
	tmxMap->setScale(scaleY);


	ui::ScrollView* scrollView = ui::ScrollView::create();
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

	auto character_point = Sprite::create(JSHERO->getSelectCharacterPoint());
	character_point->setAnchorPoint(Vec2(0.5f, 0));
	character_point->setScale(screenSize.height / 8.5f / character_point->getContentSize().width);
	auto moveUp = MoveBy::create(0.1f, Vec2(0, character_point->getBoundingBox().size.height * 0.02f));
	auto seq = Sequence::createWithTwoActions(moveUp, moveUp->reverse());
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
				scrollView->scrollToPercentHorizontal(origin.x / tmxMap->getBoundingBox().size.width * 100.0f, 1.0f, true);
		}

		MenuItemSprite* mapBtn;
		int stage = mObject["stage"].asInt();
		int mapId = mObject["mapId"].asInt();
		int haveBoss = mObject["haveBoss"].asInt();


		Sprite* un_locked = Sprite::create("UI/Select_Stage/level_on.png");
		Sprite* un_locked_press = Sprite::create("UI/Select_Stage/level_on.png");
		un_locked_press->setColor(Color3B(128, 128, 128));

		Sprite* locked = Sprite::create("UI/Select_Stage/level_off.png");

		if (stage < currentStageUnlocked) {
			mapBtn = MenuItemSprite::create(un_locked, un_locked_press,
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, id, stage, mapId, haveBoss, charId));
		}
		else if (stage == currentStageUnlocked) {
			if (mapId <= currentMapUnlocked) {
				mapBtn = MenuItemSprite::create(un_locked, un_locked_press,
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, id, stage, mapId, haveBoss, charId));
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

void SelectStageLayer::gotoPlay(int id, int stage, int map, int haveBoss, int charId)
{
	AudioManager::playSound(SOUND_BTCLICK);
	if (m_nLifeNumber > 0) {
		REF->setLastMapId(id);
		m_nLifeNumber--;
		REF->setDownLife(1);
		REF->resetAnchorTime();
		AudioManager::stopMusic();
		auto _aScene = LoadingLayer::createScene(stage, map, haveBoss, charId);
		Director::getInstance()->replaceScene(_aScene);
	} else {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(1), TOAST_LONG);
		_pToast->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 4));
		addChild(_pToast, 10);
	}
}

void SelectStageLayer::doNothing()
{
}

void SelectStageLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		/*auto _aScene = MenuLayer::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene));*/
	}
}

void SelectStageLayer::initData() {
	m_nLifeNumber = REF->getNumberOfLife();
	m_nTimeAnchor = REF->getAnchorTime();
}
