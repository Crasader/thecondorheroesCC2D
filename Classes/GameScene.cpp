#include "GameScene.h"
#include "SimpleAudioEngine.h"


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

	auto bg = Sprite::create("map1/bg.jpg");
	bg->setScaleX(visibleSize.width / bg->getContentSize().width);
	bg->setScaleY(visibleSize.height / bg->getContentSize().height);
	bg->setPosition(origin + visibleSize / 2);
	addChild(bg);


	createDuongQua("duong_qua/DuongQua.json", "duong_qua/DuongQua.atlas", visibleSize / 2);

	this->scheduleUpdate();

    return true;
}


void GameScene::createDuongQua(string path_Json, string path_Atlas, Point position)
{
	hero = DuongQua::create(path_Json, path_Atlas, 0.5f);
	hero->setPosition(position);

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
	listener();
	hero->update();
}
