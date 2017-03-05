#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	/*follow = Node::create();
	follow->setPosition(SCREEN_SIZE / 2);
	this->addChild(follow);
	camera= Follow::create(follow);
	this->runAction(camera);*/
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	cachePlist();

	
	danceWithCamera();


	initB2World();
	loadBackground();
	createGroundBody();


	creatEnemyWooder();
	createCoint();

	
  

	createDuongQua("Animation/DuongQua/DuongQua.json", "Animation/DuongQua/DuongQua.atlas", Point(visibleSize.width * 0.3f, visibleSize.height));
	//creatEnemyWooder();

	auto touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);

	this->scheduleUpdate();

    return true;
}


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 340);
	hero->setPosition(position);
	hero->initCirclePhysic(world, hero->getPosition());
	addChild(hero);
}

void GameScene::listener()
{
	if (hud->getBtnAttack()->getIsActive()) {
		hud->getBtnAttack()->setIsActive(false);
	}
}

void GameScene::update(float dt)
{
	updateB2World(dt);
	listener();
	hero->update(dt);

	updateEnemy();

	if (follow->getPositionX() <= hero->getPositionX())
		follow->setPositionX(hero->getPositionX());
}

void GameScene::initB2World()
{
	world = new b2World(b2Vec2(0, -SCREEN_SIZE.height * 8.0f / 3.0f / PTM_RATIO));

	// draw debug
	auto debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
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
	//tmx_map->setVisible(false);
	this->addChild(tmx_map,ZORDER_BG);
}

void GameScene::createGroundBody()
{
	auto groupGround = tmx_map->getObjectGroup("ground");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scaleOfMap, mObject["height"].asFloat() *scaleOfMap);
		Point pos = Point(origin.x + sizeOfBound.width / 2,origin.y);
		initGroundPhysic(world, pos, sizeOfBound);
	}
}

void GameScene::creatEnemyWooder()
{
	auto groupGround = tmx_map->getObjectGroup("wooder");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		auto scaleOfWooder = SCREEN_SIZE.height / 5 / 490; // 490 la height cua spine
		auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json", "Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
		enemy->setPosition(origin);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
	}
}

void GameScene::createCoint()
{
	createTimCoin();
	createParapolCoin();
	createCircleCoin();

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
			auto scale = SCREEN_SIZE.height / 20 / coin->getContentSize().height;
			coin->setScale(scale);
			coin->setPosition(origin + origin2);
			this->addChild(coin, ZORDER_ENEMY);
			coin->initCirclePhysic(world, origin);
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
			auto scale = SCREEN_SIZE.height / 20 / coin->getContentSize().height;
			coin->setScale(scale);
			coin->setPosition(origin + origin2);
			this->addChild(coin, ZORDER_ENEMY);
			coin->initCirclePhysic(world, origin);
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
			auto scale = SCREEN_SIZE.height / 20 / coin->getContentSize().height;
			coin->setScale(scale);
			coin->setPosition(origin+origin2);
			this->addChild(coin, ZORDER_ENEMY);
			coin->initCirclePhysic(world, origin);
			coin->runAnimation();
		}
	}
}

//void GameScene::initBoxPhysic(b2World * world, Point pos, Size size)

void GameScene::danceWithCamera()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	follow = Node::create();
	follow->setPosition(origin + SCREEN_SIZE / 2);
	addChild(follow);

	camera = Follow::create(follow);
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
	log("doi tuong thu nhat:%d", heroJsonFile["hero"][0]["level"].GetInt());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	heroJsonFile.Accept(writer);
	const char* output = buffer.GetString();

	FileUtils::getInstance()->writeStringToFile(output, "Hero.json");
}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (left_corner.containsPoint(touch->getLocation())) {
		if (hero->getNumberOfJump() > 0) {
			hero->setNumberOfJump(hero->getNumberOfJump() - 1);
			hero->setOnGround(false);

			hero->getBody()->SetLinearVelocity(b2Vec2(0.0f, hero->getJumpVel()));
			
			hero->getCurrentState()->jump(hero);
		}
		
	}


	/*updateEnemy();
	follow->setPosition(follow->getPositionX() + 2, follow->getPositionY() );*/

	return false;

}

void GameScene::updateEnemy()
{
	for (auto child : this->getChildren()) {
		if (child->getTag() > 100) {
			auto tmp = (B2Skeleton*) child;
			tmp->update(1.0f);
		}
	}
}


void GameScene::cachePlist()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
}
