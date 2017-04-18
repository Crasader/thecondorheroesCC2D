#include "SelectStageScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "RefManager.h"

Scene * SelectStageLayer::createScene(int charId)
{
	auto scene = Scene::create();
	auto layer = SelectStageLayer::create(charId);

	scene->addChild(layer);
	return scene;
}

bool SelectStageLayer::init(int charId)
{
	auto originXY = Director::getInstance()->getVisibleOrigin();
	auto screenSize = Director::getInstance()->getVisibleSize();

	auto tmxMap = TMXTiledMap::create("config_map.tmx");
	tmxMap->setPosition(originXY);
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

	Menu* menu = Menu::create();
	for (auto child : grMapBtn->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * scaleY, mObject["y"].asFloat() * scaleY);

		// get mapId on tmx map

		MenuItemImage* mapBtn;
		int stage = mObject["stage"].asInt();
		int mapId = mObject["mapId"].asInt();
		int haveBoss = mObject["haveBoss"].asInt();

		if (stage < currentStageUnlocked) {
			mapBtn = MenuItemImage::create("un_locked.png", "un_locked.png",
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, stage, mapId, haveBoss, charId));
		}
		else if (stage == currentStageUnlocked) {
			if (mapId <= currentMapUnlocked) {
				mapBtn = MenuItemImage::create("un_locked.png", "un_locked.png",
					CC_CALLBACK_0(SelectStageLayer::gotoPlay, this, stage, mapId, haveBoss, charId));
			}
			else {
				mapBtn = MenuItemImage::create("locked.png", "locked.png",
					CC_CALLBACK_0(SelectStageLayer::doNothing, this));
			}

		}
		else {
			mapBtn = MenuItemImage::create("locked.png", "locked.png",
				CC_CALLBACK_0(SelectStageLayer::doNothing, this));
		}

		mapBtn->setPosition(originXY + origin);
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

void SelectStageLayer::gotoPlay(int stage, int map, int haveBoss, int charId)
{
	auto _aScene = GameScene::createScene(stage, map, haveBoss, charId);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene));
}

void SelectStageLayer::doNothing()
{
}

void SelectStageLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		auto _aScene = MenuLayer::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _aScene));
	}
}
