#include "EnemyChong3.h"
#include "BaseHero.h"
#include "manager/AudioManager.h"

EnemyChong3::EnemyChong3() : EnemyChong2()
{

}

EnemyChong3::~EnemyChong3()
{
}

EnemyChong3::EnemyChong3(spSkeletonData * data) : EnemyChong2(data)
{
	//EnemyChong2:EnemyChong2(data);
}

EnemyChong3::EnemyChong3(string jsonFile, string atlasFile, float scale) : EnemyChong2(jsonFile, atlasFile, scale)
{
}

EnemyChong3 * EnemyChong3::create(string jsonFile, string atlasFile, float scale)
{
	EnemyChong3 *enemy = new EnemyChong3(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_CHONG3);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;

}

EnemyChong3 * EnemyChong3::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyChong3(data);
	enemy->setAnimation(0, "idle", true);
	//enemy->initWithData(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_CHONG3);
	//enemy->setScaleX(1);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;


}



