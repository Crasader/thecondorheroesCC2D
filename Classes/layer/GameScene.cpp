#include "layer/GameScene.h"
#include "SimpleAudioEngine.h"
#include "layer/MenuScene.h"
#include "layer/SelectStageScene.h"
#include "layer/Hud.h"
#include "layer/LoadingLayer.h"

#include "manager/RefManager.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


Hud *hud;
LoadingLayer* loadingLayer;


Scene* GameScene::createScene(int stage, int map, int haveboss, int charId)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object

	loadingLayer = LoadingLayer::create();
	auto layer = GameScene::create(stage, map, haveboss, charId);
	layer->setAnchorPoint(Point(0, 0));
	layer->setName("gameLayer");

	hud = Hud::create();
	hud->setPosition(Director::getInstance()->getVisibleOrigin());

	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);
	scene->addChild(layer->blur);
	scene->addChild(loadingLayer);


	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init(int stage, int map, int haveboss, int charId)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	AudioManager::stopMusic();
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
	case 4:{
		AudioManager::playMusic(MUSIC_STAGE4);
		break;
	}
	
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	isModeDebug = false;

	indexOfNextMapBoss = -1;
	this->stage = stage;
	this->haveboss = haveboss;
	this->charId = charId;
	this->map = map;

	this->isWinGame = false;
	
	initB2World();
	// cache batchnode
	//auto tmp = Sprite::createWithSpriteFrameName("coin_01.png");
	batchNode = SpriteBatchNode::create("coin_01.png");
	this->addChild(batchNode);
	// tesst

	loadBackground();
	danceWithCamera();
	//cacheSkeleton();
	if (charId == 0) {
		createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas",
			Point(origin.x, visibleSize.height * 0.75f));
	}
	else {
		createCoLong("Animation/CoLong/CoLong.json", "Animation/CoLong/CoLong.atlas",
			Point(origin.x, visibleSize.height * 0.75f));
	}

	

	

	loadPosAndTag();
	createGroundBody();
	createItem();
	initLayerToAddAgent();


	createEagle(Point(hero->getB2Body()->GetPosition().x - visibleSize.width, visibleSize.height / 2));

	if (haveboss)
		creatBoss();


	createCoin();
	return true;
}

GameScene * GameScene::create(int stage, int map, int haveboss, int charId)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(stage, map, haveboss, charId))
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


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());
	hero->addStuff();
	hero->createPool();

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}

void GameScene::createCoLong(string path_Json, string path_Atlas, Point position)
{
	hero = CoLong::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);

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
	hud->addEvents();
	if (hud->getBtnCalling() != nullptr)
		hud->getBtnCalling()->setEnabled(true);

	if (hud->getBtnMagnet() != nullptr)
		hud->getBtnMagnet()->setEnabled(true);

	if (hud->getBtnDoubleGold() != nullptr)
		hud->getBtnDoubleGold()->setEnabled(true);

	hud->getPauseItem()->setEnabled(true);

	touch_listener = EventListenerTouchOneByOne::create();
	key_listener = EventListenerKeyboard::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	key_listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
	this->scheduleUpdate();
}

