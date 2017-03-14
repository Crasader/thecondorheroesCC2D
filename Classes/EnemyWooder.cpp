#include "EnemyWooder.h"

EnemyWooder::EnemyWooder(spSkeletonData * data) :BaseEnemy(data)
{
}

EnemyWooder::EnemyWooder(string jsonFile, string atlasFile, float scale):BaseEnemy(jsonFile, atlasFile,scale)
{
}

EnemyWooder * EnemyWooder::create(string jsonFile, string atlasFile, float scale)
{
	EnemyWooder *enemy = new EnemyWooder(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_WOODER);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	return enemy;

}

EnemyWooder * EnemyWooder::create(spSkeletonData * data)
{
	EnemyWooder *enemy = new EnemyWooder(data);
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
	//auto world = this->body->GetWorld();
	//world->DestroyBody(this->body);
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	this->clearTracks();
	this->addAnimation(0,"broken",false);
	this->setToSetupPose();
}

void EnemyWooder::updateMe(float dt)
{
	BaseEnemy::updateMe(dt);
	
}

void EnemyWooder::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "broken") == 0 && loopCount == 1) {
			//getSplash()->setVisible(false);
			//setIsAttacking(false);
			this->removeFromParentAndCleanup(true);
		}

	});
}
