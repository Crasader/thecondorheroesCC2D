#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "Hud.h"
#include "LoadingLayer.h"
#include "chimdieu/ChimDieu.h"
#include "SkeletonManager.h"

Hud *hud;
LoadingLayer* loadingLayer;
ChimDieu* _aEagle;


Scene* GameScene::createScene(int map, int haveboss)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create(map, haveboss);
	layer->setName("gameLayer");


	hud = Hud::create();
	loadingLayer = LoadingLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);
	scene->addChild(loadingLayer);


	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init(int map, int haveboss)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	indexOfNextMapBoss = -1;
	this->haveboss = haveboss;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cachePlist();
	//cacheSkeleton();


	danceWithCamera();


	initB2World();
	loadBackground(map);
	createGroundBody();

	createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas",
		Point(origin.x, visibleSize.height));

	_aEagle = ChimDieu::create("Animation/ChimDieu/ChimDieu-DuongQua.json",
		"Animation/ChimDieu/ChimDieu-DuongQua.atlas", SCREEN_SIZE.height / 2048);
	_aEagle->initCirclePhysic(world, Point(hero->getB2Body()->GetPosition().x - visibleSize.width, visibleSize.height / 2));
	this->addChild(_aEagle, ZORDER_HERO);
	_aEagle->getB2Body()->SetGravityScale(0);


	danceWithEffect();

	creatEnemyWooder();
	// test data
	/*auto test = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan",1.0f);
	test->update(0.0f);
	test->setPosition(SCREEN_SIZE / 2);
	this->addChild(test,100);*/
	//
	creatEnemyToanChanStudent();
	creatEnemyToanChanStudent2();
	if (haveboss)
		creatBoss();
	createCoint();


	return true;
}

GameScene * GameScene::create(int map, int haveboss)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(map, haveboss))
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
	JSHERO->readFile(0);
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);

	addChild(hero, ZORDER_HERO);

	hero->initCirclePhysic(world, hero->getPosition());

	hero->getBloodScreen()->setPosition(follow->getPosition());
	addChild(hero->getBloodScreen(), ZORDER_SMT);
}

