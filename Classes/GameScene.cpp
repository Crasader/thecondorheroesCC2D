#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <string>

using namespace std;
USING_NS_CC;
using namespace rapidjson;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

	initB2World();
	loadBackground();
	createGroundBody();

	creatEnemyWooder();



  
	this->scheduleUpdate();
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
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
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDraw->SetFlags(flags);

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
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
		initBoxPhysic(world,pos,sizeOfBound);
	}
}

void GameScene::creatEnemyWooder()
{
	auto groupGround = tmx_map->getObjectGroup("wooder");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scaleOfMap, mObject["y"].asFloat()* scaleOfMap);
		//Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);
		//Point pos = Point(origin.x + sizeOfBound.width / 2, origin.y);
		//initPhysic(world, pos, sizeOfBound);
		auto scaleOfWooder = SCREEN_SIZE.height / 5 / 490; // 490 la height cua spine
		auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json", "Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
		enemy->setPosition(origin);
		//enemy->setVisible(false);
		this->addChild(enemy, ZORDER_ENEMY);
		enemy->initCirclePhysic(world, Point(origin.x, origin.y + enemy->getBoundingBox().size.height / 2));
		log("height of boundingbox,%f", enemy->getBoundingBox().size.height);
	}
}

void GameScene::initBoxPhysic(b2World * world, Point pos, Size size)
{
	b2Body * body;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	//auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	bodyDef.type = b2_staticBody;
	//bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

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

void GameScene::update(float dt)
{
	updateB2World(dt);

	updateEnemy();
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
