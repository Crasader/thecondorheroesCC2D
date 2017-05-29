#include "MyLayer.h"
#include "Global.h"
#include "coin/Coin.h"


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
	datNhiBa1Pool = nullptr;
	datNhiBa2Pool = nullptr;
	chong1Pool = nullptr;
	chong2Pool = nullptr;
	chong3Pool = nullptr;
	linhcamgiao1Pool = nullptr;
	linhcamgiao2Pool = nullptr;
	linhtenthangPool = nullptr;
	linhtenxienPool = nullptr;
	linhcamroiPool = nullptr;
}
MyLayer::~MyLayer() {
	delete wooderPool;
	delete toanchan1Pool;
	delete toanchan2Pool;
	delete toOngPool;
	delete hongLangBa2Pool;
	delete tnbPool;
	delete datNhiBa1Pool;
	delete datNhiBa2Pool;
	delete linhcamgiao1Pool;
	delete	linhcamgiao2Pool;
	delete	linhtenthangPool;
	delete	linhtenxienPool;
	delete	linhcamroiPool;
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
	if (tmx_map->getObjectGroup("datnhiba_1")) {
		datNhiBa1Pool = MyPool::create(max, TAG_ENEMY_DATNHIBA1);
		for (int i = 0; i < datNhiBa1Pool->maxPool; i++) {
			auto tmp = (EnemyDatNhiBa*)datNhiBa1Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("datnhiba_2")) {
		datNhiBa2Pool = MyPool::create(max, TAG_ENEMY_DATNHIBA2);
		for (int i = 0; i < datNhiBa2Pool->maxPool; i++) {
			auto tmp = (EnemyDatNhiBa2*)datNhiBa2Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("chonggo")) {
		chong1Pool = MyPool::create(max, TAG_ENEMY_CHONG1);
		for (int i = 0; i < chong1Pool->maxPool; i++) {
			auto tmp = (EnemyChong1*)chong1Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("chonggo2")) {
		chong2Pool = MyPool::create(max, TAG_ENEMY_CHONG2);
		for (int i = 0; i < chong2Pool->maxPool; i++) {
			auto tmp = (EnemyChong2*)chong2Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("chonggo3")) {
		chong3Pool = MyPool::create(max, TAG_ENEMY_CHONG3);
		for (int i = 0; i < chong3Pool->maxPool; i++) {
			auto tmp = (EnemyChong3*)chong3Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}

	if (tmx_map->getObjectGroup("linhcamgiao1")) {
		linhcamgiao1Pool = MyPool::create(max, TAG_ENEMY_LINHCAMGIAO1);
		for (int i = 0; i < linhcamgiao1Pool->maxPool; i++) {
			auto tmp = (EnemyLinhCamGiao*)linhcamgiao1Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}

	if (tmx_map->getObjectGroup("linhcamgiao2")) {
		linhcamgiao2Pool = MyPool::create(max, TAG_ENEMY_LINHCAMGIAO2);
		for (int i = 0; i < linhcamgiao2Pool->maxPool; i++) {
			auto tmp = (EnemyLinhCamGiao2*)linhcamgiao2Pool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
		}
	}
	if (tmx_map->getObjectGroup("linhtenthang")) {
		linhtenthangPool = MyPool::create(max, TAG_ENEMY_LINH_TEN_THANG);
		for (int i = 0; i < linhtenthangPool->maxPool; i++) {
			auto tmp = (EnemyLinhTenThang*)linhtenthangPool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
			tmp->genSlash();
		}
	}
	if (tmx_map->getObjectGroup("linhtenxien")) {
		linhtenxienPool = MyPool::create(max, TAG_ENEMY_LINH_TEN_XIEN);
		for (int i = 0; i < linhtenxienPool->maxPool; i++) {
			auto tmp = (EnemyLinhTenXien*)linhtenxienPool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
			tmp->genSlash();
		}
	}
	if (tmx_map->getObjectGroup("linhcamroi")) {
		linhcamroiPool = MyPool::create(max, TAG_ENEMY_LINHCAMROI);
		for (int i = 0; i < linhcamroiPool->maxPool; i++) {
			auto tmp = (EnemyLinhCamRoi*)linhcamroiPool->getObject();
			tmp->setPosition(0, -SCREEN_SIZE.height);
			tmp->setVisible(false);
			tmp->pauseSchedulerAndActions();
			this->addChild(tmp);
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