void GameScene::checkActiveButton()
{
	if (hero->getIsDriverEagle() ||
		hero->getPositionY() + hero->getTrueRadiusOfHero() * 2 < 0) {

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
			if (!hud->getBtnCalling()->isEnabled())
				hud->getBtnCalling()->setEnabled(true);
		}

		return;
	}

	if (hud->getBtnSkill_1()->getIsBlocked()) {		// 2 and 3 active
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				if(hero->getFSM()->currentState != MRevive)
					hero->killThemAll(listEnemyOccurInScreen);
				hero->getActiveSkill()->setVisible(false);

				if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
					if(!hud->getBtnCalling()->isEnabled())
						hud->getBtnCalling()->setEnabled(true);
				}

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
				if (hero->getFSM()->currentState != MRevive)
					hero->killThemAll(listEnemyOccurInScreen);
				hero->getActiveSkill()->setVisible(false);

				if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
					if (!hud->getBtnCalling()->isEnabled())
						hud->getBtnCalling()->setEnabled(true);
				}

				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible() &&
					!hud->getBtnSkill_2()->getMain()->isVisible())
					hud->getBtnSkill_2()->getMain()->setVisible(true);

				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_3()->getIsBlocked()) {		// 2 and 1 active
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
				if (hero->getFSM()->currentState != MRevive)
					hero->killThemAll(listEnemyOccurInScreen);
				hero->getActiveSkill()->setVisible(false);

				if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
					if (!hud->getBtnCalling()->isEnabled())
						hud->getBtnCalling()->setEnabled(true);
				}

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

	if (hud->getBtnAttack()->getIsActive() && !hud->getBtnAttack()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		hero->getFSM()->changeState(MAttack);


		hud->getBtnAttack()->setIsActive(false);
	}

	if (hud->getBtnSkill_1()->getIsActive() && !hud->getBtnSkill_1()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

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

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
			hud->getBtnCalling()->setEnabled(false);
		}
	}

	if (hud->getBtnSkill_2()->getIsActive() && !hud->getBtnSkill_2()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 2;

		hero->setIsDoneDuration2(false);
		hero->doCounterSkill2();

		hud->runnerSkillDuration(2, hero->getDurationSkill2());

		hud->getBtnSkill_1()->setIsBlocked(true);
		if(hud->getBtnSkill_1()->getCanTouch())
			hud->getBtnSkill_1()->getMain()->setVisible(false);

		hud->getBtnSkill_3()->setIsBlocked(true);
		if (hud->getBtnSkill_3()->getCanTouch())
			hud->getBtnSkill_3()->getMain()->setVisible(false);


		hud->getBtnSkill_2()->setIsActive(false);
		hero->getActiveSkill()->setVisible(true);

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
			hud->getBtnCalling()->setEnabled(false);
		}
	}

	if (hud->getBtnSkill_3()->getIsActive() && !hud->getBtnSkill_3()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

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

		if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
			hud->getBtnCalling()->setEnabled(false);
		}
	}

}

