#include "MyLayer.h"
#include "Global.h"
#include "Coin.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"
#include "EnemyToOng.h"

MyLayer::MyLayer()
{
    wooderPool = nullptr;
    toanchan1Pool = nullptr;
    toanchan2Pool = nullptr;
    toOngPool = nullptr;
    hongLangBa1Pool = nullptr;
    hongLangBa2Pool = nullptr;
}

bool MyLayer::init(TMXTiledMap* tmx_map)
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    if (tmx_map->getObjectGroup("wooder")) {
        wooderPool = MyPool::create(5, TAG_ENEMY_WOODER);
        for (int i = 0; i < wooderPool->maxPool; i++) {
            auto tmp = (EnemyWooder*)wooderPool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
        }
    }
    if (tmx_map->getObjectGroup("toanchan_student")) {
        toanchan1Pool = MyPool::create(5, TAG_ENEMY_TOANCHAN1);
        for (int i = 0; i < toanchan1Pool->maxPool; i++) {
            auto tmp = (EnemyToanChanStudent*)toanchan1Pool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
        }
    }
    if (tmx_map->getObjectGroup("toanchan_student2")) {
        toanchan2Pool = MyPool::create(5, TAG_ENEMY_TOANCHAN2);
        for (int i = 0; i < toanchan2Pool->maxPool; i++) {
            auto tmp = (EnemyToanChanStudent2*)toanchan2Pool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
            tmp->genSlash();
        }
    }
    if (tmx_map->getObjectGroup("bee")) {
        toOngPool = MyPool::create(5, TAG_ENEMY_TOONG);
        for (int i = 0; i < toOngPool->maxPool; i++) {
            auto tmp = (EnemyToOng*)toOngPool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
            //tmp->genSlash();
        }
    }
    if (tmx_map->getObjectGroup("lms_student lv1")) {
        hongLangBa1Pool = MyPool::create(5, TAG_ENEMY_HONGLANGBA1);
        for (int i = 0; i < hongLangBa1Pool->maxPool; i++) {
            auto tmp = (EnemyHongLangBa*)hongLangBa1Pool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
            //tmp->genSlash();
        }
    }
    if (tmx_map->getObjectGroup("lms_student lv2")) {
        hongLangBa2Pool = MyPool::create(5, TAG_ENEMY_HONGLANGBA2);
        for (int i = 0; i < hongLangBa2Pool->maxPool; i++) {
            auto tmp = (EnemyHongLangBa2*)hongLangBa2Pool->getObject();
            tmp->setPosition(0, -SCREEN_SIZE.height);
            tmp->setVisible(false);
            tmp->pauseSchedulerAndActions();
            this->addChild(tmp);
            tmp->genDarts();
        }
    }
    
    return true;
}

void MyLayer::refresh()
{
    for (auto i : this->getChildren()) {
        if (i->isVisible()) {
            i->setVisible(false);
            i->pauseSchedulerAndActions();
        }
    }
}

MyLayer * MyLayer::create(TMXTiledMap * tmx_map)
{
    MyLayer *pRet = new(std::nothrow) MyLayer();
    if (pRet && pRet->init(tmx_map))
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

void MyLayer::cleanup()
{
    delete wooderPool;
    delete toanchan1Pool;
    delete toanchan2Pool;
    delete toOngPool;
    delete hongLangBa1Pool;
    delete hongLangBa2Pool;
}