void GameScene::onBegin()
{
	hud->addEvents();

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

	if (hud->getBtnSkill_1()->getIsBlocked()) {		// 2 and 3 active
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_1()->setIsBlocked(false);
			}

		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {
				if (!hud->getBtnSkill_1()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_1()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_2()->getIsBlocked()) {		// 1 and 3 active
		if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {
				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}
		else if (currentButton == 3) {
			if (hero->getIsDoneDuration3()) {
				if (!hud->getBtnSkill_2()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_2()->setIsBlocked(false);
			}
		}

	}

	if (hud->getBtnSkill_3()->getIsBlocked()) {		// 2 and 1 active
		if (currentButton == 2) {
			if (hero->getIsDoneDuration2()) {
				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_3()->setIsBlocked(false);
			}

		}
		else if (currentButton == 1) {
			if (hero->getIsDoneDuration1()) {
				if (!hud->getBtnSkill_3()->getNumberCoolDown()->isVisible())
					hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(false);
				hud->getBtnSkill_3()->setIsBlocked(false);
			}
		}

	}

}

void GameScene::listener()
{
	if (hud->getBtnAttack()->getIsActive() && !hud->getBtnAttack()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {


		if (!hero->getIsDoneDuration1()) {
			hero->getFSM()->changeState(MSKill1);  // move to attack
			hero->setIsPriorSkill1(true);			// move to attack
		}

		else {
			hero->changeSwordCategoryBitmask(BITMASK_SWORD);

			hero->getFSM()->changeState(MAttack);
			hero->setIsPriorAttack(true);
		}

		hud->getBtnAttack()->setIsActive(false);
	}

	if (hud->getBtnSkill_1()->getIsActive() && !hud->getBtnSkill_1()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 1;

		hero->setIsDoneDuration1(false);
		hero->doCounterSkill1();

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(true);


		hud->getBtnSkill_3()->setIsBlocked(true);
		hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(true);


		hud->getBtnSkill_1()->setIsActive(false);
	}

	if (hud->getBtnSkill_2()->getIsActive() && !hud->getBtnSkill_2()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 2;

		hero->setIsDoneDuration2(false);
		hero->doCounterSkill2();

		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSkill_3()->setIsBlocked(true);
		hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(true);


		hud->getBtnSkill_2()->setIsActive(false);
	}

	if (hud->getBtnSkill_3()->getIsActive() && !hud->getBtnSkill_3()->getIsBlocked() &&
		hero->getFSM()->currentState != MDie && hero->getFSM()->currentState != MInjured) {

		currentButton = 3;

		hero->setIsDoneDuration3(false);
		hero->doCounterSkill3();

		// block
		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSkill_3()->setIsActive(false);
	}

	if (hud->getBtnSpecial()->getIsActive() && !hud->getBtnSpecial()->getIsBlocked()) {
		if (!hero->getIsDriverEagle()) {
			Vec2 screenSize = Director::getInstance()->getVisibleSize();
			hero->setIsDriverEagle(true);
			hero->setOnGround(false);
			hero->getFSM()->changeState(MIdle);
			hero->setVisible(false);
			hud->setBtnSpecialHintDone(false);
			_aEagle->setVisible(true);
			_aEagle->getB2Body()->SetTransform(hero->getB2Body()->GetPosition(), 0.0f);
			_aEagle->flyUp(b2Vec2(hero->getMoveVel(), 7.0f));

			auto _aEagleFlyDown = Sequence::create(DelayTime::create(4.0f), CallFunc::create([&]() {
				_aEagle->flyDown(b2Vec2(hero->getMoveVel(), -7.0f));
			}), nullptr);
			auto _aHeroGetOffEagle = Sequence::create(DelayTime::create(6.0f), CallFunc::create([&]() { heroGetOffEagle(); }), nullptr);
			runAction(_aEagleFlyDown);
			runAction(_aHeroGetOffEagle);
		}

		// block

		hud->getBtnAttack()->setIsBlocked(true);

		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSkill_3()->setIsBlocked(true);
		hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(true);

		hud->getBtnSpecial()->setIsActive(false);
		hud->removeSpecial();
	}
}

void GameScene::heroGetOffEagle() {
	hero->setIsDriverEagle(false);
	hero->getB2Body()->SetTransform(b2Vec2(_aEagle->getB2Body()->GetPosition().x, _aEagle->getB2Body()->GetPosition().y - 100.0f / PTM_RATIO), 0.0f);
	hero->setVisible(true);
	hero->getB2Body()->SetGravityScale(1);
	hero->getFSM()->changeState(MLand);
	_aEagle->flyAway();

	hud->getBtnAttack()->setIsBlocked(false);

	hud->getBtnSkill_1()->setIsBlocked(false);
	hud->getBtnSkill_1()->getCoolDownSprite()->setVisible(false);

	hud->getBtnSkill_2()->setIsBlocked(true);
	hud->getBtnSkill_2()->getCoolDownSprite()->setVisible(false);

	hud->getBtnSkill_3()->setIsBlocked(true);
	hud->getBtnSkill_3()->getCoolDownSprite()->setVisible(false);
}

void GameScene::update(float dt)
{
	//creatEnemyWooderRT();
	updateB2World(dt);
	listener();

	checkActiveButton();

	_aEagle->updateMe(dt);
	if (hero->getIsDriverEagle()) {
		hero->getB2Body()->SetGravityScale(0);
		hero->getB2Body()->SetLinearVelocity(b2Vec2_zero);
		hero->getB2Body()->SetTransform(b2Vec2(_aEagle->getB2Body()->GetPosition().x, _aEagle->getB2Body()->GetPosition().y - 100.0f / PTM_RATIO), 0.0f);
	}

	hero->updateMe(dt);

	if (hero->getCurrentRunDis() - hero->getPreRunDis() > 10.0f) {
		updateScore(15);
		hero->setPreRunDis(hero->getCurrentRunDis());
	}

	updateEnemy();
	//cleanMap();
	updateCamera();

	// fall down some hold
	/*if (hero->getPositionY() < 0) {
		hero->getFSM()->changeState(MLandRevive);
		hero->getB2Body()->SetGravityScale(0);
		hero->setOnGround(false);
		hero->getB2Body()->SetTransform(b2Vec2(hero->getB2Body()->GetPosition().x, SCREEN_SIZE.height / 10 / PTM_RATIO), 0.0f);
		hud->hintSpecial(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	}

	if (hud->getBtnSpecialHintDone()) {
		static float g_fTimeCounter = 0.0f;
		g_fTimeCounter += dt;
		if (g_fTimeCounter > 5.0f / 180) {
			bool _bResult = hud->specialCooldown();
			if (!_bResult) {
				hud->setBtnSpecialHintDone(false);
				Director::getInstance()->replaceScene(MenuLayer::createScene());
			}
			g_fTimeCounter = 0.0f;
		}
	}*/

	//if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
	//	if (_aEagle->getIsUp() && this->getPositionZ() < 100.0f) {
	//		this->setPositionZ(this->getPositionZ() + 5.0f);
	//		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() - 0.04f);
	//	}
	//	if (_aEagle->getIsDown() && this->getPositionZ() > 0.0f) {
	//		this->setPositionZ(this->getPositionZ() - 5.0f);
	//		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() + 0.04f);
	//	}
	//	/*if (hero->getIsDriverEagle()) {
	//	follow->setPositionX(_aEagle->getPositionX() + SCREEN_SIZE.width / 10);
	//	follow->setPositionY(_aEagle->getPositionY() + SCREEN_SIZE.height / 10);
	//	}
	//	else {
	//	follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 3);
	//	follow->setPositionY(hero->getPositionY() + SCREEN_SIZE.height / 4);
	//	}*/
	//	follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4 - SCREEN_SIZE.width * 3 / 20 * this->getPositionZ() / 100.0f);
	//	follow->setPositionY(hero->getPositionY() + SCREEN_SIZE.height / 4 - SCREEN_SIZE.height * 3 / 20 * this->getPositionZ() / 100.0f);
	//}

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
		hero->getBloodScreen()->setPositionX(follow->getPositionX());

	updateCharacterPoint();

}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 10.0f / 3.0f / PTM_RATIO));

	// draw debug
	auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	//world->SetDebugDraw(debugDraw);
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


void GameScene::loadBackground(int map)
{
	// khoi tao map

	tmx_map = TMXTiledMap::create(StringUtils::format("Map/map1/map%d.tmx", map));
	tmx_map->setAnchorPoint(Point::ZERO);
	scaleOfMap = SCREEN_SIZE.height / tmx_map->getContentSize().height;
	tmx_map->setScale(scaleOfMap);

	tmx_map->setPosition(Point::ZERO);

	//tmx_map->setVisible(false);

	this->addChild(tmx_map, ZORDER_BG2);
	if (haveboss) {
		for (int i = 0; i < 2; i++) {
			tmx_mapboss[i] = TMXTiledMap::create("Map/map1/mapboss.tmx");
			tmx_mapboss[i]->setAnchorPoint(Point::ZERO);
			tmx_mapboss[i]->setScale(scaleOfMap);
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

	auto bg1_1 = Sprite::create("Map/bg1.png");
	//auto bg1_1 = Sprite::create("bg-4.png");
	bg1_1->setScale(SCREEN_SIZE.width / (bg1_1->getContentSize().width / 2));
	//bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::create("Map/bg1.png");
	//auto bg1_2 = Sprite::create("bg-4.png");
	bg1_2->setScale(SCREEN_SIZE.width / (bg1_2->getContentSize().width / 2));
	//bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5f));

	auto bg2_1 = Sprite::create("Map/bg2.png");
	bg2_1->setScale(SCREEN_SIZE.width / (bg2_1->getContentSize().width / 2));
	//bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::create("Map/bg2.png");
	bg2_2->setScale(SCREEN_SIZE.width / (bg2_2->getContentSize().width/2));
	//bg2_2->setScaleY(SCREEN_SIZE.height / bg2_2->getContentSize().height);
	bg2_2->setAnchorPoint(Point(0, 0.5f));

	auto bg3_1 = Sprite::create("Map/bg3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("Map/bg3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));


	background->addChild(bg1_1, 0, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 0, Vec2(0.5f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));
	background->addChild(bg2_1, 0, Vec2(0.7f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 0, Vec2(0.7f, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	//background->addChild(bg3_1, 0, Vec2(1.5f, 1), Vec2(0, 0));
	//background->addChild(bg3_2, 0, Vec2(1.5f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(-SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	//background->setVisible(false);
	this->addChild(background, ZORDER_BG);
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
	if (!groupGround) return;
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

void GameScene::creatEnemyWooder()
{
	auto groupGround = tmx_map->getObjectGroup("wooder");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
		//auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
		//	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
		auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan", scaleOfWooder);
		enemy->setPosition(origin);
		enemy->setVisible(true);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
		enemy->changeBodyCategoryBits(BITMASK_WOODER);
		enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		enemy->listener();
	}
}

void GameScene::creatEnemyToanChanStudent()
{
	auto groupGround = tmx_map->getObjectGroup("toanchan_student");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
		/*auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1.json",
			"Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", scaleOfEnemy);*/
		auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1", scaleOfEnemy);
		enemy->setPosition(origin);
		enemy->setVisible(false);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 4));
		enemy->changeBodyCategoryBits(BITMASK_TOANCHAN1);
		enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		//enemy->genSplash();
		enemy->listener();
	}
}

void GameScene::creatEnemyToanChanStudent2()
{
	auto groupGround = tmx_map->getObjectGroup("toanchan_student2"); if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
		/*auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2.json",
			"Animation/Enemy_DeTuToanChan2/ToanChan2.atlas", scaleOfEnemy);*/
		auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2", scaleOfEnemy);
		enemy->setPosition(origin);
		enemy->setVisible(false);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
		enemy->changeBodyCategoryBits(BITMASK_TOANCHAN2);
		enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		enemy->genSlash();
		enemy->listener();
		auto slash = enemy->getSlash();
		slash->initCirclePhysic(world, slash->getPosition());
		slash->changeBodyCategoryBits(BITMASK_SLASH);
		slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
		slash->getB2Body()->GetFixtureList()->SetSensor(true);
	}
}

void GameScene::creatBoss()
{
	auto groupGround = tmx_map->getObjectGroup("boss");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 3.0f / 560; // 560 la height cua spine
		/*auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);*/
		auto enemy = EnemyBoss1::create("Animation/Enemy_Boss1/Boss1.json",
			"Animation/Enemy_Boss1/Boss1.atlas", scaleOfEnemy);
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
	}
}

