#include "MyPool.h"
#include "Global.h"
#include "coin\Coin.h"
#include "coin\CoinBag.h"
#include "coin\CoinBullion.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"
#include "EnemyWooder.h"
#include "EnemyTNB.h"
#include "EnemyToOng.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"
#include "EnemyHoacDo.h"
#include "EnemyHoacDo2.h"
#include "EnemyDatNhiBa.h"
#include "EnemyDatNhiBa2.h"



MyPool::MyPool()
{
	indexOfPool = 0;
}

MyPool::~MyPool()
{
	/*for (int i = 0; i < maxPool; i++) {
	if(pool[i]!= nullptr)
	delete pool[i];
	}*/
	delete[] pool;

}

MyPool::MyPool(int maxPool, int tag) : MyPool()
{
	this->maxPool = maxPool;
	this->tag = tag;
	switch (tag)
	{
		//case TAG_COIN:
		//{
		//	pool = new Ref*[maxPool];

		//	for (int i = 0; i < maxPool; i++) {
		//		auto coin = Coin::create();
		//		auto scale = Director::getInstance()->getVisibleSize().height * 0.075 / coin->getContentSize().height;
		//		coin->setScale(scale);
		//		coin->setTag(tag);
		//		pool[i] = coin;
		//	}
		//	//log("%d", pool->count());
		//	break;
		//}

	case TAG_ENEMY_WOODER: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
																   //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
																   //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
			auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan", scaleOfWooder);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_TOANCHAN1: {
		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
																 /*auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1.json",
																 "Animation/Enemy_DeTuToanChan1/ToanChan1.atlas", scaleOfEnemy);*/
			auto enemy = EnemyToanChanStudent::create("Animation/Enemy_DeTuToanChan1/ToanChan1", scaleOfEnemy);
			//auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_TOANCHAN2: {
		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 401 la height cua spine
																 /*auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2.json",
																 "Animation/Enemy_DeTuToanChan2/ToanChan2.atlas", scaleOfEnemy);*/
			auto enemy = EnemyToanChanStudent2::create("Animation/Enemy_DeTuToanChan2/ToanChan2", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}

	case TAG_ENEMY_TNB:
	{
		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfTNB = (SCREEN_SIZE.height / 3.5) / 300;
			auto enemy = EnemyTNB::create("Animation/Enemy_TNB/TNB", scaleOfTNB);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_TOONG: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 490 la height cua spine
																   //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
																   //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
			auto enemy = EnemyToOng::create("Animation/Enemy_ToOng/toong", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_HONGLANGBA1: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401;// 490 la height cua spine
																  //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
																  //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
			auto enemy = EnemyHongLangBa::create("Animation/Enemy_HongLangBa/HLBa1", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_HONGLANGBA2: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 401; // 490 la height cua spine
																  //auto enemy = EnemyWooder::create("Animation/Enemy_MocNhan/MocNhan.json",
																  //	"Animation/Enemy_MocNhan/MocNhan.atlas", scaleOfWooder);
			auto enemy = EnemyHongLangBa2::create("Animation/Enemy_HongLangBa2/HLBa2", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}

	case TAG_ENEMY_HOACDO1: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 406; 
			auto enemy = EnemyHoacDo::create("Animation/Enemy_HoacDo1/HoacDo1", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}

	case TAG_ENEMY_HOACDO2: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.5f / 406;
			auto enemy = EnemyHoacDo2::create("Animation/Enemy_HoacDo2/HoacDo2", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_DATNHIBA1: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.0f / 406;
			auto enemy = EnemyDatNhiBa::create("Animation/Enemy_DatNhiBa/DatNhiBa1", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_DATNHIBA2: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.0f / 406;
			auto enemy = EnemyDatNhiBa2::create("Animation/Enemy_DatNhiBa/DatNhiBa2", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	default:
		break;
	}
}

MyPool * MyPool::create(int maxPool, int tag)
{
	auto pool = new MyPool(maxPool, tag);
	return pool;
}

Ref * MyPool::getObject()
{
	auto object = pool[indexOfPool];
	// log("getObject: %d", indexOfPool);
	indexOfPool++;
	indexOfPool = indexOfPool%maxPool;
	return object;
}
