#include "SelectStageScene.h"
#include "LoadingLayer.h"
#include "MenuScene.h"
#include "manager/RefManager.h"
#include "manager/AudioManager.h"
#include "manager/JSonHeroManager.h"
#include "thirdsdkhelper/AdmobHelper.h"


bool SelectStageLayer::init(int charId)
{
	AudioManager::playMusic(MUSIC_MENU);

	auto originXY = Director::getInstance()->getVisibleOrigin();
	auto screenSize = Director::getInstance()->getVisibleSize();
	tmxMap = TMXTiledMap::create("UI/Select_Stage/config_map.tmx");
	tmxMap->setPosition(Vec2::ZERO);

	auto scaleY = screenSize.height / tmxMap->getContentSize().height;
	tmxMap->setScale(scaleY);

	createCloud();

	scrollView = ui::ScrollView::create();
	scrollView->setContentSize(Size(screenSize.width, screenSize.height));
	scrollView->setInnerContainerSize(Size(tmxMap->getBoundingBox().size.width, screenSize.height));
	scrollView->setPosition(Vec2::ZERO);
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
	character_point->setScale(screenSize.height / 8.0f / character_point->getContentSize().width);
	auto moveUp = MoveBy::create(0.3f, Vec2(0, character_point->getBoundingBox().size.height * 0.04f));
	auto scaleUp = ScaleBy::create(0.3f, 1.04f);
	auto seq = Sequence::createWithTwoActions(Spawn::create(EaseInOut::create(moveUp, 2), scaleUp, nullptr), 
											Spawn::create(EaseInOut::create(moveUp->reverse(), 2), scaleUp->reverse(), nullptr));
	character_point->runAction(RepeatForever::create(seq));
	scrollView->addChild(character_point, 2);

	menu = Menu::create();
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

void SelectStageLayer::moveAva()
{
	int lastMapId = REF->getLastMapIdPlay();
	if (lastMapId < 12) {
		if (REF->getIsGetNewMap()) {
			auto screenSize = Director::getInstance()->getVisibleSize();
			auto effectUpLvMap = Sprite::create("UI/Select_Stage/effect_lv.png");
			effectUpLvMap->setScale(screenSize.height / 5.0f / effectUpLvMap->getContentSize().width);
			auto pos = Point(nextMapPos.x, nextMapPos.y - character_point->getBoundingBox().size.height * 0.25f);
			effectUpLvMap->setPosition(pos);
			scrollView->addChild(effectUpLvMap);

			auto scale = ScaleBy::create(0.5f, 1.2f);
			effectUpLvMap->runAction(RepeatForever::create(Sequence::createWithTwoActions(scale, scale->reverse())));
			REF->setReachNewMap(false);
		}
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
	auto m_nLifeNumber = REF->getNumberOfLife();
	if (m_nLifeNumber > 0) {
		REF->setLastMapId(id);
		m_nLifeNumber--;
		REF->setDownLife(1);
		REF->setAnchorTime(time(0));
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

	/*Vector <SpriteFrame*> aniframes;
	for (int i = 0; i < 1; ++i) {
		string frameName = StringUtils::format("boss%d_0%d.png", order, i);
		aniframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}

	auto animation = Animation::createWithSpriteFrames(aniframes, 0.08f);
	auto animate = Animate::create(animation);*/

	boss->setAnchorPoint(Vec2::ZERO);
	boss->setScale(boss->getScale() * 1.5f);
	auto scaleUp = ScaleBy::create(0.8f, 1.04f);
	auto seq = Sequence::createWithTwoActions(EaseInOut::create(scaleUp, 2), EaseInOut::create(scaleUp->reverse(), 2));
	boss->runAction(RepeatForever::create(seq));

	return boss;
}

void SelectStageLayer::doNothing()
{
}

//int SelectStageLayer::convertId()
//{
//	int currentStageUnlocked = REF->getCurrentStageUnlocked();
//	int currentMapUnlocked = REF->getCurrentMapUnLocked();
//	int convertValue = 1;
//	switch (currentStageUnlocked)
//	{
//	case 1: case 2: case 3:
//		convertValue = currentMapUnlocked + (currentStageUnlocked - 1 ) * 3;
//		break;
//
//	case 4:
//		convertValue = currentStageUnlocked + 8;
//		break;
//	default:
//		break;
//	}
//	return convertValue;
//}

void SelectStageLayer::createCloud() {
	auto screenSize = Director::getInstance()->getVisibleSize();
	for (int i = 0; i < 5; i++) {
		auto _aClound = Sprite::create("cloud.png");
		_aClound->setScale(screenSize.height / _aClound->getContentSize().height * (0.1f + CCRANDOM_0_1() * 0.05f));
		_aClound->setAnchorPoint(Vec2(0.0f, 0.0f));
		_aClound->setPosition(screenSize.width * (1.0f + CCRANDOM_0_1() * 0.5f), screenSize.height * (0.6f + i * 0.04f + CCRANDOM_0_1() * 0.04f));
		_aClound->setOpacity(220.0f);
		this->addChild(_aClound, 9);

		MoveBy *_pMove1 = MoveBy::create((15.0f + CCRANDOM_0_1() * 15.0f), Vec2(-screenSize.width * 2.0f, 0.0f));
		MoveBy *_pMove2 = MoveBy::create(0.0f, Vec2(screenSize.width * 2.0f, 0.0f));
		Sequence *_pMoveSequence = Sequence::create(_pMove1, _pMove2, NULL);
		RepeatForever* _pMoveRepeat = RepeatForever::create(_pMoveSequence);
		_aClound->runAction(_pMoveRepeat);
	}
}