void GameScene::creatEnemyWooderRT()
{
	auto groupGround = tmx_map->getObjectGroup("wooder");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		string a = mObject["id"].asString();
		try {
			checkGenEnemy.at(a);
		}
		catch (out_of_range& e) {
			//log("Key: %s", a.c_str());
			if (origin.x > follow->getPositionX() + SCREEN_SIZE.width) continue;

			//if (origin.x < follow->getPositionX()+SCREEN_SIZE.width) {
			auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
																   //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
																   //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
			auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan", scaleOfWooder);
			enemy->setPosition(origin);
			enemy->setVisible(true);
			this->addChild(enemy, ZORDER_ENEMY);
			enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
			enemy->changeBodyCategoryBits(BITMASK_WOODER);
			enemy->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
			enemy->listener();
			checkGenEnemy.insert(std::pair<string, bool>(mObject["id"].asString(), true));
		}

		//}
	}
}

void GameScene::creatEnemyToanChanStudentRT()
{
}

void GameScene::creatEnemyToanChanStudent2RT()
{
}

void GameScene::createCoint()
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
	auto groupGround = tmx_map->getObjectGroup("coin_bag");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 5.0f / 123; //  la height cua spine
		auto coin = CoinBag::create("Gold_bag.json",
			"Gold_bag.atlas", scaleOfEnemy);
		coin->setPosition(origin);
		//enemy->setVisible(false);
		this->addChild(coin, ZORDER_ENEMY);
		coin->initCirclePhysic(world, Point(origin.x, origin.y));
		coin->getB2Body()->SetType(b2_staticBody);
		coin->getB2Body()->GetFixtureList()->SetSensor(true);
		coin->changeBodyCategoryBits(BITMASK_COIN_BAG);
		coin->changeBodyMaskBits(BITMASK_SWORD);
		coin->updateMe(0.0f);
		//enemy->listener();
	}
}

