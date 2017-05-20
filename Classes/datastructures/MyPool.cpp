#include "MyPool.h"
#include "Global.h"
#include "coin\Coin.h"
#include "coin\CoinBag.h"
#include "coin\CoinBullion.h"



MyPool::MyPool()
{
	indexOfPool = 0;
}

MyPool::~MyPool()
{
	delete[] pool;

}

MyPool::MyPool(int maxPool, int tag) : MyPool()
{
	this->maxPool = maxPool;
	this->tag = tag;
	switch (tag)
	{

	case TAG_ENEMY_WOODER: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfWooder = (SCREEN_SIZE.height / 3.5) / 490; // 490 la height cua spine
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
	case TAG_ENEMY_CHONG1: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 6.0f / 200;
			auto enemy = EnemyChong1::create("Animation/EnemyChong1/BayChong1", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_CHONG2: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.0f / (205*1.5f);
			auto enemy = EnemyChong2::create("Animation/EnemyChong2/BayChong2", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_CHONG3: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 3.0f / (205*2);
			auto enemy = EnemyChong3::create("Animation/EnemyChong3/BayChong3", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_LINHCAMGIAO1: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.0f / (406);
			auto enemy = EnemyLinhCamGiao::create("Animation/Enemy_LinhCamGiao/LinhCamGiao", scaleOfEnemy);
			//coin->runAnimation();

			enemy->setTag(tag);
			pool[i] = enemy;
		}
		break;
	}
	case TAG_ENEMY_LINHCAMGIAO2: {

		Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
		pool = new Ref*[maxPool];

		for (int i = 0; i < maxPool; i++) {
			auto scaleOfEnemy = SCREEN_SIZE.height / 4.0f / (406);
			auto enemy = EnemyLinhCamGiao2::create("Animation/Enemy_LinhCamGiao/LinhCamGiao", scaleOfEnemy);
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
