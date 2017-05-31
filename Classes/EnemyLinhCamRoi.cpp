#include "EnemyLinhCamRoi.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"

EnemyLinhCamRoi::EnemyLinhCamRoi(spSkeletonData * data):EnemyLinhCamGiao(data)
{
}

EnemyLinhCamRoi * EnemyLinhCamRoi::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhCamRoi(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMROI);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 12;
	enemy->autorelease();
	return enemy;
}

void EnemyLinhCamRoi::playsoundAt()
{
}

void EnemyLinhCamRoi::playsoundDie()
{
}
