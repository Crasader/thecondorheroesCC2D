#include "EnemyWooder.h"

EnemyWooder::EnemyWooder(string jsonFile, string atlasFile, float scale):BaseEnemy(jsonFile, atlasFile,scale)
{
}

EnemyWooder * EnemyWooder::create(string jsonFile, string atlasFile, float scale)
{
	EnemyWooder *enemy = new EnemyWooder(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_WOODER);
	enemy->setScaleX(1);
	return enemy;

}

void EnemyWooder::run()
{
}

void EnemyWooder::attack()
{
}

void EnemyWooder::die()
{

}

void EnemyWooder::update(float dt)
{
	BaseEnemy::update(dt);
}
