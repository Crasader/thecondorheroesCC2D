#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "SelectStageScene.h"
#include "LoadingLayer.h"
#include "manager/RefManager.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"

#include "colong/CoLong.h"
#include "duongqua/DuongQua.h"
#include "quachtinh/QuachTinh.h"

LayerColor *blur;

Scene* GameScene::createScene(GameScene *layer, Hud* m_hud)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object

	m_hud->setPosition(Director::getInstance()->getVisibleOrigin());
	
	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(m_hud);


	blur = LayerColor::create(Color4B(0, 0, 0, 170));
	blur->setVisible(false);
	scene->addChild(blur);

	layer->onBegin();

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init(int stage, int map, int charId)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	AudioManager::stopMusic();
	GAHelper::getInstance()->logScreen(StringUtils::format("Stage: %d, Map: %d", stage, map));
	isFirstPlay = REF->getIsFirstPlay();


	isModeDebug = false;

	changebg = 0;

	indexOfNextMapBoss = -1;
	this->stage = stage;
	this->charId = charId;
	this->map = map;

	this->isWinGame = false;

	initB2World();
	// cache batchnode
	batchNode = SpriteBatchNode::create("coin_01.png");
	this->addChild(batchNode);
	// test

	loadBackground();
	danceWithCamera();

	selectHero();

	if (REF->getNumberItemHealth() > 0) {
		hero->setHealth(hero->getHealth() + 1);
		hero->setMaxHealth(hero->getHealth());
	}


	loadPosAndTag();
	createGroundBody();
	createItem();
	initLayerToAddAgent();

	if (this->haveboss)
		creatBoss();

	createCoin();
	return true;
}

GameScene * GameScene::create(int stage, int map, int charId)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(stage, map, charId))
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

void GameScene::enableCalling()
{
	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		if (haveboss && posXComingBoss < 0) {	// means: hero pass over posXComingBoss
												// do nothing
		}
		else
			hud->getBtnCalling()->setEnabled(true);
	}
}


void GameScene::selectHero()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	heroStartPosition = Point(origin.x, visibleSize.height * 0.75f);

	switch (charId)
	{
	case 0:
		createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas");
		break;

	case 1:
		createCoLong("Animation/CoLong/CoLong.json", "Animation/CoLong/CoLong.atlas");
		break;

	case 4:
		createQuachTinh("Animation/QuachTinh/QuachTinh.json", "Animation/QuachTinh/QuachTinh.atlas");
		break;

	default:
		createCoLong("Animation/CoLong/CoLong.json", "Animation/CoLong/CoLong.atlas");
		break;
	}
}

void GameScene::createDuongQua(string path_Json, string path_Atlas)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(heroStartPosition);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();
	hero->createPool();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}

void GameScene::createCoLong(string path_Json, string path_Atlas)
{
	hero = CoLong::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(heroStartPosition);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();
	hero->createPool();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}

void GameScene::createQuachTinh(string path_Json, string path_Atlas)
{
	hero = QuachTinh::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 300);
	hero->listener();
	hero->setPosition(heroStartPosition);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();
	hero->createPool();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}


void GameScene::createEagle(Point position)
{
	_aEagle = ChimDieu::create("Animation/ChimDieu/ChimDieu.json",
		"Animation/ChimDieu/ChimDieu.atlas", SCREEN_SIZE.height / 2048);
	_aEagle->initCirclePhysic(world, position);
	this->addChild(_aEagle, ZORDER_HERO);
	_aEagle->setStringHero(charId == 0 ? "DuongQua" : "CoLong");
}

void GameScene::onBegin()
{
	switch (stage)
	{
	case 1: {

		AudioManager::playMusic(MUSIC_STAGE1);
		break;
	}
	case 2: {
		AudioManager::playMusic(MUSIC_STAGE2);
		break;
	}
	case 3: {
		AudioManager::playMusic(MUSIC_STAGE3);
		break;
	}
	case 4: {
		AudioManager::playMusic(MUSIC_STAGE4);
		break;
	}
	}

	if (hud->getBtnCalling() != nullptr) {
		createEagle(Point(hero->getB2Body()->GetPosition().x - SCREEN_SIZE.width, SCREEN_SIZE.height / 2));
		hud->getBtnCalling()->setEnabled(true);
	}

	hud->getPauseItem()->setEnabled(true);

	if (REF->getNumberItemDoubleGold() > 0) {
		runnerItem(Item_type::DOUBLE_COIN, DURATION_DOUBLE_COIN);
		REF->decreaseNumberItemDoubleGold();
	}

	if (REF->getNumberItemMagnet() > 0) {
		charId == 1 ?
			runnerItem(Item_type::MAGNET, DURATION_MAGNET * 1.15f) :
			runnerItem(Item_type::MAGNET, DURATION_MAGNET);
		REF->decreaseNumberItemMagnet();
	}

	auto key_listener = EventListenerKeyboard::create();

	key_listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);

	if (!isFirstPlay) {
		hud->addEvents();
		touch_listener = EventListenerTouchOneByOne::create();
		touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
	}

	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
	this->scheduleUpdate();
}

void GameScene::checkActiveButton()
{
	if (hud->getBtnSkill_1() == nullptr) return;
	if (hero->getIsDriverEagle() ||
		hero->getPositionY() + hero->getTrueRadiusOfHero() * 2 < 0) {

		enableCalling();

		return;
	}

	if (hero->getFSM()->currentState == MDie) return;

	if (hud->getBtnSkill_1()->getIsBlocked()) {		// 2 and 3 active
		if (hud->getBtnSkill_1()->getMain()->getPercentage() >= 99.9f &&
			hud->getBtnSkill_1()->getMain()->isVisible()) {
			hud->getBtnSkill_1()->getMain()->setVisible(false);
		}

		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				if (hero->getFSM()->currentState != MRevive && !hud->getBtnSkill_2()->getNumberOfUseHasNotUsedYet() >= 1)
					hero->killThemAll();
				hero->getActiveSkill()->setVisible(false);

				enableCalling();

				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_1()->getMain()->isVisible())
					hud->getBtnSkill_1()->getMain()->setVisible(true);

				hud->getBtnSkill_1()->setIsBlocked(false);
			}

		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {

				if (hud->getBtnSkill_3()->getNumberOfUseHasNotUsedYet() >= 1) {
					hud->getBtnSkill_3()->setCanTouch(true);
					hud->getBtnSkill_3()->getMain()->setVisible(true);
				}

				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_1()->getMain()->isVisible())
					hud->getBtnSkill_1()->getMain()->setVisible(true);

				hud->getBtnSkill_1()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_2()->getIsBlocked()) {		// 1 and 3 active
		if (hud->getBtnSkill_2()->getMain()->getPercentage() >= 99.9f &&
			hud->getBtnSkill_2()->getMain()->isVisible()) {
			hud->getBtnSkill_2()->getMain()->setVisible(false);
		}
		if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {

				if (hud->getBtnSkill_1()->getNumberOfUseHasNotUsedYet() >= 1) {
					hud->getBtnSkill_1()->setCanTouch(true);
					hud->getBtnSkill_1()->getMain()->setVisible(true);
				}

				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_2()->getMain()->isVisible())
					hud->getBtnSkill_2()->getMain()->setVisible(true);

				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {
				if (hero->getFSM()->currentState != MRevive && !hud->getBtnSkill_3()->getNumberOfUseHasNotUsedYet() >= 1)
					hero->killThemAll();
				hero->getActiveSkill()->setVisible(false);

				enableCalling();

				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_2()->getMain()->isVisible())
					hud->getBtnSkill_2()->getMain()->setVisible(true);

				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_3()->getIsBlocked()) {		// 2 and 1 active
		if (hud->getBtnSkill_3()->getMain()->getPercentage() >= 99.9f &&
			hud->getBtnSkill_3()->getMain()->isVisible()) {
			hud->getBtnSkill_3()->getMain()->setVisible(false);
		}
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {

				if (hud->getBtnSkill_2()->getNumberOfUseHasNotUsedYet() >= 1) {
					hud->getBtnSkill_2()->setCanTouch(true);
					hud->getBtnSkill_2()->getMain()->setVisible(true);
				}

				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_3()->getMain()->isVisible())
					hud->getBtnSkill_3()->getMain()->setVisible(true);

				hud->getBtnSkill_3()->setIsBlocked(false);
			}

		}
		else if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {
				if (hero->getFSM()->currentState != MRevive && !hud->getBtnSkill_1()->getNumberOfUseHasNotUsedYet() >= 1)
					hero->killThemAll();
				hero->getActiveSkill()->setVisible(false);

				enableCalling();

				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_3()->getMain()->isVisible())
					hud->getBtnSkill_3()->getMain()->setVisible(true);

				hud->getBtnSkill_3()->setIsBlocked(false);
			}
		}

	}

}