void GameScene::heroGetOffEagle() {
	hud->showButton();
	hero->changeBodyCategoryBits(BITMASK_HERO);
	_aEagle->changeBodyCategoryBits(0);
	_aEagle->changeBodyMaskBits(0);
	hero->setIsDriverEagle(false);
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

	if (m_fMultiKillsCounterTime > 0.0f) {
		m_fMultiKillsCounterTime -= dt;
	}
	else {
		m_nMultiKills = 0;
	}

	checkActiveButton();

	_aEagle->updateMe(dt);
	/*if (_aEagle->getIsAbleToDropHero() == true) {
		heroGetOffEagle();
		_aEagle->setIsAbleToDropHero(false);
	}*/

	if (hero->getIsDriverEagle()) {
		if (hero->getHealth() <= 0) {	// die
			hero->getB2Body()->SetGravityScale(1);
			hero->getB2Body()->SetLinearVelocity(b2Vec2(0, -hero->getJumpVel()));
			hero->setVisible(true);
			_aEagle->stopAllActions();
			_aEagle->flyAway();
		}
		else {
			hero->getB2Body()->SetTransform(_aEagle->getB2Body()->GetPosition(), 0.0f);
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
			if(item->getB2Body() == nullptr) continue;
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
	

	// fall down some hold
	if (hero->getPositionY() + hero->getTrueRadiusOfHero() * 3 < 0 &&
		hud->getBtnSkill_1()->isVisible()) {
		if (hud->getBtnCalling()->isVisible()) {
			hud->hideButton();
			hud->moveCallBirdToCenterScreen(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
			hero->setOnGround(false);
			hero->setVisible(false);
			hero->getB2Body()->SetGravityScale(0);
			hero->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		}
		else {
			overGame();
		}
	}

	if (hud->getBtnCallingHintDone()) {
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

	updateHUD(dt);
}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 10.0f / 3.0f / PTM_RATIO));

	// draw debug
	auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	if (isModeDebug)
		world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;

	debugDraw->SetFlags(flags);

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

	this->addChild(tmx_map, ZORDER_TMX);
	if (haveboss) {
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

	createInfiniteNode();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::create(StringUtils::format("Map/map%d/bg%d_1.png", stage, map));
	bg1_1->setScale(SCREEN_SIZE.width / (bg1_1->getContentSize().width));
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::create(StringUtils::format("Map/map%d/bg%d_1.png", stage, map));
	bg1_2->setScale(SCREEN_SIZE.width / (bg1_2->getContentSize().width));
	bg1_2->setAnchorPoint(Point(0, 0.5f));

	auto moonGr = tmx_map->getObjectGroup("moon");
	if (moonGr) {
		auto object = moonGr->getObject("moon");
		auto pos = Point(object["x"].asFloat()*tmx_map->getScale(), object["y"].asFloat()*tmx_map->getScale());
		auto moon = Sprite::create("moon.png");
		moon->setScale(SCREEN_SIZE.height / 4 / moon->getContentSize().height);
		background->addChild(moon, 2, Vec2(0, 1),Vec2(pos.x,pos.y-SCREEN_SIZE.height/2));
	}

	auto bg2_1 = Sprite::create(StringUtils::format("Map/map%d/bg%d_2.png", stage, map));
	//auto bg2_1 = Sprite::create("moon.png");
	bg2_1->setScale(SCREEN_SIZE.width / (bg2_1->getContentSize().width));
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::create(StringUtils::format("Map/map%d/bg%d_2.png", stage, map));
	//auto bg2_2 = Sprite::create("moon.png");
	bg2_2->setScale(SCREEN_SIZE.width / (bg2_2->getContentSize().width));
	bg2_2->setAnchorPoint(Point(0, 0.5f));

	
	background->addChild(bg1_1, 1, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 1, Vec2(0.5f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));

	background->addChild(bg2_1, 3, Vec2(0.7f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 3, Vec2(0.7f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));

	//background->setPosition(Point(-SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	background->setPosition(Point(0, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	if (isModeDebug)
		background->setVisible(false);
	this->addChild(background, ZORDER_BG);

	background2 = InfiniteParallaxNode::create();


	if (stage == 1) {
		auto bg3_1 = Sprite::createWithSpriteFrameName("bg3.png");
		bg3_1->setScaleX(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
		bg3_1->setScaleY(bg3_1->getScaleX()*0.7f);
		//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
		bg3_1->setAnchorPoint(Point(0, 0));

		auto bg3_2 = Sprite::createWithSpriteFrameName("bg3.png");
		bg3_2->setScale(SCREEN_SIZE.width / (bg3_1->getContentSize().width));
		bg3_2->setScaleY(bg3_2->getScaleX()*0.7f);
		//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
		bg3_2->setAnchorPoint(Point(0, 0));

		background2->addChild(bg3_1, 0, Vec2(0.8f, 1), Vec2(0, 0));
		background2->addChild(bg3_2, 0, Vec2(0.8f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	}

	background2->setPosition(Point(0, 0));
	background2->setAnchorPoint(Point(0, 0.0f));
	this->addChild(background2, ZORDER_BG);
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

	auto groupUnderGround = tmx_mapboss[0]->getObjectGroup("under_ground");
	if (!groupGround) return;
	for (auto child : groupUnderGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap) + tmx_mapboss[0]->getPosition();
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initUnderGroundPhysic(world, origin, Size(INT_MAX / 4, sizeOfBound.height));
	}
}

void GameScene::creatEnemyWooder(MyLayer * layer, Vec2 pos)
{
	auto enemy = (EnemyWooder*)layer->wooderPool->getObject();
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	enemy->resumeSchedulerAndActions();
	//layer->addChild(enemy, ZORDER_ENEMY);
	if (enemy->getB2Body()) {
		world->DestroyBody(enemy->getB2Body());
	}
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2.5f));
	enemy->changeBodyCategoryBits(BITMASK_WOODER);
	enemy->changeBodyMaskBits(BITMASK_SWORD);

	enemy->listener();
	//log("wooder is die: %i", enemy->getIsDie());
}

void GameScene::createEnemyToanChanStudent(MyLayer * layer, Vec2 pos)
{
	auto enemy = (EnemyToanChanStudent*)layer->toanchan1Pool->getObject();
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	enemy->resumeSchedulerAndActions();
	//layer->addChild(enemy, ZORDER_ENEMY);
	if (enemy->getB2Body()) {
		world->DestroyBody(enemy->getB2Body());
	}
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 4));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN1);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);

	enemy->listener();
}

void GameScene::createEnemyToanChanStudent2(MyLayer * layer, Vec2 pos)
{
	auto enemy = (EnemyToanChanStudent2*)layer->toanchan2Pool->getObject();
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	enemy->resumeSchedulerAndActions();
	//layer->addChild(enemy, ZORDER_ENEMY);
	if (enemy->getB2Body()) {
		world->DestroyBody(enemy->getB2Body());
	}
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN2);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);

	enemy->listener();
}

