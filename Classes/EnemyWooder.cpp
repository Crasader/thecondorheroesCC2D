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
//	auto world = this->body->GetWorld();
	//world->DestroyBody(this->body);
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	//this->setTimeScale(0.05f);
	this->clearTracks();
	this->addAnimation(0,"broken",false);
	this->setToSetupPose();
}