void GameScene::listener()
{
	if (hero->getB2Body() == nullptr)
		return;

	if (hero->getFSM()->currentState == MDie) {
		if (hud->getBtnSkill_1()->getListener() != nullptr) {
			hud->pauseIfVisible();
			if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
				hud->getBtnCalling()->setEnabled(false);
			}

			hud->getPauseItem()->setEnabled(false);

			hero->setItemValue(KEY_ITEM_MAGNET, 0);
			hero->setItemValue(KEY_ITEM_DOUPLE_COIN, 0);

			if (hero->getActiveSkill()->isVisible())
				hero->getActiveSkill()->setVisible(false);

			if (hero->getSuctionCoin()->isVisible())
				hero->getSuctionCoin()->setVisible(false);
		}
	}

	if (hud->getBtnAttack() == nullptr) return;
	if (hud->getBtnAttack()->getIsActive() && !hud->getBtnAttack()->getIsBlocked()
		&& hero->getFSM()->currentState != MInjured) {

		hero->getFSM()->changeState(MAttack);

		hud->getBtnAttack()->setIsActive(false);
	}

	if (hud->getBtnSkill_1() == nullptr) return;
	if (hud->getBtnSkill_1()->getIsActive() && !hud->getBtnSkill_1()->getIsBlocked()
		&& hero->getFSM()->currentState != MInjured) {

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isEnabled()) {
			hud->getBtnCalling()->setEnabled(false);
		}
		currentButton = 1;

		hero->setIsDoneDuration1(false);
		hero->doCounterSkill1();

		hud->runnerSkillDuration(1, hero->getDurationSkill1());

		hud->getBtnSkill_2()->setIsBlocked(true);
		if (hud->getBtnSkill_2()->getCanTouch())
			hud->getBtnSkill_2()->getMain()->setVisible(false);


		hud->getBtnSkill_3()->setIsBlocked(true);
		if (hud->getBtnSkill_3()->getCanTouch())
			hud->getBtnSkill_3()->getMain()->setVisible(false);


		hud->getBtnSkill_1()->setIsActive(false);
		hero->getActiveSkill()->setVisible(true);
	}

	if (hud->getBtnSkill_2()->getIsActive() && !hud->getBtnSkill_2()->getIsBlocked()
		&& hero->getFSM()->currentState != MInjured) {
		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isEnabled()) {
			hud->getBtnCalling()->setEnabled(false);
		}
		currentButton = 2;

		hero->setIsDoneDuration2(false);
		hero->doCounterSkill2();

		hud->runnerSkillDuration(2, hero->getDurationSkill2());

		hud->getBtnSkill_1()->setIsBlocked(true);
		if (hud->getBtnSkill_1()->getCanTouch())
			hud->getBtnSkill_1()->getMain()->setVisible(false);

		hud->getBtnSkill_3()->setIsBlocked(true);
		if (hud->getBtnSkill_3()->getCanTouch())
			hud->getBtnSkill_3()->getMain()->setVisible(false);


		hud->getBtnSkill_2()->setIsActive(false);
		hero->getActiveSkill()->setVisible(true);
	}

	if (hud->getBtnSkill_3()->getIsActive() && !hud->getBtnSkill_3()->getIsBlocked()
		&& hero->getFSM()->currentState != MInjured) {
		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isEnabled()) {
			hud->getBtnCalling()->setEnabled(false);
		}

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isEnabled()) {
			hud->getBtnCalling()->setEnabled(false);
		}

		currentButton = 3;

		hero->setIsDoneDuration3(false);
		hero->doCounterSkill3();

		hud->runnerSkillDuration(3, hero->getDurationSkill3());

		// block
		hud->getBtnSkill_1()->setIsBlocked(true);
		if (hud->getBtnSkill_1()->getCanTouch())
			hud->getBtnSkill_1()->getMain()->setVisible(false);

		hud->getBtnSkill_2()->setIsBlocked(true);
		if (hud->getBtnSkill_2()->getCanTouch())
			hud->getBtnSkill_2()->getMain()->setVisible(false);

		hud->getBtnSkill_3()->setIsActive(false);
		hero->getActiveSkill()->setVisible(true);
	}

}

void GameScene::heroGetOffEagle() {
	hud->showButton();
	hero->setIsNoDie(false);
	_aEagle->changeBodyCategoryBits(0);
	_aEagle->changeBodyMaskBits(0);
	hero->setIsDriverEagle(false);
	hud->setBtnCallingHintDone(false);
	hero->setVisible(true);
	hero->getB2Body()->SetGravityScale(1);
	hero->getFSM()->changeState(MLand);
	_aEagle->flyAway();
}

void GameScene::update(float dt)
{

	updateB2World(dt);
	listener();
	updateLayer();
	updateChangeBg();
	if (m_fMultiKillsCounterTime > 0.0f) {
		m_fMultiKillsCounterTime -= dt;
	}
	else {
		m_nMultiKills = 0;
	}

	checkActiveButton();

	if (_aEagle != nullptr) {
		_aEagle->updateMe(dt);
		if (_aEagle->getIsAbleToDropHero()) {
			heroGetOffEagle();
			_aEagle->setIsAbleToDropHero(false);
		}
	}

	if (hero->getIsDriverEagle()) {
		if (hero->getHealth() <= 0) {	// die
			hero->getB2Body()->SetGravityScale(1);
			hero->getB2Body()->SetLinearVelocity(b2Vec2(0, -hero->getJumpVel()));
			hero->setVisible(true);
			_aEagle->stopAllActions();
			_aEagle->flyAway();
		}
		else {
			hero->getB2Body()->SetTransform(b2Vec2(
				_aEagle->getB2Body()->GetPosition().x,
				_aEagle->getB2Body()->GetPosition().y + 40.0f / PTM_RATIO), 0.0f);
		}
	}

	hero->updateMe(dt);

	updateEnemy();
	updateCamera();
	updateCoin();

	if (!listItem.empty()) {
		if (numberDeadItem >= listItem.size()) {
			listItem.clear();
		}

		for (auto item : listItem) {
			if (item->getB2Body() == nullptr) continue;
			if (item->getTaken() || follow->getPositionX() - item->getPositionX() - item->getBoundingBox().size.width
		> SCREEN_SIZE.width / 2) {
				world->DestroyBody(item->getB2Body());
				item->setB2Body(nullptr);
				item->setVisible(false);
				item->removeFromParentAndCleanup(true);
				numberDeadItem++;
			}
			else
				item->updateMe(hero);
		}
	}


	// fall down some hole
	if (hero->getHealth() > 0) {
		if (hero->getPositionY() + hero->getTrueRadiusOfHero() * 2.8f <= follow->getPositionY() - SCREEN_SIZE.height / 2) {

			// with every single hero
			switch (charId) {
			case 0:
				hero->stopSkillAction(false, false, true);
				break;

			default:
				hero->stopSkillAction(true, false, true);
				break;
			}

			if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {

				hud->hideButton();
				hud->moveCallBirdToCenterScreen(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
				hero->setOnGround(false);
				hero->setVisible(false);
				hero->getB2Body()->SetGravityScale(0);
				hero->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				hero->getB2Body()->SetTransform(b2Vec2(hero->getB2Body()->GetPosition().x, hero->getB2Body()->GetPosition().y + 1), 0.0f);

			}
			else {
				overGame();
			}
		}

		if (hud->getBtnCallingHintDone() && !hero->getIsDriverEagle()) {
			static float g_fTimeCounter = 0.0f;
			g_fTimeCounter += dt;
			if (g_fTimeCounter > 5.0f / 180) {
				bool _bResult = hud->callBirdCooldown();
				if (!_bResult) {
					hud->setBtnCallingHintDone(false);
					overGame();
				}
				g_fTimeCounter = 0.0f;
			}
		}
	}

	if (haveboss) {
		if (hero->getPositionX() > tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width / 2 && indexOfNextMapBoss < 0) {
			createGroundForMapBoss();
			indexOfNextMapBoss = 1;
		}

		if (hero->getPositionX() > tmx_mapboss[indexOfNextMapBoss]->getPositionX() + SCREEN_SIZE.width && indexOfNextMapBoss >= 0) {
			TMXTiledMap* tmpmap;
			indexOfNextMapBoss == 1 ? tmpmap = tmx_mapboss[0] : tmpmap = tmx_mapboss[1];
			tmpmap->setPositionX(tmx_mapboss[indexOfNextMapBoss]->getPositionX() + tmx_mapboss[indexOfNextMapBoss]->getBoundingBox().size.width);
			indexOfNextMapBoss == 1 ? indexOfNextMapBoss = 0 : indexOfNextMapBoss = 1;
		}
	}


	if (hero->getBloodScreen()->isVisible())
		hero->getBloodScreen()->setPosition(follow->getPosition());

	if (posXComingBoss > 0) {
		if (hero->getPositionX() >= posXComingBoss) {
			if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isEnabled()) {
				CustomLayerToToast *_pToast = CustomLayerToToast::create(JSHERO->getNotifyAtX(6), TOAST_LONG);
				_pToast->setPosition(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 4));
				this->getParent()->addChild(_pToast, 10);

				hud->getBtnCalling()->setEnabled(false);
				posXComingBoss = -1;
			}
		}
	}

	if (isFirstPlay && (stage == 1 && map == 1)) {
		tutorial();
	}

	updateHUD(dt);
}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 10.0f / 3.0f / PTM_RATIO));

	// draw debug
	if (isModeDebug) {
		auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
		world->SetDebugDraw(debugDraw);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;

		debugDraw->SetFlags(flags);
	}

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	auto collisionListener = new CollisionListener();
	world->SetContactListener(collisionListener);
}