//void GameScene::createEnemyTNB(Layer * layer, Vec2 pos) {
//	auto scaleOfWooder = (SCREEN_SIZE.height / 3.5f) / 300;
//	auto enemy = (EnemyTNB*)tnbPool->getObject();
//	enemy->setPosition(pos);
//	enemy->setVisible(true);
//	layer->addChild(enemy, ZORDER_ENEMY);
//	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2.5f));
//	enemy->changeBodyCategoryBits(BITMASK_TNB);
//	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
//
//	enemy->listener();
//}

void GameScene::createEnemyHongLangBa(Layer * layer, Vec2 pos) {
	auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401;
	auto enemy = EnemyHongLangBa::create("Animation/Enemy_HongLangBa/HLBa1", scaleOfEnemy);
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN1);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	//enemy->genSplash();
	enemy->listener();
}

void GameScene::createEnemyHongLangBa2(Layer * layer, Vec2 pos) {
	auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401;
	auto enemy = EnemyHongLangBa2::create("Animation/Enemy_HongLangBa2/HLBa2", scaleOfEnemy);
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 2));
	enemy->changeBodyCategoryBits(BITMASK_TOANCHAN2);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	enemy->genDarts();
	enemy->listener();
}

void GameScene::createEnemyToOng(Layer * layer, Vec2 pos) {
	auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401;
	auto enemy = EnemyToOng::create("Animation/Enemy_ToOng/toong", scaleOfEnemy);
	enemy->setIsDie(false);
	enemy->setPosition(pos);
	enemy->setVisible(true);
	layer->addChild(enemy, ZORDER_ENEMY);
	enemy->initCirclePhysic(world, Point(pos.x + layer->getPositionX(), pos.y + layer->getPositionY() + enemy->getBoundingBox().size.height / 4));
	enemy->changeBodyCategoryBits(BITMASK_TOONG);
	enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	//enemy->genSplash();
	enemy->listener();
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
		auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);
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
			coin->pauseSchedulerAndActions();
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

