#include "MyLayer.h"
#include "Global.h"
#include "coin\Coin.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"

bool MyLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	wooderPool = MyPool::create(5, TAG_ENEMY_WOODER);
	toanchan1Pool = MyPool::create(5, TAG_ENEMY_TOANCHAN1);
	toanchan2Pool = MyPool::create(5, TAG_ENEMY_TOANCHAN2);

	for (int i = 0; i < wooderPool->maxPool; i++) {
		auto tmp = (EnemyWooder*)wooderPool->getObject();
		tmp->setPosition(0, -SCREEN_SIZE.height);
		tmp->setVisible(false);
		tmp->pauseSchedulerAndActions();
		this->addChild(tmp);
	}
	for (int i = 0; i < toanchan1Pool->maxPool; i++) {
		auto tmp = (EnemyToanChanStudent*)toanchan1Pool->getObject();
		tmp->setPosition(0, -SCREEN_SIZE.height);
		tmp->setVisible(false);
		tmp->pauseSchedulerAndActions();
		this->addChild(tmp);
	}
	for (int i = 0; i < toanchan2Pool->maxPool; i++) {
		auto tmp = (EnemyToanChanStudent2*)toanchan2Pool->getObject();
		tmp->setPosition(0, -SCREEN_SIZE.height);
		tmp->setVisible(false);
		tmp->pauseSchedulerAndActions();
		this->addChild(tmp);
		tmp->genSlash();
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