void GameScene::updateB2World(float dt)
{
	int positionIterations = 3;		// position
	int velocityIterations = 8;		// velocity

	world->Step(dt, velocityIterations, positionIterations);
}

void GameScene::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	Layer::draw(renderer, transform, flags);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this);
	renderer->addCommand(&_customCommand);

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GameScene::onDraw()
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");

	auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
	world->DrawDebugData();
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}


void GameScene::loadBackground()
{
	// khoi tao map
	tmx_map = TMXTiledMap::create(StringUtils::format("Map/map%d/map%d.tmx", stage, map));
	tmx_map->setAnchorPoint(Point::ZERO);
	float numberOfTileHeight = tmx_map->getMapSize().height;
	scaleOfMap = SCREEN_SIZE.height / (tmx_map->getContentSize().height / numberOfTileHeight * 10);

	tmx_map->setScale(scaleOfMap);

	tmx_map->setPosition(Point::ZERO);

	if (isModeDebug)
		tmx_map->setVisible(false);

	this->haveboss = tmx_map->getObjectGroup("boss") != nullptr ? 1 : 0;

	this->addChild(tmx_map, ZORDER_TMX);
	this->haveboss = tmx_map->getObjectGroup("boss") != nullptr ? 1 : 0;
	if (haveboss) {

		auto groupComingBoss = tmx_map->getObjectGroup("coming_boss");
		auto mObjectX = groupComingBoss->getObject("coming_boss");
		posXComingBoss = mObjectX["x"].asFloat() * scaleOfMap;

		for (int i = 0; i < 2; i++) {
			tmx_mapboss[i] = TMXTiledMap::create(StringUtils::format("Map/map%d/mapboss.tmx", stage));
			tmx_mapboss[i]->setAnchorPoint(Point::ZERO);
			tmx_mapboss[i]->setScale(scaleOfMap);
			if (isModeDebug)
				tmx_mapboss[i]->setVisible(false);
		}

		tmx_mapboss[0]->setPosition(tmx_map->getPosition() + Vec2(tmx_map->getBoundingBox().size.width, 0));
		tmx_mapboss[1]->setPosition(tmx_mapboss[0]->getPosition() + Vec2(tmx_mapboss[0]->getBoundingBox().size.width, 0));
		this->addChild(tmx_mapboss[0], ZORDER_BG2);
		this->addChild(tmx_mapboss[1], ZORDER_BG2);
	}

	if (isFirstPlay) {
		auto groupTut = tmx_map->getObjectGroup("tut");
		for (auto child : groupTut->getObjects()) {
			auto mObjectX = child.asValueMap();

			float origin_X = mObjectX["x"].asFloat() * scaleOfMap;

			switch (mObjectX["order"].asInt())
			{
			case 1:
				posXJump1Tut = origin_X;
				break;

			case 2:
				posXJump2Tut = origin_X;
				break;

			case 3:
				posXAttackTut = origin_X;
				break;

			case 4:
				posXSkillTut = origin_X;
				break;

			case 5:
				posXIntroBird = origin_X;
				break;
			}
		}
	}

	createInfiniteNode();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::create(StringUtils::format("Map/map%d/bg%d_1.png", stage, map));
	bg1_1->setScale(SCREEN_SIZE.width / (bg1_1->getContentSize().width));
	bg1_1->setAnchorPoint(Point(0, 0.5f));
	//bg1_1->setVisible(false);

	auto bg1_2 = Sprite::create(StringUtils::format("Map/map%d/bg%d_1.png", stage, map));
	bg1_2->setScale(SCREEN_SIZE.width / (bg1_2->getContentSize().width));
	bg1_2->setAnchorPoint(Point(0, 0.5f));
	//bg1_2->setVisible(false);

	background->addChild(bg1_1, 1, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 1, Vec2(0.5f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));

	auto moonGr = tmx_map->getObjectGroup("moon");
	if (moonGr) {
		auto object = moonGr->getObject("moon");
		auto pos = Point(object["x"].asFloat()*tmx_map->getScale(), object["y"].asFloat()*tmx_map->getScale());
		auto moon = Sprite::create("moon.png");
		moon->setScale(SCREEN_SIZE.height / 4 / moon->getContentSize().height);
		background->addChild(moon, 2, Vec2(0, 1), Vec2(pos.x, pos.y - SCREEN_SIZE.height / 2));
	}

	auto changebgGR = tmx_map->getObjectGroup("changebg");
	if (changebgGR) {
		auto object = changebgGR->getObject("changebg");
		auto pos = Point(object["x"].asFloat()*tmx_map->getScale(), object["y"].asFloat()*tmx_map->getScale());
		//auto moon = Sprite::create("moon.png");
		//moon->setScale(SCREEN_SIZE.height / 4 / moon->getContentSize().height);
		//background->addChild(moon, 2, Vec2(0, 1), Vec2(pos.x, pos.y - SCREEN_SIZE.height / 2));
		changebg = pos.x;
	}
	if ((stage == 3 && map == 2) || (stage == 4 && map == 2)) {}
	else {
		auto bg2_1 = Sprite::create(StringUtils::format("Map/map%d/bg%d_2.png", stage, map));
		//auto bg2_1 = Sprite::create("moon.png");
		bg2_1->setScale(SCREEN_SIZE.width / (bg2_1->getContentSize().width));
		bg2_1->setAnchorPoint(Point(0, 0.5f));
		bg2_1->setTag(21);
		//bg2_1->setVisible(false);


		auto bg2_2 = Sprite::create(StringUtils::format("Map/map%d/bg%d_2.png", stage, map));
		//auto bg2_2 = Sprite::create("moon.png");
		bg2_2->setScale(SCREEN_SIZE.width / (bg2_2->getContentSize().width));
		bg2_2->setAnchorPoint(Point(0, 0.5f));
		bg2_2->setTag(22);
		//bg2_2->setVisible(false);
		background->addChild(bg2_1, 3, Vec2(0.7f, 1), Vec2(0, 0));
		background->addChild(bg2_2, 3, Vec2(0.7f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));

	}


	//background->setPosition(Point(-SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	background->setPosition(Point(0, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	if (isModeDebug)
		background->setVisible(false);
	this->addChild(background, ZORDER_BG);

	background2 = InfiniteParallaxNode::create();
	if (stage == 1 || (stage == 3 && map == 2) || (stage == 4 && map == 1)) {

		auto bg3_1 = Sprite::create(StringUtils::format("Map/map%d/bg3.png", stage));
		bg3_1->setScaleX(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
		bg3_1->setScaleY(bg3_1->getScaleX()*0.7f);
		//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
		bg3_1->setAnchorPoint(Point(0, 0));

		auto bg3_2 = Sprite::create(StringUtils::format("Map/map%d/bg3.png", stage));
		bg3_2->setScale(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
		bg3_2->setScaleY(bg3_2->getScaleX()*0.7f);
		//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
		bg3_2->setAnchorPoint(Point(0, 0));

		background2->addChild(bg3_1, 0, Vec2(0.8f, 1), Vec2(0, 0));
		background2->addChild(bg3_2, 0, Vec2(0.8f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	}

	background2->setPosition(Point(0, 0));
	background2->setAnchorPoint(Point(0, 0.0f));
	if (stage == 3) {
		this->addChild(background2, ZORDER_TMX + 1);
	}
	else {
		this->addChild(background2, ZORDER_BG);
	}
}

void GameScene::createGroundBody()
{
	auto groupGround = tmx_map->getObjectGroup("ground");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initGroundPhysic(world, pos, sizeOfBound);
	}

	if (charId == 0) {
		auto groupUnderGround = tmx_map->getObjectGroup("under_ground");
		if (!groupUnderGround) return;
		for (auto child : groupUnderGround->getObjects()) {
			auto mObject = child.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
			initUnderGroundPhysic(world, pos, sizeOfBound);
		}
	}
}

void GameScene::createGroundForMapBoss()
{
	auto groupGround = tmx_mapboss[0]->getObjectGroup("ground");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap) + tmx_mapboss[0]->getPosition();
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initGroundPhysic(world, origin, Size(INT_MAX / 4, sizeOfBound.height));
	}

	if (charId == 0) {
		auto groupUnderGround = tmx_mapboss[0]->getObjectGroup("under_ground");
		if (!groupUnderGround) return;
		for (auto child : groupUnderGround->getObjects()) {
			auto mObject = child.asValueMap();
			Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap) + tmx_mapboss[0]->getPosition();
			Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
			//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
			initUnderGroundPhysic(world, origin, Size(INT_MAX / 4, sizeOfBound.height));
		}
	}
}

