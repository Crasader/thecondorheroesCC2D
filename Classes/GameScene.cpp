#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "MenuScene.h"

using namespace rapidjson;

Hud *hud;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	hud = Hud::create();

	// add layer as a child to scene
	scene->addChild(layer);
	scene->addChild(hud);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	cachePlist();
	//cacheSkeleton();


	danceWithCamera();


	initB2World();
	loadBackground();
	createGroundBody();

	createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas",
		Point(visibleSize.width * 0.25f, visibleSize.height));

	danceWithEffect();

	creatEnemyWooder();
	creatEnemyToanChanStudent();
	creatEnemyToanChanStudent2();
	createCoint();


	auto touch_listener = EventListenerTouchOneByOne::create();
	auto keylistener = EventListenerKeyboard::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	keylistener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	this->scheduleUpdate();

	return true;
}


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->listener();
	hero->setPosition(position);
	hero->initCirclePhysic(world, hero->getPosition());
	
	addChild(hero, ZORDER_HERO);
	//addChild(hero->getSlash_1(), 4);
	addChild(hero->getSlash_2(), 4);
}

void GameScene::checkActiveButton()
{

	if (hud->getBtnAttack()->getIsBlocked()) {
		if (!hud->getBtnSkill_3()->getCanTouch() && !hero->getIsPriorSkill3()) { // check later
			hud->getBtnAttack()->setIsBlocked(false);
		}

	}

	if (hud->getBtnSkill_1()->getIsBlocked()) {		// 2 and 3 active
		if (!hud->getBtnSkill_2()->getCanTouch() && !hero->getIsPriorSkill2()) {
			hud->getBtnSkill_1()->setIsBlocked(false);
		}
		else if (!hud->getBtnSkill_3()->getCanTouch() && !hero->getIsPriorSkill3()) {
			hud->getBtnSkill_1()->setIsBlocked(false);
		}

	}

	if (hud->getBtnSkill_2()->getIsBlocked()) {		// 1 and 3 active
		if (!hud->getBtnSkill_1()->getCanTouch() && hero->getIsDoneDuration1()) {
			log("Done duration 1");
			hud->getBtnSkill_2()->setIsBlocked(false);
		}
		else if (!hud->getBtnSkill_3()->getCanTouch() && !hero->getIsPriorSkill3()) {
			hud->getBtnSkill_2()->setIsBlocked(false);
		}

	}

	if (hud->getBtnSkill_3()->getIsBlocked()) {		// 2 and 1 active
		if (!hud->getBtnSkill_2()->getCanTouch() && !hero->getIsPriorSkill2()) {
			hud->getBtnSkill_3()->setIsBlocked(false);
		}
		else if (!hud->getBtnSkill_1()->getCanTouch() && hero->getIsDoneDuration1()) {
			log("Done duration 1");
			hud->getBtnSkill_3()->setIsBlocked(false);
		}

	}

}

void GameScene::listener()
{
	if (hud->getBtnAttack()->getIsActive() && !hud->getBtnAttack()->getIsBlocked()) {

		// you cannot attack while being dead
		if (hero->getFSM()->currentState == MDie) {
			log("You cannot");
			hud->getBtnAttack()->setIsActive(false);
			return;
		}

		if (! hero->getIsDoneDuration1()) {
			hero->getFSM()->changeState(MSKill1);  // move to attack
			hero->setIsPriorSkill1(true);			// move to attack
		}
		else {
			hero->changeSwordCategoryBitmask(BITMASK_SWORD);

			hero->getFSM()->changeState(MAttack);
			hero->setIsPrior(true);
		}

		hud->getBtnAttack()->setIsActive(false);
	}

	if (hud->getBtnSkill_1()->getIsActive() && !hud->getBtnSkill_1()->getIsBlocked()) {

		// you cannot attack while being dead
		if (hero->getFSM()->currentState == MDie) {
			log("You cannot");
			hud->getBtnAttack()->setIsActive(false);
			return;
		}

		hero->setIsDoneDuration1(false);
		hero->doCounterSkill1();
		

		hud->getBtnSkill_2()->setIsBlocked(true);
		hud->getBtnSkill_3()->setIsBlocked(true);

		hud->getBtnSkill_1()->setIsActive(false);
	}

	if (hud->getBtnSkill_2()->getIsActive() && !hud->getBtnSkill_2()->getIsBlocked()) {

		// you cannot attack while being dead
		if (hero->getFSM()->currentState == MDie) {
			log("You cannot");
			hud->getBtnAttack()->setIsActive(false);
			return;
		}

		hero->getFSM()->changeState(MSKill2);
		hero->setIsPriorSkill2(true);

		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_3()->setIsBlocked(true);

		hud->getBtnSkill_2()->setIsActive(false);
	}

	if (hud->getBtnSkill_3()->getIsActive() && !hud->getBtnSkill_3()->getIsBlocked()) {

		// you cannot attack while being dead
		if (hero->getFSM()->currentState == MDie) {
			log("You cannot");
			hud->getBtnAttack()->setIsActive(false);
			return;
		}

		hero->getFSM()->changeState(MSKill3);
		hero->setIsPriorSkill3(true);

		// block
		hud->getBtnAttack()->setIsBlocked(true);
		hud->getBtnSkill_1()->setIsBlocked(true);
		hud->getBtnSkill_2()->setIsBlocked(true);

		hud->getBtnSkill_3()->setIsActive(false);
	}


}