void GameScene::createCoinBullion()
{
	auto groupGround = tmx_map->getObjectGroup("coin_bullion");
	if (!groupGround) return;
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 6.0f / 118; //  la height cua spine
		auto coin = CoinBullion::create("gold.json",
			"gold.atlas", scaleOfEnemy);
		coin->setPosition(origin);
		//enemy->setVisible(false);
		this->addChild(coin, ZORDER_ENEMY);
		coin->initCirclePhysic(world, Point(origin.x, origin.y));
		coin->getB2Body()->SetType(b2_staticBody);
		coin->getB2Body()->GetFixtureList()->SetSensor(true);
		coin->changeBodyCategoryBits(BITMASK_COIN_BULLION);
		coin->changeBodyMaskBits(BITMASK_HERO);
		coin->updateMe(0.0f);
		//enemy->listener();
	}
}

//void GameScene::createTimCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_tim");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/tim.tmx");
//		auto groupCoin = tmx->getObjectGroup("tim");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createParapolCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_parapol");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/parapol.tmx");
//		auto groupCoin = tmx->getObjectGroup("parapol");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createCircleCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_circle");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/circle.tmx");
//		auto groupCoin = tmx->getObjectGroup("circle");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createSquareCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_square");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/square.tmx");
//		auto groupCoin = tmx->getObjectGroup("square");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createStraightCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_straight");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/straight.tmx");
//		auto groupCoin = tmx->getObjectGroup("straight");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createZigzagCoin()
//{
//	auto group = tmx_map->getObjectGroup("coin_straight");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/straight.tmx");
//		auto groupCoin = tmx->getObjectGroup("straight");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}
//
//void GameScene::createZigzagCoin2()
//{
//	auto group = tmx_map->getObjectGroup("coin_straight");
//	for (auto child : group->getObjects()) {
//		auto mObject = child.asValueMap();
//		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
//		auto tmx = TMXTiledMap::create("Map/straight.tmx");
//		auto groupCoin = tmx->getObjectGroup("straight");
//		for (auto c : groupCoin->getObjects()) {
//			auto mObject2 = c.asValueMap();
//			Point origin2 = Point(mObject2["x"].asFloat() *scaleOfMap, mObject2["y"].asFloat()* scaleOfMap);
//			auto coin = Coin::create();
//			auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
//			coin->setScale(scale);
//			coin->setPosition(origin + origin2);
//			this->addChild(coin, ZORDER_ENEMY);
//			coin->initCirclePhysic(world, origin + origin2);
//			coin->changeBodyCategoryBits(BITMASK_COIN);
//			coin->changeBodyMaskBits(BITMASK_HERO);
//			coin->runAnimation();
//		}
//	}
//}