void GameScene::danceWithCamera()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	follow = Node::create();
	follow->setPosition(/*origin +*/ SCREEN_SIZE / 2);
	//follow->setAnchorPoint()
	this->addChild(follow,ZORDER_MOON);

	camera = Follow::create(follow);
	camera->setTarget(follow);
	runAction(camera);

	blur = LayerColor::create(Color4B(0, 0, 0, 170));
	blur->setVisible(false);
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

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_FLOOR;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR | BITMASK_COIN | BITMASK_BIRD;

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
			hero->getPositionY() + hero->getTrueRadiusOfHero() < 0
			)

			return false;

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
	auto child = this->getChildren();
	for (int i = 0; i < child.size(); i++) {
		if (child.at(i)->getTag() > 100) {
			auto tmp = (BaseEnemy*)child.at(i);
			if (tmp->getTag() == TAG_BOSS) {
				if (tmp->getIsDie()) {
					this->isWinGame = true;
				}
				else {
					auto boss = (EnemyBoss1*)tmp;
					if (tmp->getB2Body()->GetLinearVelocity().x != 0)
					{
						boss->updateMe(hero);
					}

					else if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.75f) {
						boss->updateMe(hero);

					}
					if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width) {
						hero->updateAttackBossAsTarget(boss);
					}
				}
			}
		}
	}
	if (preLayer != nullptr) {
		auto childLayer1 = preLayer->getChildren();

		for (int i = 0; i < childLayer1.size(); i++) {
			auto agent = childLayer1.at(i);

			if (agent->getTag() >= 90) {
				if ((agent->getPosition() + preLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2
					&& (agent->getPositionX() + preLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2) {
					auto tmp = (B2Skeleton*)(agent);
					tmp->updateMe(hero);
				}
			}
			else if (agent->getTag() >= 80 && agent->getTag() < 90) {
				if ((agent->getPosition() + preLayer->getPosition()).x < follow->getPositionX() + SCREEN_SIZE.width * 1 / 2.5f
					&& (agent->getPositionX() + preLayer->getPositionX())>follow->getPositionX() - SCREEN_SIZE.width * 1 / 2.5f) {
					auto tmp = (B2Sprite*)(agent);
					tmp->updateMe(hero);
				}
			}
			if (childLayer1.at(i)->getTag() > 100) {
				auto tmp = (BaseEnemy*)childLayer1.at(i);
				if (tmp->getB2Body() != nullptr) {
					if (tmp->getIsDie()) {
						if (tmp->getIsOccur()) {
							listEnemyOccurInScreen.remove(tmp);
						}
					}
					else {
						if (tmp->getPositionX() + preLayer->getPosition().x < follow->getPositionX() - SCREEN_SIZE.width * 0.7f) {
							listEnemyOccurInScreen.remove(tmp);
						}

						else if (tmp->getPositionX() + preLayer->getPosition().x < follow->getPositionX() + SCREEN_SIZE.width &&
							tmp->getPositionX() + preLayer->getPosition().x > follow->getPositionX() - SCREEN_SIZE.width / 2) {
							if (!tmp->getIsOccur() &&
								(tmp->getPositionX() + preLayer->getPosition().x < follow->getPositionX() + SCREEN_SIZE.width * 0.52f &&
									tmp->getPositionX() + preLayer->getPosition().x > follow->getPositionX() - SCREEN_SIZE.width / 2)) {
								listEnemyOccurInScreen.push_back(tmp);
							}
						}
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

				if (agent->getTag() >= 90) {
					auto tmp = (B2Skeleton*)(agent);
					tmp->updateMe(hero);
				}
				else if (agent->getTag() >= 80 && agent->getTag() < 90) {
					auto tmp = (B2Sprite*)(agent);
					tmp->updateMe(hero);

				}
				if (childLayer2.at(i)->getTag() > 100) {
					auto tmp = (BaseEnemy*)childLayer2.at(i);
					if (tmp->getB2Body() != nullptr) {
						if (tmp->getIsDie()) {
							if (tmp->getIsOccur()) {
								listEnemyOccurInScreen.remove(tmp);
							}
						}
						else {
							if (tmp->getPositionX() + posLayer->getPosition().x < follow->getPositionX() - SCREEN_SIZE.width * 0.7f) {
								listEnemyOccurInScreen.remove(tmp);
							}

							else if (tmp->getPositionX() + posLayer->getPosition().x < follow->getPositionX() + SCREEN_SIZE.width &&
								tmp->getPositionX() + posLayer->getPosition().x > follow->getPositionX() - SCREEN_SIZE.width / 2) {
								if (!tmp->getIsOccur() &&
									(tmp->getPositionX() + posLayer->getPosition().x < follow->getPositionX() + SCREEN_SIZE.width * 0.52f &&
										tmp->getPositionX() + posLayer->getPosition().x > follow->getPositionX() - SCREEN_SIZE.width / 2)) {
									listEnemyOccurInScreen.push_back(tmp);
								}
							}
						}
					}
				}
			}
		}
	}
}

void GameScene::updateBoss()
{

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

void GameScene::runnerItem(int counter)
{
	hud->runnerItem(counter);
	hero->setItemValue(KEY_ITEM_MAGNET, counter);
	hero->getSuctionCoinAni()->setVisible(true);
}

void GameScene::updateBloodBar(int numberOfHealth, bool isVisible)
{
	if (numberOfHealth >= 0) {
		auto blood = (Sprite*)hud->getListBlood()->getObjectAtIndex(numberOfHealth);
		blood->setVisible(isVisible);
	}
}

void GameScene::updateCamera()
{

	if (_aEagle->getIsUp() && _aEagle->getPositionZ() < 100.0f) {
		_aEagle->setPositionZ(_aEagle->getPositionZ() + 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() - 0.04f);
		//background->setPositionZ(background->getPositionZ() + 5);
	}
	if (_aEagle->getIsDown() && _aEagle->getPositionZ() > 0.0f) {
		_aEagle->setPositionZ(_aEagle->getPositionZ() - 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() + 0.04f);
		//background->setPositionZ(background->getPositionZ() - 5);
	}
	if (!_aEagle->getIsUp()) {
		if (hero->getPositionY() > SCREEN_SIZE.height * 5 / 6) {
			background->setPositionY(hero->getPositionY() - SCREEN_SIZE.height * 2 / 6);
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}

		/*if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
			if (!haveboss) {
				if (hero->getPositionX() < tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width)
					follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
				if (hero->getPositionX() > follow->getPositionX() + SCREEN_SIZE.width / 4) {
					this->winGame();
				}
			}
			else {
				follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
			}

		}*/
	}
	else {
		if (hero->getPositionY() > SCREEN_SIZE.height * 0.5f) {
			background->setPositionY(hero->getPositionY());
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}


		//if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
		//	// ngungcamemra
		//	if (!haveboss) {
		//		if (hero->getPositionX() < (tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width))
		//			follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);

		//		if (hero->getPositionX() > follow->getPositionX()+SCREEN_SIZE.width/4) {
		//			this->winGame();
		//		}

		//	}
		//	else {
		//		follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
		//	}
		//	follow->setPositionY(background->getPositionY());
		//}
	}
	if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
		if (hero->getPositionX() > (tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width) && !haveboss)
			this->isWinGame = true;
		if (!isWinGame) {
			follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
		}
		else {
			if (hero->getPositionX() > follow->getPositionX() + SCREEN_SIZE.width / 2) {
				this->winGame();
			}
		}
	}
	background->updatePosition();
	background2->updatePosition();
	follow->setPositionY(background->getPositionY());
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

//void GameScene::cleanMap()
//{
//	for (auto body = world->GetBodyList(); ; body != NULL) {
//		log("a");
//		auto tmp = body;
//		body->GetNext();
//		if (tmp->GetPosition().y < 0) {
//			auto data = (Node*)tmp->GetUserData();
//			data->removeFromParentAndCleanup(true);
//			world->DestroyBody(tmp);
//		}
//	}
//}


void GameScene::cachePlist()
{
	/*SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/bg.plist");*/
}

void GameScene::cacheSkeleton()
{


	//auto scaleOfToanchan1 = SCREEN_SIZE.height / 5 / 401; // 401 la height cua spine
	//sr_toanchan1 = createSkeletonData("Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", "Animation/Enemy_DeTuToanChan1/ToanChan1.json");

//	sr_wooder = createSkeletonData("Animation/Enemy_MocNhan/MocNhan.atlas","Animation/Enemy_MocNhan/MocNhan.json");
	//sr_toanchan1 = createSkeletonData("ToanChan1.atlas", "ToanChan1.json");

	//sr_wooder = createSkeletonData("MocNhan.atlas", "MocNhan.json");

}

void GameScene::shakeTheScreen()
{
	auto shake = MoveBy::create(0.01f, Vec2(0, -0.005f * SCREEN_SIZE.height));
	this->runAction(Sequence::create(shake, shake->reverse(), shake, shake->reverse(), nullptr));
}

void GameScene::reviveHero()
{
	resumeGame();
	hero->resume();
	hero->setDieHard(1);
	hero->setIsPriorInjured(false);
	hero->getBloodScreen()->setVisible(false);
	hero->setHealth(REF->getCurrentHealth());
	for (int i = 0; i < hero->getHealth(); i++) {
		auto health = (Sprite*)hud->getListBlood()->getObjectAtIndex(i);
		health->setVisible(true);
	}

	hero->doDestroyBodies(world);

	auto killAll = CallFunc::create([&]() {
		hero->killThemAll(listEnemyOccurInScreen);
	});

	auto reviveUp = MoveBy::create(1.43f, Vec2(0, hero->getTrueRadiusOfHero() * 2.7f));
	hero->runAction(Sequence::create(reviveUp, killAll, nullptr));
	hero->getFSM()->changeState(MRevive);
}

void GameScene::callingBird()
{
	if (hero->getActiveSkill()->isVisible())
		hero->getActiveSkill()->setVisible(false);

	hero->changeBodyCategoryBits(BITMASK_WOODER);
	hero->getB2Body()->SetGravityScale(0);
	hero->setIsDriverEagle(true);
	hero->setOnGround(false);
	hero->getFSM()->changeState(MIdle);
	hero->setVisible(false);
	hud->setBtnCallingHintDone(false);
	_aEagle->setVisible(true);
	_aEagle->getB2Body()->SetTransform(hero->getB2Body()->GetPosition(), 0.0f);
	_aEagle->flyUp(b2Vec2(hero->getMoveVel(), 7.0f));

	auto _aEagleFlyDown = Sequence::create(DelayTime::create(4.0f), CallFunc::create([&]() {
		_aEagle->flyDown(b2Vec2(hero->getMoveVel(), -7.0f));
	}), nullptr);
	auto _aFinishFly = Sequence::create(DelayTime::create(6.0f), CallFunc::create([&]() {
		heroGetOffEagle();
	}), nullptr);
	_aEagle->runAction(_aEagleFlyDown);
	_aEagle->runAction(_aFinishFly);
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
	blurScreen();
	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogRevive::create(++numberRevive);
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::overGame()
{
	AudioManager::playSound(SOUND_FAIL);
	if (!blur->isVisible())
		blurScreen();

	//hud->getLbScore()->setString(StringUtils::format("%i", hero->getScore()));
	//hud->getLbMoney()->setString(StringUtils::format("%i", hero->getCoinExplored()));

	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible()) {
		hud->getBtnCalling()->setEnabled(false);
	}

	if (hero->getSmokeRun()->isVisible())
		hero->getSmokeRun()->pause();

	hud->pauseIfVisible();

	hero->pause();
	if (hero->getIsDriverEagle())
		_aEagle->pause();

	dialogPause = DialogOverGame::create(hero->getScore(), hero->getCoinExplored());
	this->getParent()->addChild(dialogPause);

	hud->getPauseItem()->setEnabled(false);

	this->pause();
}

void GameScene::nextGame()
{
	auto _scene = SelectStageLayer::createScene(charId);
	Director::getInstance()->replaceScene(TransitionFade::create(0.3f, _scene));
}

void GameScene::winGame()
{
	AudioManager::playSound(SOUND_WIN);
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

	hud->resumeIfVisible();
	hud->getPauseItem()->setEnabled(true);
	if (hud->getBtnCalling() != nullptr && hud->getBtnCalling()->isVisible())
		hud->getBtnCalling()->setEnabled(true);


	this->resume();
}

void GameScene::restartGame()
{
	Director::getInstance()->replaceScene(GameScene::createScene(stage, map, haveboss, charId));
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

	loadPosOfObjectInGroup("coin_parabol", TAG_COIN_PARABOL);
	loadPosOfObjectInGroup("coin_straight", TAG_COIN_STRAIGHT);
	loadPosOfObjectInGroup("coin_zigzag", TAG_COIN_ZIGZAG);
	loadPosOfObjectInGroup("coin_square", TAG_COIN_SQUARE);
	loadPosOfObjectInGroup("coin_circle", TAG_COIN_CIRCLE);
	loadPosOfObjectInGroup("coin_tim", TAG_COIN_TIM);
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
	preLayer = MyLayer::create();
	posLayer = MyLayer::create();
	preLayer->setPosition(Vec2::ZERO);
	preLayer->setAnchorPoint(Point(0, 0));
	posLayer->setPosition(SCREEN_SIZE.width*1.5f, 0);
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

	case TAG_COINBAG: {
		// ok
		//createCointBag(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}
	case TAG_ENEMY_TNB: {
		//createEnemyTNB(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
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
	case TAG_COINBULLION: {

		//createCoinBullion(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()));
		break;
	}

						  //	case TAG_COIN_PARABOL: {
						  //		//ok
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/parapol.tmx", "parapol", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_STRAIGHT: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/straight.tmx", "straight", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_ZIGZAG: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/zigzag.tmx", "zigzag", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_ZIGZAG2: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/zigzag2.tmx", "zigzag2", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_SQUARE: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/square.tmx", "square", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_CIRCLE: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/circle.tmx", "circle", batchNode);
						  //		break;
						  //	}
						  //	case TAG_COIN_TIM: {
						  //		createFormCoin(layer, Vec2(data.x - layer->getPositionX(), data.y - layer->getPositionY()), "Map/tim.tmx", "tim", batchNode);
						  //		break;
						  //	}

	default:
		break;
	}
}

//void GameScene::createMapItem()
//{
//	hero->checkItem.insert(KEY_ITEM_MAGNET,100);
//	hero->checkItem.insert(KEY_ITEM_DOUPLE_COIN, 0);
//}
//
//void GameScene::updateMapItem()
//{
//	for (int i = KEY_ITEM_MAGNET; i < KEY_ITEM_DOUPLE_COIN; i++) {
//		int value = hero->checkItem.at(i);
//		if (value > 0) {
//			value--;
//			hero->checkItem.insert(i, value);
//		};
//	}
//}
