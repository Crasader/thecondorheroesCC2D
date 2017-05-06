#include "MyLayer.h"
#include "Global.h"
#include "coin/Coin.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"
#include "EnemyToOng.h"
#include "EnemyTNB.h"
#include "EnemyHoacDo.h"
#include "EnemyHoacDo2.h"

MyLayer::MyLayer()
{
	 wooderPool = nullptr;
	 toanchan1Pool = nullptr;
	 toanchan2Pool = nullptr;
	 toOngPool = nullptr;
	 hongLangBa1Pool = nullptr;
	 hongLangBa2Pool = nullptr;
	 tnbPool = nullptr;
	 hoacDo1Pool = nullptr;
}
MyLayer::~MyLayer(){
	if(wooderPool!= nullptr)
		delete wooderPool;
	if(toanchan1Pool!=nullptr)
		delete toanchan1Pool;
	if(toanchan2Pool)
		delete toanchan2Pool;
	if(toOngPool!=nullptr)
		delete toOngPool;
	if(hongLangBa1Pool!=nullptr)
		delete hongLangBa1Pool;
	if(hongLangBa2Pool != nullptr)
		delete hongLangBa2Pool;
}
bool MyLayer::init(TMXTiledMap* tmx_map)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	int max = 5;
	if (tmx_map->getObjectGroup("wooder")) {
		wooderPool = MyPool::create(max, TAG_ENEMY_WOODER);
		for (int i = 0; i < wooderPool->maxPool; i++) {
			auto tmp = (EnemyWooder*)wooderPool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("toanchan_student")) {
		toanchan1Pool = MyPool::create(max, TAG_ENEMY_TOANCHAN1);
		for (int i = 0; i < toanchan1Pool->maxPool; i++) {
			auto tmp = (EnemyToanChanStudent*)toanchan1Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("toanchan_student2")) {
		toanchan2Pool = MyPool::create(max, TAG_ENEMY_TOANCHAN2);
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
		toOngPool = MyPool::create(max, TAG_ENEMY_TOONG);
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
		hongLangBa1Pool = MyPool::create(max, TAG_ENEMY_HONGLANGBA1);
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
		hongLangBa2Pool = MyPool::create(max, TAG_ENEMY_HONGLANGBA2);
		for (int i = 0; i < hongLangBa2Pool->maxPool; i++) {
			auto tmp = (EnemyHongLangBa2*)hongLangBa2Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
			tmp->genDarts();
		}
	}
	if (tmx_map->getObjectGroup("tnb")) {
		tnbPool = MyPool::create(max, TAG_ENEMY_TNB);
		for (int i = 0; i < tnbPool->maxPool; i++) {
			auto tmp = (EnemyTNB*)tnbPool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}

	if (tmx_map->getObjectGroup("hoacdo_1")) {
		hoacDo1Pool = MyPool::create(max, TAG_ENEMY_HOACDO1);
		for (int i = 0; i < hoacDo1Pool->maxPool; i++) {
			auto tmp = (EnemyHoacDo*)hoacDo1Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}

	if (tmx_map->getObjectGroup("hoacdo_2")) {
		hoacDo2Pool = MyPool::create(max, TAG_ENEMY_HOACDO2);
		for (int i = 0; i < hoacDo2Pool->maxPool; i++) {
			auto tmp = (EnemyHoacDo2*)hoacDo2Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
			tmp->genSlash();
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




