#include "EnemyWooder.h"

EnemyWooder::EnemyWooder() : BaseEnemy()
{
	
}

EnemyWooder::~EnemyWooder()
{
}

EnemyWooder::EnemyWooder(spSkeletonData * data) : BaseEnemy(data)
{
	//BaseEnemy:BaseEnemy(data);
}

EnemyWooder::EnemyWooder(string jsonFile, string atlasFile, float scale) : BaseEnemy(jsonFile, atlasFile, scale)
{
}

EnemyWooder * EnemyWooder::create(string jsonFile, string atlasFile, float scale)
{
	EnemyWooder *enemy = new EnemyWooder(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_WOODER);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;

}

EnemyWooder * EnemyWooder::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyWooder(data);
	//enemy->initWithData(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_WOODER);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
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

	BaseEnemy::die();

	auto world = this->body->GetWorld();
	world->DestroyBody(this->body);
	this->body = nullptr;
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	this->clearTracks();
	this->setAnimation(0, "broken", false);
	this->setToSetupPose();
}

void EnemyWooder::updateMe(BaseHero* hero)
{
	BaseEnemy::updateMe(hero);
	if (this->getIsDie()&& this->getB2Body()!= nullptr) {
		this->die();
	}

}

void EnemyWooder::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "broken") == 0 && loopCount == 1) {
//			this->removeFromParentAndCleanup(false);
            this->setVisible(false);
            this->setIsDie(false);
            this->clearTracks();
            this->setToSetupPose();
		}

	});
}