//spSkeletonData * GameScene::createSkeletonData(string atlasFileName, string jsonFileName)
//{
//	/*std::string atlasFile = atlasFileName;
//	std::string skeletonDataFile = jsonFileName;
//	float timeScale = 1.0;
//	auto _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
//	CCASSERT(_atlas, "Error reading atlas file.");
//
//	spSkeletonJson* json = spSkeletonJson_create(_atlas);
//	json->scale = timeScale;
//	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
//	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
//	spSkeletonJson_dispose(json);
//	return skeletonData;*/
//}

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
			coin->changeBodyMaskBits(BITMASK_HERO);
			coin->runAnimation();
		}
	}
}

void GameScene::danceWithEffect()
{
	EM->createWithFile(hero->getScale() / 3);
	addChild(EM->getSlashBreak(), 4);
	addChild(EM->getSmokeJumpX2(), 4);
	addChild(EM->getSmokeLanding(), 4);
	addChild(EM->getSmokeRun(), 4);
}

void GameScene::danceWithCamera()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	follow = Node::create();
	follow->setPosition(origin + SCREEN_SIZE / 2);
	addChild(follow);

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

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_FLOOR;
	fixtureDef.filter.maskBits = BITMASK_HERO;

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



//void GameScene::readWriteJson()
//{
//	/**
//	* test json
//	*/
//
//	Document heroJsonFile;
//	string herobuffer = FileUtils::getInstance()->getStringFromFile("Hero.json");
//	heroJsonFile.Parse(herobuffer.c_str());
//	assert(heroJsonFile.IsObject());
//	heroJsonFile["hero"][0]["level"].SetInt(2);
//	//log("doi tuong thu nhat:%d", heroJsonFile["hero"][0]["level"].GetInt());
//
//	StringBuffer buffer;
//	Writer<StringBuffer> writer(buffer);
//	heroJsonFile.Accept(writer);
//	const char* output = buffer.GetString();
//
//	FileUtils::getInstance()->writeStringToFile(output, "Hero.json");
//}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (left_corner.containsPoint(touch->getLocation())
		&& !hud->getBtnSpecial()->getBoundingBox().containsPoint(touch->getLocation())
		) {

		// cannot jump while attacking or being injured
		if (hero->getFSM()->currentState == MAttack || hero->getFSM()->currentState == MInjured ||
			hero->getFSM()->currentState == MDie || hero->getFSM()->currentState == MSKill1)

			return false;

		if (hero->getNumberOfJump() > 0) {
			hero->setNumberOfJump(hero->getNumberOfJump() - 1);
			auto currentVelX = hero->getB2Body()->GetLinearVelocity().x;
			hero->getB2Body()->SetLinearVelocity(b2Vec2(currentVelX, hero->getJumpVel()));

			if (hero->getFSM()->currentState == MLand && hero->getNumberOfJump() > 0) {
				EM->getSmokeJumpX2()->setPosition(hero->getPosition());
				EM->getSmokeJumpX2()->setVisible(true);
				EM->smokeJumpX2Ani();
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
		Director::getInstance()->replaceScene(MenuLayer::createScene());
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

				}
				else {
					auto boss = (EnemyBoss1*)tmp;
					if (tmp->getB2Body()->GetLinearVelocity().x != 0)
					{
						boss->updateMe(hero->getPosition());
					}
					else if (tmp->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width / 2) {
						auto boss = (EnemyBoss1*)tmp;
						boss->updateMe(hero->getPosition());
					}
				}
			}
			else if (tmp->getB2Body() != nullptr) {
				if (tmp->getIsDie()) {
					tmp->die();
				}
				else {
					if (tmp->getPositionX() < follow->getPositionX() - SCREEN_SIZE.width) {
						tmp->setIsDie(true);

					}

					else if (tmp->getPositionX() < follow->getPositionX() + SCREEN_SIZE.width &&
						tmp->getPositionX() > follow->getPositionX() - SCREEN_SIZE.width / 2) {
						tmp->setVisible(true);
						tmp->updateMe(1.0f);
					}
				}
			}

		}
	}
}