void GameScene::update(float dt)
{
	updateB2World(dt);
	listener();
	hero->updateMe(dt);

	checkActiveButton();

	updateEnemy();
	//cleanMap();

	if (hero->getPositionX() < SCREEN_SIZE.width / 4) {
		follow->setPositionX(SCREEN_SIZE.width / 2);
	}
	else
		follow->setPositionX(hero->getPositionX()+ SCREEN_SIZE.width / 4);

	background->updatePosition();

}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 10.0f / 3.0f / PTM_RATIO));

	// draw debug
	auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;

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
	tmx_map = TMXTiledMap::create("Map/map1/map.tmx");
	tmx_map->setAnchorPoint(Point::ZERO);
	scaleOfMap = SCREEN_SIZE.height / tmx_map->getContentSize().height;
	tmx_map->setScale(scaleOfMap);

	tmx_map->setPosition(Point::ZERO);

	tmx_map->setVisible(false);

	this->addChild(tmx_map, ZORDER_BG2);
	createInfiniteNode();
}

void GameScene::createInfiniteNode()
{
	background = InfiniteParallaxNode::create();

	auto bg1_1 = Sprite::create("Map/bg1.png");
	//auto bg1_1 = Sprite::create("bg-4.png");
	bg1_1->setScaleX(SCREEN_SIZE.width / bg1_1->getContentSize().width);
	bg1_1->setScaleY(SCREEN_SIZE.height / bg1_1->getContentSize().height);
	bg1_1->setAnchorPoint(Point(0, 0.5f));

	auto bg1_2 = Sprite::create("Map/bg1.png");
	//auto bg1_2 = Sprite::create("bg-4.png");
	bg1_2->setScaleX(SCREEN_SIZE.width / bg1_2->getContentSize().width);
	bg1_2->setScaleY(SCREEN_SIZE.height / bg1_2->getContentSize().height);
	bg1_2->setAnchorPoint(Point(0, 0.5f));

	auto bg2_1 = Sprite::create("Map/bg2.png");
	bg2_1->setScaleX(SCREEN_SIZE.width / bg2_1->getContentSize().width);
	bg2_1->setScaleY(SCREEN_SIZE.height / bg2_1->getContentSize().height);
	bg2_1->setAnchorPoint(Point(0, 0.5f));

	auto bg2_2 = Sprite::create("Map/bg2.png");
	bg2_2->setScaleX(SCREEN_SIZE.width / bg2_2->getContentSize().width);
	bg2_2->setScaleY(SCREEN_SIZE.height / bg2_2->getContentSize().height);
	bg2_2->setAnchorPoint(Point(0, 0.5f));

	auto bg3_1 = Sprite::create("Map/bg3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("Map/bg3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));

	/*auto bg3_1 = Sprite::create("bg-3.png");
	bg3_1->setScaleX(SCREEN_SIZE.width / bg3_1->getContentSize().width);
	bg3_1->setScaleY(SCREEN_SIZE.height / bg3_1->getContentSize().height);
	bg3_1->setAnchorPoint(Point(0, 0.5f));

	auto bg3_2 = Sprite::create("bg-3.png");
	bg3_2->setScaleX(SCREEN_SIZE.width / bg3_2->getContentSize().width);
	bg3_2->setScaleY(SCREEN_SIZE.height / bg3_2->getContentSize().height);
	bg3_2->setAnchorPoint(Point(0, 0.5f));*/


	background->addChild(bg1_1, 0, Vec2(0.5f, 1), Vec2(0, 0));
	background->addChild(bg1_2, 0, Vec2(0.5f, 1), Vec2(bg1_1->getBoundingBox().size.width, 0));
	background->addChild(bg2_1, 0, Vec2(0.7f, 1), Vec2(0, 0));
	background->addChild(bg2_2, 0, Vec2(0.7f, 1), Vec2(bg2_1->getBoundingBox().size.width, 0));
	//background->addChild(bg3_1, 0, Vec2(1.5f, 1), Vec2(0, 0));
	//background->addChild(bg3_2, 0, Vec2(1.5f, 1), Vec2(bg3_1->getBoundingBox().size.width, 0));
	background->setPosition(Point(-SCREEN_SIZE.width / 2, SCREEN_SIZE.height / 2));
	background->setAnchorPoint(Point(0, 0.5f));
	background->setVisible(false);
	this->addChild(background, ZORDER_BG);
}

