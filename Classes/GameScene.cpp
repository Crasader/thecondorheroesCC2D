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


	initB2World();
	loadBackground();
	createGroundBody();

	createDuongQua("duong_qua/DuongQua.json", "duong_qua/DuongQua.atlas", visibleSize / 2);

	this->scheduleUpdate();

    return true;
}


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, SCREEN_SIZE.height / 5 / 1024);
	hero->setPosition(position);
	hero->initCirclePhysic(world, hero->getPosition());
	addChild(hero);

}

void GameScene::listener()
{
	if (hud->getBtnAttack()->getIsActive()) {
		hero->getCurrentState()->jump(hero);
		hud->getBtnAttack()->setIsActive(false);
	}
}

void GameScene::update(float dt)
{
	updateB2World(dt);
	listener();
	hero->update();
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
	tmx_map = TMXTiledMap::create("map1/map.tmx");
	tmx_map->setAnchorPoint(Point::ZERO);
	scale = SCREEN_SIZE.height / tmx_map->getContentSize().height;
	tmx_map->setScale(scale);
	tmx_map->setPosition(Point::ZERO);
	tmx_map->setVisible(false);
	this->addChild(tmx_map, ZORDER_BG);
}

void GameScene::createGroundBody()
{
	auto groupGround = tmx_map->getObjectGroup("ground");
	for (auto child : groupGround->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() *scale, mObject["y"].asFloat()* scale);
		Size sizeOfBound = Size(mObject["width"].asFloat() *scale, mObject["height"].asFloat() *scale);
		Point pos = Point(origin.x + sizeOfBound.width / 2,origin.y);
		initPhysic(world,pos,sizeOfBound);
	}
}

void GameScene::initPhysic(b2World * world, Point pos, Size size)
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