void GameScene::updateBoss()
{

}

void GameScene::updateMoney(int numberOfCoin)
{
	hero->setCoinExplored(hero->getCoinExplored() + numberOfCoin);
	hud->getLbMoney()->setString(StringUtils::format("%i", hero->getCoinExplored()));
}

void GameScene::updateScore(int score)
{
	hero->setScore(hero->getScore() + score);
	hud->getLbScore()->setString(StringUtils::format("%i", hero->getScore()));
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
	background->updatePosition();
	if (_aEagle->getIsUp() && this->getPositionZ() < 100.0f) {
		this->setPositionZ(this->getPositionZ() + 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() - 0.04f);
	}
	if (_aEagle->getIsDown() && this->getPositionZ() > 0.0f) {
		this->setPositionZ(this->getPositionZ() - 5.0f);
		_aEagle->setSequenceCloud(_aEagle->getSequenceCloud() + 0.04f);
	}
	if (!_aEagle->getIsUp()) {
		if (hero->getPositionY() > SCREEN_SIZE.height * 3 / 4) {
			background->setPositionY(hero->getPositionY() - SCREEN_SIZE.height * 1 / 4);
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}


		if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
			if (!haveboss) {
				if (hero->getPositionX() < tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width / 1.8f)
					follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);

			}
			else {
				follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
			}
			follow->setPositionY(background->getPositionY());
		}
	}
	else {
		if (hero->getPositionY() > SCREEN_SIZE.height * 2 / 4) {
			background->setPositionY(hero->getPositionY() - SCREEN_SIZE.height * 0);
		}
		else
		{
			background->setPositionY(SCREEN_SIZE.height / 2);
		}


		if (hero->getPositionX() >= SCREEN_SIZE.width / 4) {
			// ngungcamemra
			if (!haveboss) {
				if (hero->getPositionX() < tmx_map->getBoundingBox().size.width - SCREEN_SIZE.width / 1.8f)
					follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);

			}
			else {
				follow->setPositionX(hero->getPositionX() + SCREEN_SIZE.width / 4);
			}
			follow->setPositionY(background->getPositionY());
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
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
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
	//log("SHAKING ME");
	auto shake = MoveBy::create(0.01f, Vec2(0, -0.005f * SCREEN_SIZE.height));
	this->runAction(Sequence::create(shake, shake->reverse(), shake, shake->reverse(), nullptr));
}
