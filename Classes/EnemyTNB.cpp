#include "EnemyTNB.h"

EnemyTNB::EnemyTNB() : BaseEnemy()
{
	
}

EnemyTNB::~EnemyTNB()
{
}

EnemyTNB::EnemyTNB(spSkeletonData * data) : BaseEnemy(data)
{
	//BaseEnemy:BaseEnemy(data);
}

EnemyTNB::EnemyTNB(string jsonFile, string atlasFile, float scale) : BaseEnemy(jsonFile, atlasFile, scale)
{
}

EnemyTNB * EnemyTNB::create(string jsonFile, string atlasFile, float scale)
{
	EnemyTNB *enemy = new EnemyTNB(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TNB);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;

}

EnemyTNB * EnemyTNB::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyTNB(data);
	enemy->setAnimation(0, "idle", true);
	//enemy->initWithData(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TNB);
	//enemy->setScaleX(1);
	//enemy->setTimeScale(1.4f);
	
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;


}


void EnemyTNB::run()
{
}

void EnemyTNB::attack()
{
}

void EnemyTNB::die()
{
	BaseEnemy::die();

	auto world = this->body->GetWorld();
	world->DestroyBody(this->body);
	this->body = nullptr;
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	this->clearTracks();
	this->setAnimation(0, "die", false);
	this->setToSetupPose();
}

void EnemyTNB::updateMe(BaseHero* hero)
{
	//BaseEnemy::updateMe(hero);
	if (this->getIsDie() && this->getB2Body() != nullptr) {
		this->die();
	}

}

void EnemyTNB::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			//this->removeFromParentAndCleanup(false);
		}
		if (strcmp(getCurrent()->animation->name, "appear") == 0 && loopCount == 1) {
			//this->removeFromParentAndCleanup(false);
		}
	});
}