void GameScene::createGroundBody()
{
	auto groupGround = tmx_map->getObjectGroup("ground");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		initGroundPhysic(world, pos, sizeOfBound);
	}
}

void GameScene::creatEnemyWooder()
{
	auto groupGround = tmx_map->getObjectGroup("wooder");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
		auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
			"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
		enemy->setPosition(origin);
		enemy->setVisible(false);
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
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
		auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1.json",
			"Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", scaleOfEnemy);
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
	auto groupGround = tmx_map->getObjectGroup("toanchan_student2");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
		auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2.json",
			"Animation/Enemy_DeTuToanChan2/ToanChan2.atlas", scaleOfEnemy);
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
		slash->changeBodyMaskBits(BITMASK_HERO|BITMASK_SWORD);
		slash->getB2Body()->GetFixtureList()->SetSensor(true);
	}
}

void GameScene::createCoint()
{
	createTimCoin();
	createParapolCoin();
	createCircleCoin();
	createSquareCoin();
}

void GameScene::createTimCoin()
{
	auto group = tmx_map->getObjectGroup("coin_tim");
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto tmx = TMXTiledMap::create("Map/tim.tmx");
		auto groupCoin = tmx->getObjectGroup("tim");
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

void GameScene::createParapolCoin()
{
	auto group = tmx_map->getObjectGroup("coin_parapol");
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto tmx = TMXTiledMap::create("Map/parapol.tmx");
		auto groupCoin = tmx->getObjectGroup("parapol");
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

void GameScene::createCircleCoin()
{
	auto group = tmx_map->getObjectGroup("coin_circle");
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto tmx = TMXTiledMap::create("Map/circle.tmx");
		auto groupCoin = tmx->getObjectGroup("circle");
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

void GameScene::createSquareCoin()
{
	auto group = tmx_map->getObjectGroup("coin_square");
	for (auto child : group->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto tmx = TMXTiledMap::create("Map/square.tmx");
		auto groupCoin = tmx->getObjectGroup("square");
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

void GameScene::readWriteJson()
{
	/**
	* test json
	*/

	Document heroJsonFile;
	string herobuffer = FileUtils::getInstance()->getStringFromFile("Hero.json");
	heroJsonFile.Parse(herobuffer.c_str());
	assert(heroJsonFile.IsObject());
	heroJsonFile["hero"][0]["level"].SetInt(2);
	//log("doi tuong thu nhat:%d", heroJsonFile["hero"][0]["level"].GetInt());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	heroJsonFile.Accept(writer);
	const char* output = buffer.GetString();

	FileUtils::getInstance()->writeStringToFile(output, "Hero.json");
}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (left_corner.containsPoint(touch->getLocation())) {

		// cannot jump while attacking or being injured
		if (hero->getFSM()->currentState == MAttack || hero->getFSM()->currentState == MInjured ||
			hero->getFSM()->currentState == MDie || hero->getFSM()->currentState == MSKill1 ||
			hero->getFSM()->currentState == MSKill2)

			return false;

		if (hero->getNumberOfJump() > 0) {
			hero->setNumberOfJump(hero->getNumberOfJump() - 1);
			hero->setOnGround(false);

			hero->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, hero->getJumpVel()));

			if (hero->getFSM()->currentState == MSKill3) {		// skill 3, can jump
				return false;
			}

			
			if (hero->getNumberOfJump() == 1)
				hero->getFSM()->changeState(MJump);
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
			if (tmp->getB2Body() != nullptr) {
				if (tmp->getIsDie()) {
					tmp->getB2Body()->SetType(b2_dynamicBody);

					world->DestroyBody(tmp->getB2Body());
					tmp->setB2Body(nullptr);
					//tmp->removeFromParentAndCleanup(true);
					//}
				}
				else {
					if (tmp->getPositionX() < follow->getPositionX() - SCREEN_SIZE.width) {
						//tmp->setIsDie(true);
						world->DestroyBody(tmp->getB2Body());
						tmp->removeFromParentAndCleanup(true);

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