void GameScene::creatEnemyWooder(MyLayer * layer, Vec2 pos)
{

	if (layer->wooderPool) {
		auto enemy = (EnemyWooder*)layer->wooderPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2.5f));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyToanChanStudent(MyLayer * layer, Vec2 pos)
{
	if (layer->toanchan1Pool) {
		auto enemy = (EnemyToanChanStudent*)layer->toanchan1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 4));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyToanChanStudent2(MyLayer * layer, Vec2 pos)
{
	if (layer->toanchan2Pool) {
		auto enemy = (EnemyToanChanStudent2*)layer->toanchan2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->setControlAttack(100);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();
		enemy->listener();
	}
}

void GameScene::createEnemyTNB(MyLayer * layer, Vec2 pos)
{
	if (layer->tnbPool) {
		auto enemy = (EnemyTNB*)layer->tnbPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 1.5f));
		enemy->makeMask();

		enemy->listener();
	}
}


void GameScene::createEnemyHongLangBa(MyLayer * layer, Vec2 pos) {
	//auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401;
	if (layer->hongLangBa1Pool) {
		auto enemy = (EnemyHongLangBa*)layer->hongLangBa1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		//enemy->genSplash();
		enemy->listener();
	}
}

void GameScene::createEnemyHongLangBa2(MyLayer * layer, Vec2 pos) {
	if (layer->hongLangBa2Pool) {
		auto enemy = (EnemyHongLangBa2*)layer->hongLangBa2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->setControlAttack(100);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyToOng(MyLayer * layer, Vec2 pos) {
	if (layer->toOngPool) {
		auto enemy = (EnemyToOng*)layer->toOngPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 4));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyHoacDo(MyLayer * layer, Vec2 pos)
{
	if (layer->hoacDo1Pool) {
		auto enemy = (EnemyHoacDo*)layer->hoacDo1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();


		enemy->listener();
	}
}

void GameScene::createEnemyHoacDo2(MyLayer * layer, Vec2 pos)
{
	if (layer->hoacDo2Pool) {
		auto enemy = (EnemyHoacDo2*)layer->hoacDo2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->setControlAttack(100);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyDatNhiBa(MyLayer * layer, Vec2 pos)
{
	if (layer->datNhiBa1Pool) {
		auto enemy = (EnemyDatNhiBa*)layer->datNhiBa1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setHealth(2);
		enemy->setCanRun(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyDatNhiBa2(MyLayer * layer, Vec2 pos)
{
	if (layer->datNhiBa2Pool) {
		auto enemy = (EnemyDatNhiBa2*)layer->datNhiBa2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setHealth(2);
		enemy->setCanRun(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}
		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 3));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyChong1(MyLayer * layer, Vec2 pos)
{
	if (layer->chong1Pool) {
		auto enemy = (EnemyChong1*)layer->chong1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setAnimation(0, "idle", true);
		enemy->setToSetupPose();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyChong2(MyLayer * layer, Vec2 pos)
{
	if (layer->chong2Pool) {
		auto enemy = (EnemyChong2*)layer->chong2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "appear", false);
		enemy->pause();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		//enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyChong3(MyLayer * layer, Vec2 pos)
{
	if (layer->chong3Pool) {
		auto enemy = (EnemyChong3*)layer->chong3Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "appear", false);
		enemy->pause();
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyLinhCamGiao(MyLayer * layer, Vec2 pos)
{
	if (layer->linhcamgiao1Pool) {
		auto enemy = (EnemyLinhCamGiao*)layer->linhcamgiao1Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resume();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "idle", true);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyLinhCamGiao2(MyLayer * layer, Vec2 pos)
{
	if (layer->linhcamgiao2Pool) {
		auto enemy = (EnemyLinhCamGiao2*)layer->linhcamgiao2Pool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "idle", true);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyLinhTenThang(MyLayer * layer, Vec2 pos)
{
	if (layer->linhtenthangPool) {
		auto enemy = (EnemyLinhTenThang*)layer->linhtenthangPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "idle", true);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyLinhTenXien(MyLayer * layer, Vec2 pos)
{
	if (layer->linhtenxienPool) {
		auto enemy = (EnemyLinhTenXien*)layer->linhtenxienPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resumeSchedulerAndActions();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "idle", true);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::createEnemyLinhCamRoi(MyLayer * layer, Vec2 pos)
{
	if (layer->linhcamroiPool) {
		auto enemy = (EnemyLinhCamRoi*)layer->linhcamroiPool->getObject();
		enemy->setIsDie(false);
		enemy->setIsEndOfScreen(false);
		enemy->setPosition(pos);
		enemy->setVisible(true);
		enemy->resume();
		enemy->clearTracks();
		enemy->setToSetupPose();
		enemy->setAnimation(0, "idle", true);
		enemy->update(0.0f);
		//layer->addChild(enemy, ZORDER_ENEMY);
		if (enemy->getB2Body()) {
			world->DestroyBody(enemy->getB2Body());
		}

		enemy->initBoxPhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
		enemy->makeMask();

		enemy->listener();
	}
}

void GameScene::creatBoss()
{
	auto groupBoss = tmx_map->getObjectGroup("boss");
	if (!groupBoss) return;
	for (auto child : groupBoss->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 3.0f / 560; // 560 la height cua spine
		/*auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);*/
		EnemyBoss1* enemy;
		switch (stage)
		{
		case 1: {
			enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
				"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);
			break;
		}
		case 2: {
			enemy = EnemyBoss2::create("Animation/Enemy_Boss2/Boss2.json",
				"Animation/Enemy_Boss2/Boss2.atlas", scaleOfEnemy);
			break;
		}
		case 3: {
			enemy = EnemyBoss3::create("Animation/Enemy_Boss3/Boss3.json",
				"Animation/Enemy_Boss3/Boss3.atlas", scaleOfEnemy);
			break;
		}
		default:
			break;
		}

		//auto enemy = EnemyBoss1::create("Effect/exxp.json", "Effect/exxp.atlas", scaleOfEnemy);
		enemy->setPosition(origin);
		//enemy->setVisible(false);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
		enemy->changeBodyCategoryBits(BITMASK_BOSS);
		enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		//enemy->listener();
		enemy->createPool();
		enemy->creatHpSprite();
		enemy->listener();

		enemy->createCoinPool();
	}
}

void GameScene::createCoin()
{
	createFormCoin("coin_tim", "Map/tim.tmx", "tim");
	createFormCoin("coin_straight", "Map/straight.tmx", "straight");
	createFormCoin("coin_parabol", "Map/parapol.tmx", "parapol");
	createFormCoin("coin_square", "Map/square.tmx", "square");
	createFormCoin("coin_zigzag", "Map/zigzag.tmx", "zigzag");
	createFormCoin("coin_zigzag2", "Map/zigzag2.tmx", "zigzag2");
	createFormCoin("coin_circle", "Map/circle.tmx", "circle");
	createCointBag();
	createCoinBullion();
}


void GameScene::createCointBag()
{

	auto group = tmx_map->getObjectGroup("coin_bag");
	if (!group) return;
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 5.0f / 123; //  la height cua spine
		auto coin = CoinBag::create("Gold_bag", scaleOfEnemy);
		coin->setPosition(origin);
		coin->setVisible(false);
		//coin->pause();
		this->addChild(coin, ZORDER_ENEMY);
		coin->initCirclePhysic(world, origin);
		coin->getB2Body()->SetType(b2_staticBody);
		coin->getB2Body()->GetFixtureList()->SetSensor(true);
		coin->changeBodyCategoryBits(BITMASK_COIN_BAG);
		coin->changeBodyMaskBits(BITMASK_SWORD);
		//coin->update(0);
	}

}

void GameScene::createCoinBullion()
{

	auto group = tmx_map->getObjectGroup("coin_bullion");
	if (!group) return;
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 6.0f / 118; //  la height cua spine
		auto coin = CoinBullion::create("gold", scaleOfEnemy);
		coin->setPosition(origin);
		coin->setVisible(false);
		//coin->setAni
		coin->pause();
		this->addChild(coin, ZORDER_ENEMY);
		coin->initCirclePhysic(world, origin);
		coin->getB2Body()->SetType(b2_staticBody);
		coin->getB2Body()->GetFixtureList()->SetSensor(true);
		coin->changeBodyCategoryBits(BITMASK_COIN_BULLION);
		coin->changeBodyMaskBits(BITMASK_HERO);
		//coin->update(0);
	}

}
void GameScene::createFormCoin(string objectName, string objectMap, string objectInForm)
{
	auto group = tmx_map->getObjectGroup(objectName);
	if (!group) return;
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto tmx = TMXTiledMap::create(objectMap);
		auto groupCoin = tmx->getObjectGroup(objectInForm);
		for (auto c : groupCoin->getObjects()) {
			auto mObject2 = c.asValueMap();
			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
			auto coin = Coin::create();
			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
			coin->setScale(scale);
			coin->setPosition(origin + origin2);
			this->addChild(coin, ZORDER_ENEMY);
			coin->initCirclePhysic(world, origin + origin2);
			coin->changeBodyCategoryBits(BITMASK_COIN);
			coin->changeBodyMaskBits(0);//22/4 thuandv edited
			coin->setVisible(false);
			coin->runAnimation();
			coin->pause();
		}
	}

}

void GameScene::createItem()
{
	auto groupItem = tmx_map->getObjectGroup("item");
	if (!groupItem) return;
	for (auto child : groupItem->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);

		auto type_val = mObject["type"].asInt();
		Item *item;
		switch (type_val)
		{
		case 0:				// health
			item = Item::create("UI/UI_main_menu/item1_health.png", Item_type::HEALTH, origin);
			break;
		case 1:
			item = Item::create("UI/UI_main_menu/item4_doublecoin.png", Item_type::DOUBLE_COIN, origin);
			break;
		case 2:
			item = Item::create("UI/UI_main_menu/item3_magnet.png", Item_type::MAGNET, origin);
			break;
		}

		item->setScale(SCREEN_SIZE.height / 9.0f / item->getContentSize().height);
		item->initCirclePhysic(world, origin);
		addChild(item, ZORDER_SMT);
		listItem.push_back(item);
	}
}

void GameScene::updateQuest()
{

	switch (charId)
	{
	case 0:
		REF->setUpNumberQuest(INDEX_QUEST_DQ, hero->getScore());
		break;

	case 1:
		REF->setUpNumberQuest(INDEX_QUEST_TNL, hero->getScore());
		break;

	default:
		REF->setUpNumberQuest(INDEX_QUEST_TNL, hero->getScore());
		break;
	}
}

void GameScene::reachNewMap()
{
	int stageUnlocked = REF->getCurrentStageUnlocked();
	if (stageUnlocked == stage) {
		int mapUnlocked = REF->getCurrentMapUnLocked();
		if (mapUnlocked == map) {
			REF->setReachNewMap(true);

			switch (stageUnlocked)
			{
			case 1: case 2:
				if (map < 3) {
					REF->setMapUnlocked(map + 1);
				}
				else {
					REF->increaseStageUnlocked();
					REF->setMapUnlocked(1);
				}

				break;

			case 3:
				if (map < 2) {
					REF->setMapUnlocked(map + 1);
				}
				else {
					REF->increaseStageUnlocked();
					REF->setMapUnlocked(1);
				}
				break;

			case 4:
				if (map < 3) {
					REF->setMapUnlocked(map + 1);
				}
				break;
			default:
				break;
			}
		}
	}
}

void GameScene::danceWithCamera()
{
	follow = Node::create();
	follow->setPosition(SCREEN_SIZE / 2);
	//follow->setAnchorPoint()
	this->addChild(follow, ZORDER_MOON);

	camera = Follow::create(follow);
	camera->setTarget(follow);
	runAction(camera);

	left_corner = CCRectMake(0, 0, SCREEN_SIZE.width / 2, SCREEN_SIZE.height);
}

void GameScene::updateCharacterPoint()
{
	if (previousPercentPosition < 1.0f) {
		auto map_distance = tmx_map->getBoundingBox().size.width;
		auto currentPercent = hero->getPositionX() / map_distance;
		auto deltaPos = (currentPercent - previousPercentPosition) * hud->getDistanceBar()->getBoundingBox().size.width * 0.82f;
		hud->getCharacterPoint()->setPositionX(hud->getCharacterPoint()->getPositionX() + deltaPos);
		previousPercentPosition = currentPercent;
	}
}

void GameScene::initGroundPhysic(b2World * world, Point pos, Size size)
{
	b2Body * body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_FLOOR;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR | BITMASK_COIN | BITMASK_BIRD| BITMASK_WOODER;

	bodyDef.type = b2_staticBody;

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void GameScene::initUnderGroundPhysic(b2World * world, Point pos, Size size)
{
	b2Body * body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_UNDER_GROUND;
	fixtureDef.filter.maskBits = BITMASK_SWORD | BITMASK_UNDER_GROUND;

	bodyDef.type = b2_staticBody;

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}


bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (left_corner.containsPoint(touch->getLocation())) {

		// cannot jump while attacking or being injured
		if (hero->getFSM()->currentState == MAttack || hero->getFSM()->currentState == MInjured ||
			hero->getFSM()->currentState == MDie || hero->getFSM()->currentState == MRevive ||
			hero->getPositionY() + hero->getTrueRadiusOfHero() < 0 || hero->getIsDriverEagle()
			)

			return false;

		jump();

	}

	return false;

}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		pauseGame();
	}
}

void GameScene::updateEnemy()
{
	// update for boss ok.
	auto tmp = (BaseEnemy*)this->getChildByTag(TAG_BOSS);
	if (tmp != nullptr) {
		if (tmp->getIsDie()) {
			this->isWinGame = true;
		}
		else {
			auto boss = (EnemyBoss1*)tmp;
			if (tmp->getB2Body()->GetLinearVelocity().x != 0)
			{
				boss->updateMe(hero);
			}

			else if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.65f) {
				boss->updateMe(hero);

			}
			if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width) {
				hero->updateAttackBossAsTarget(boss);
			}
		}
	}

	if (preLayer != nullptr) {
		auto childLayer1 = preLayer->getChildren();

		for (int i = 0; i < childLayer1.size(); i++) {
			auto agent = childLayer1.at(i);
			// nhung enemy trong man hinh
			if ((agent->getPosition() + preLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2
				&& (agent->getPositionX() + preLayer->getPositionX()) > follow->getPositionX() - SCREEN_SIZE.width * 1 / 2) {


				if (childLayer1.at(i)->getTag() > 100) {
					auto tmp = (BaseEnemy*)childLayer1.at(i);
					// va co body thi moi update
					if (tmp->getB2Body() != nullptr) {
						tmp->updateMe(hero);
					}
				}
			}
		}
	}
	if (posLayer != nullptr) {
		auto childLayer2 = posLayer->getChildren();

		for (int i = 0; i < childLayer2.size(); i++) {
			auto agent = childLayer2.at(i);
			if ((agent->getPosition() + posLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2
				&& (agent->getPositionX() + posLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2) {



				if (childLayer2.at(i)->getTag() > 100) {
					auto tmp = (BaseEnemy*)childLayer2.at(i);
					if (tmp->getB2Body() != nullptr) {
						tmp->updateMe(hero);
					}
				}
			}
		}
	}
}

void GameScene::updateHUD(float dt)
{

	if (hero->getCurrentRunDis() - hero->getPreRunDis() > 10.0f) {
		hero->setScore(hero->getScore() + 15);
		hero->setPreRunDis(hero->getCurrentRunDis());
	}

	if (previousGold < hero->getCoinExplored()) {
		hud->getLbMoney()->setString(StringUtils::format("%i", ++previousGold));
	}

	if (previousScore < hero->getScore()) {
		hud->getLbScore()->setString(StringUtils::format("%i", ++previousScore));
	}

	updateCharacterPoint();
}

void GameScene::updateMultiKills() {
	m_nMultiKills += 1;
	m_fMultiKillsCounterTime = 1.0f;
	if (m_nMultiKills > 1) {
		hud->updateMultiKills(m_nMultiKills);
		if (m_nMultiKills >= 5) m_nMultiKills = 5;
		hero->setScore(hero->getScore() + (m_nMultiKills - 1) * m_lastScore);
	}
}

void GameScene::runnerItem(Item_type type, int counter)
{
	switch (type)
	{
	case MAGNET:
		hud->runnerItemMagnet(counter);
		hero->setItemValue(KEY_ITEM_MAGNET, counter);
		hero->getSuctionCoin()->setVisible(true);
		break;
	case DOUBLE_COIN:
		hud->runnerItemDC(counter);
		hero->setItemValue(KEY_ITEM_DOUPLE_COIN, counter);
		break;
	}

}

void GameScene::updateBloodBar(int numberOfHealth, bool isVisible)
{
	if (numberOfHealth >= 0) {
		if (isVisible) {
			auto blood = (Sprite*)hud->getListBlood()->getObjectAtIndex(numberOfHealth);
			blood->setVisible(true);
		}
		else {
			for (int i = numberOfHealth; i < hero->getMaxHealth(); i++) {
				auto blood = (Sprite*)hud->getListBlood()->getObjectAtIndex(i);
				blood->setVisible(false);
			}
		}
	}
}

void GameScene::updateCamera()
{
	if (_aEagle != nullptr) {
		if (_aEagle->getIsUp() && _aEagle->getPositionZ() < 100.0f) {
			_aEagle->setPositionZ(_aEagle->getPositionZ() + 5.0f);
			_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() - 0.04f);
		}
		if (_aEagle->getIsDown() && _aEagle->getPositionZ() > 0.0f) {
			_aEagle->setPositionZ(_aEagle->getPositionZ() - 5.0f);
			_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() + 0.04f);
		}
		if (!_aEagle->getIsCarry()) {

			if (hero->getB2Body()) {
				if (hero->getPositionY() > background->getPositionY() + SCREEN_SIZE.height / 6 && hero->getB2Body()->GetLinearVelocity().y > 0) {
					background->up(hero->getPosition());
				}
				else if (hero->getPositionY() < background->getPositionY() - SCREEN_SIZE.height / 6 && hero->getB2Body()->GetLinearVelocity().y < 0) {
					background->down(hero->getPosition());
				}
			}

		}
	}
	else {
		if (hero->getPositionY() > SCREEN_SIZE.height * 0.5f) {
			background->setPositionY(hero->getPositionY());
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}

	}
	if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
		if (hero->getPositionX() > (tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width) && !haveboss)
			this->isWinGame = true;
		if (!isWinGame) {
			follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
		}
		else {
			if (hero->getPositionX() > follow->getPositionX() + SCREEN_SIZE.width / 1.8f) {
				this->winGame();
			}
		}
	}
	background->updatePosition();
	background2->updatePosition();
	follow->setPositionY(background->getPositionY());

	if (hero->getIsDriverEagle()) {
		float _fTempCheck = _aEagle->getPositionY() - SCREEN_SIZE.height * 0.5f;
		if (_fTempCheck > 0) {
			_fTempCheck = hero->getPositionY();
			_fTempCheck -= SCREEN_SIZE.height * 0.3f * (1 - _aEagle->getPositionZ() / 100.0f);
			if (_fTempCheck < SCREEN_SIZE.height * 0.5f) {
				_fTempCheck = SCREEN_SIZE.height * 0.5f;
			}
		}
		else {
			_fTempCheck = SCREEN_SIZE.height * 0.5f;
		}

		background->setPositionY(_fTempCheck);
		follow->setPositionY(background->getPositionY());
	}
}

void GameScene::updateChangeBg()
{
	if (changebg > 0) {
		if (hero->getPositionX() > changebg) {
			//log("changbgggg");
			changebg = 0;
			auto call1 = CallFunc::create([&]() {
				background->removeChildByTag(21);
			});
			auto call2 = CallFunc::create([&]() {
				background->removeChildByTag(22);
			});
			background->getChildByTag(21)->runAction(Sequence::createWithTwoActions(FadeTo::create(5.0f, 0), call1));
			background->getChildByTag(22)->runAction(Sequence::createWithTwoActions(FadeTo::create(5.0f, 0), call2));
			/*background->getChildByTag(21)->runAction(FadeTo::create(5.0f,0));
			background->getChildByTag(22)->runAction(FadeTo::create(5.0f, 0));*/
		}
	}
}

void GameScene::updateCoin()
{
	auto children = this->getChildren();
	for (auto child : children) {

		if (child->getTag() == TAG_COIN) {
			//log("vi tri cua vang: %f, %f", child->getPositionX(), child->getPositionY());
			if (child->getPositionX() < follow->getPositionX() + SCREEN_SIZE.width / 2 && child->getPositionX() > follow->getPositionX() - SCREEN_SIZE.width / 2) {

				((Coin*)child)->updateMe(hero);
			}
			else if (child->getPositionX() < follow->getPositionX() - SCREEN_SIZE.width / 2) {
				child->removeFromParentAndCleanup(true);
			}
		}
		else if (child->getTag() == TAG_COINBAG || child->getTag() == TAG_COINBULLION) {
			if (child->getPositionX() < follow->getPositionX() + SCREEN_SIZE.width / 2 && child->getPositionX() > follow->getPositionX() - SCREEN_SIZE.width / 2) {
				((B2Skeleton*)child)->updateMe(hero);
			}
			else if (child->getPositionX() < follow->getPositionX() - SCREEN_SIZE.width / 2) {
				child->removeFromParentAndCleanup(true);
			}
		}

	}
}


void GameScene::shakeTheScreen()
{
	auto shake = MoveBy::create(0.01f, Vec2(0, -0.005f * SCREEN_SIZE.height));
	this->runAction(Sequence::create(shake, shake->reverse(), shake, shake->reverse(), nullptr));
}

void GameScene::reviveHero()
{
	AudioManager::playSound(SOUND_MCREVIVE);
	hud->refreshControl();
	resumeGame();

	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hero->setDieHard(1);
	hero->setIsPriorInjured(false);
	hero->getBloodScreen()->setVisible(false);
	hero->setHealth(hero->getMaxHealth());
	for (int i = 0; i < hero->getHealth(); i++) {
		auto health = (Sprite*)hud->getListBlood()->getObjectAtIndex(i);
		health->setVisible(true);
	}

	hero->doDestroyBodies(world);

	auto killAll = CallFunc::create([&]() {
		hero->killThemAll();
	});

	auto reviveUp = MoveBy::create(1.43f, Vec2(0, hero->getTrueRadiusOfHero() * 2.7f));
	hero->runAction(Sequence::create(reviveUp, killAll, nullptr));
	hero->getFSM()->changeState(MRevive);
}

void GameScene::callingBird()
{
	AudioManager::playSound(SOUND_BIRD);
	REF->setUpNumberQuest(INDEX_QUEST_CALL_BIRD, 1);
	if (hero->getActiveSkill()->isVisible())
		hero->getActiveSkill()->setVisible(false);

	hero->setIsNoDie(true);
	hero->setIsDriverEagle(true);
	hero->setOnGround(false);
	hero->getFSM()->changeState(MIdle);
	hero->setVisible(false);
	hud->setBtnCallingHintDone(false);
	_aEagle->setVisible(true);
	if (_aEagle->getB2Body() == nullptr || hero->getB2Body() == nullptr) {
		log("Crazy fox");
	}

	// DQ increase 30% move
	_aEagle->getB2Body()->SetTransform(hero->getB2Body()->GetPosition(), 0.0f);
	_aEagle->flyUp(b2Vec2((charId == 0) ? hero->getMoveVel() * 1.3f : hero->getMoveVel(), 10.0f));

	auto _aEagleFlyDown = Sequence::create(DelayTime::create(4.0f), CallFunc::create([&]() {
		_aEagle->flyDown(b2Vec2((charId == 0) ? hero->getMoveVel() * 1.3f : hero->getMoveVel(), -10.0f));
	}), nullptr);
	//auto _aFinishFly = Sequence::create(DelayTime::create(6.0f), CallFunc::create([&]() {
	//	//heroGetOffEagle();
	//	//_aEagle->changeBodyMaskBits(BITMASK_FLOOR);
	//}), nullptr);
	_aEagle->runAction(_aEagleFlyDown);
	//_aEagle->runAction(_aFinishFly);

	auto _aDropHeroAnyWay = Sequence::create(DelayTime::create(7.0f), CallFunc::create([&]() { // chinh thoi gian o day
		if (hero->getIsDriverEagle()) {
			heroGetOffEagle();
		}
	}), nullptr);
	_aEagle->runAction(_aDropHeroAnyWay);
}

void GameScene::blurScreen()
{
	blur->setVisible(true);
}

void GameScene::pauseGame()
{
	blurScreen();

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogPause::create();
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::dieGame()
{
	if (REF->getIsLockedHero()) {	// if hero is locked
		overGame();
		return;
	}

	blurScreen();
	/*if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}*/

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	//hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

#ifdef SDKBOX_ENABLED
	sdkbox::PluginVungle::setListener(this);
#endif
	dialogPause = DialogRevive::create(++numberRevive);
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::overGame()
{
	AudioManager::stopSoundandMusic();
	AudioManager::playSound(SOUND_FAIL);
	if (!blur->isVisible())
		blurScreen();

	//hud->getLbScore()->setString(StringUtils::format("%i", hero->getScore()));
	//hud->getLbMoney()->setString(StringUtils::format("%i", hero->getCoinExplored()));

	/*if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}*/

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogOverGame::create(hero->getScore(), hero->getCoinExplored());
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	updateQuest();
	this->pause();
}

void GameScene::nextGame()
{
	this->removeAllChildrenWithCleanup(true);
	Layer *_pMenuScene = MenuLayer::create(true);
	auto _aMainMenuScene = Scene::create();
	_aMainMenuScene->addChild(_pMenuScene);
	Director::getInstance()->replaceScene(_aMainMenuScene);
}

void GameScene::winGame()
{
	AudioManager::stopSoundandMusic();
	AudioManager::playSound(SOUND_WIN);

	//reachNewMap();

	blurScreen();
	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	//hud->getLbScore()->setString(StringUtils::format("%i", hero->getScore()));
	//hud->getLbMoney()->setString(StringUtils::format("%i", hero->getCoinExplored()));

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogStageClear::create(hero->getScore(), hero->getCoinExplored());
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);
	updateQuest();
	this->pause();
}

void GameScene::resumeGame()
{
	blur->setVisible(false);

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->resume();

	hero->resume();
	if (hero->getIsDriverEagle())
		_aEagle->resume();

	dialogPause->removeFromParentAndCleanup(true);
	dialogPause = nullptr;

	// not in revive
	if (!hero->getNoActive())
		hud->resumeIfVisible();
	hud->getPauseItem()->setEnabled(true);

	if (!hero->getIsDoneDuration1() || !hero->getIsDoneDuration2() || !hero->getIsDoneDuration3()) {

	}
	else
		enableCalling();

	this->resume();
}

void GameScene::restartGame()
{
	this->removeAllChildrenWithCleanup(true);
	Director::getInstance()->replaceScene(LoadingLayer::createScene(stage, map, charId));
}

void GameScene::loadPosAndTag()
{
	loadPosOfObjectInGroup("wooder", TAG_ENEMY_WOODER);
	loadPosOfObjectInGroup("toanchan_student", TAG_ENEMY_TOANCHAN1);
	loadPosOfObjectInGroup("toanchan_student2", TAG_ENEMY_TOANCHAN2);

	loadPosOfObjectInGroup("coin_bullion", TAG_COINBULLION);
	loadPosOfObjectInGroup("coin_bag", TAG_COINBAG);

	loadPosOfObjectInGroup("lms_student lv1", TAG_ENEMY_HONGLANGBA1);
	loadPosOfObjectInGroup("lms_student lv2", TAG_ENEMY_HONGLANGBA2);
	loadPosOfObjectInGroup("bee", TAG_ENEMY_TOONG);
	loadPosOfObjectInGroup("tnb", TAG_ENEMY_TNB);
	loadPosOfObjectInGroup("hoacdo_1", TAG_ENEMY_HOACDO1);
	loadPosOfObjectInGroup("hoacdo_2", TAG_ENEMY_HOACDO2);
	loadPosOfObjectInGroup("datnhiba_1", TAG_ENEMY_DATNHIBA1);
	loadPosOfObjectInGroup("datnhiba_2", TAG_ENEMY_DATNHIBA2);
	loadPosOfObjectInGroup("chonggo", TAG_ENEMY_CHONG1);
	loadPosOfObjectInGroup("chonggo2", TAG_ENEMY_CHONG2);
	loadPosOfObjectInGroup("chonggo3", TAG_ENEMY_CHONG3);
	loadPosOfObjectInGroup("linhcamgiao1", TAG_ENEMY_LINHCAMGIAO1);
	loadPosOfObjectInGroup("linhcamgiao2", TAG_ENEMY_LINHCAMGIAO2);
	loadPosOfObjectInGroup("linhtenthang", TAG_ENEMY_LINH_TEN_THANG);
	loadPosOfObjectInGroup("linhtenxien", TAG_ENEMY_LINH_TEN_XIEN);
	loadPosOfObjectInGroup("linhcamroi", TAG_ENEMY_LINHCAMROI);

}

void GameScene::loadPosOfObjectInGroup(string nameOfGroup, float tag)
{
	auto group = tmx_map->getObjectGroup(nameOfGroup);
	if (!group) return;
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		MyData data(tag, origin.x, origin.y);
		listPosAndTag.push_back(data);
	}
}

void GameScene::initLayerToAddAgent()
{
	preLayer = MyLayer::create(tmx_map);
	posLayer = MyLayer::create(tmx_map);
	preLayer->setPosition(Vec2::ZERO);
	preLayer->setAnchorPoint(Point(0, 0));
	posLayer->setPosition(SCREEN_SIZE.width*1.0f, 0);
	posLayer->setAnchorPoint(Point(0, 0));
	this->addChild(preLayer, ZORDER_ENEMY);
	this->addChild(posLayer, ZORDER_ENEMY);
	createAgentOnLayer(preLayer);
	createAgentOnLayer(posLayer);

}

void GameScene::updateLayer()
{
	if (posLayer != nullptr) {
		if (posLayer->getPositionX() <= follow->getPositionX() - SCREEN_SIZE.width / 2) {
			//preLayer->removeAllChildrenWithCleanup(false);
			preLayer->refresh();
			MyLayer *tmpLayer = preLayer;
			preLayer = posLayer;
			posLayer = tmpLayer;

			posLayer->setPosition(preLayer->getPositionX() + SCREEN_SIZE.width, 0);
			if (posLayer->getPositionX() < tmx_map->getBoundingBox().size.width) {
				auto call = CCCallFunc::create([&]() {
					createAgentOnLayer(posLayer);
				});
				this->runAction(call);
			}
			else {
				posLayer->removeFromParentAndCleanup(true);
				posLayer = nullptr;
			}
		}
	}
}

void GameScene::createAgentOnLayer(MyLayer * layer)
{
	for (auto i : listPosAndTag) {
		//log("i.x = %f", i.x);
		if (i.x >= layer->getPositionX() && i.x < layer->getPositionX() + SCREEN_SIZE.width) {
			creatAgentByMydata(layer, i);
		}
	}
}

void GameScene::creatAgentByMydata(MyLayer * layer, MyData data)
{
	switch (data.tag)
	{
	case TAG_ENEMY_WOODER: {
		//ok
		creatEnemyWooder(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		//log("wooder: %f, %f", layer->getPositionX(), layer->getPositionY());
		break;

	}
	case TAG_ENEMY_TOANCHAN1: {
		createEnemyToanChanStudent(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_TOANCHAN2: {
		createEnemyToanChanStudent2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}

	case TAG_ENEMY_TNB: {
		createEnemyTNB(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_HONGLANGBA1: {
		createEnemyHongLangBa(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_HONGLANGBA2: {
		createEnemyHongLangBa2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_TOONG: {
		createEnemyToOng(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}

	case TAG_ENEMY_HOACDO1: {
		createEnemyHoacDo(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_HOACDO2: {
		createEnemyHoacDo2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_DATNHIBA1: {
		createEnemyDatNhiBa(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_DATNHIBA2: {
		createEnemyDatNhiBa2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_CHONG1: {
		createEnemyChong1(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_CHONG2: {
		createEnemyChong2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_CHONG3: {
		createEnemyChong3(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_LINHCAMGIAO1: {
		createEnemyLinhCamGiao(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}

	case TAG_ENEMY_LINHCAMGIAO2: {
		createEnemyLinhCamGiao2(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_LINH_TEN_XIEN: {
		createEnemyLinhTenXien(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_LINH_TEN_THANG: {
		createEnemyLinhTenThang(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_LINHCAMROI: {
		createEnemyLinhCamRoi(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}

	}
}

void GameScene::jump()
{
	if (hero->getNumberOfJump() > 0) {
		hero->setNumberOfJump(hero->getNumberOfJump() - 1);
		auto currentVelX = hero->getB2Body()->GetLinearVelocity().x;
		hero->getB2Body()->SetLinearVelocity(b2Vec2(currentVelX, hero->getJumpVel()));

		if (hero->getFSM()->currentState == MLand && hero->getNumberOfJump() > 0) {
			hero->getSmokeJumpX2()->setPosition(hero->getPosition());
			hero->getSmokeJumpX2()->setVisible(true);
			hero->smokeJumpX2Ani();
		}


		if (hero->getNumberOfJump() == 1) {
			hero->getFSM()->changeState(MJump);
			hero->setOnGround(false);
		}
		if (hero->getNumberOfJump() == 0)
			hero->getFSM()->changeState(MDoubleJump);
	}
}

void GameScene::introJump(int type)
{
	blurScreen();
	tut = TutorialJump::create(type);
	this->getParent()->addChild(tut);

	hero->getSmokeRun()->pause();
	hero->pause();
	hud->getPauseItem()->setEnabled(false);
	if (posXJump1Tut > 0)
		posXJump1Tut = -1;
	else
		posXJump2Tut = -1;
	this->pause();
}

void GameScene::introAttack()
{
	tut = TutorialAttack::create();
	this->getParent()->addChild(tut);

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();
	hero->pause();

	hud->getPauseItem()->setEnabled(false);
	hud->introAttack();

	posXAttackTut = -1;
	this->pause();
}

void GameScene::introSkills()
{
	tut = TutorialSkill::create();
	this->getParent()->addChild(tut);

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();
	hero->pause();

	hud->getPauseItem()->setEnabled(false);
	hud->introSkills();

	posXSkillTut = -1;
	this->pause();
}

void GameScene::introBird()
{
	tut = TutorialIntroBird::create();
	this->getParent()->addChild(tut);

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();
	hero->pause();

	hud->getPauseItem()->setEnabled(false);
	hud->introBird();

	if (!hero->getIsDoneDuration1() || !hero->getIsDoneDuration2() || !hero->getIsDoneDuration3()) {

	}
	else
		enableCalling();

	posXIntroBird = -1;
	this->pause();
}

void GameScene::tutorial()
{
	if (posXJump1Tut > 0) {	// no need boolean
		if (hero->getPositionX() >= posXJump1Tut) {
			introJump(1);
		}
	}

	if (posXJump2Tut > 0) {	// no need boolean
		if (hero->getPositionX() >= posXJump2Tut) {
			introJump(2);
		}
	}

	if (posXAttackTut > 0) {
		if (hero->getPositionX() >= posXAttackTut) {
			introAttack();
		}
	}

	if (posXSkillTut > 0) {
		if (hero->getPositionX() >= posXSkillTut) {
			introSkills();
		}
	}

	if (posXIntroBird > 0) {
		if (hero->getPositionX() >= posXIntroBird) {
			introBird();
		}
	}
}

void GameScene::resumeAfterTut(int caseTut)
{
	if (blur->isVisible())
		blur->setVisible(false);
	hud->disableBlur();
	hero->getSmokeRun()->resume();

	hero->resume();

	tut->removeFromParentAndCleanup(true);
	tut = nullptr;


	hud->getPauseItem()->setEnabled(true);


	switch (caseTut)
	{
	case 1:
		if (posXJump2Tut > 0) {
			touch_listener = EventListenerTouchOneByOne::create();
			touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
		}

		this->resume();
		break;

	case 2:
		if (hud->getCoverSkill()->isVisible()) {
			hud->getCoverSkill()->resume();
		}

		if (hud->getCoverItemMagNet()->isVisible()) {
			hud->getCoverItemMagNet()->resume();
		}

		if (hud->getCoverItemDC()->isVisible()) {
			hud->getCoverItemDC()->resume();
		}

		this->resume();
		break;

	case 4:
		hud->resumeIfVisible();
		isFirstPlay = false;
		REF->setDoneFirstPlay();
		this->resume();
		break;
	default:
		break;
	}
}
#ifdef SDKBOX_ENABLED

void GameScene::onVungleAdViewed(bool isComplete)
{
	
}
void GameScene::onVungleCacheAvailable()
{
}
void GameScene::onVungleStarted()
{
}
void GameScene::onVungleFinished()
{
}
void GameScene::onVungleAdReward(const std::string & name)
{
	this->reviveHero();
}
#endif 