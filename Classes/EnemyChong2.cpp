#include "EnemyChong2.h"
#include "BaseHero.h"
#include "manager/AudioManager.h"

EnemyChong2::EnemyChong2() : EnemyChong1()
{
	
}

EnemyChong2::~EnemyChong2()
{
}

EnemyChong2::EnemyChong2(spSkeletonData * data) : EnemyChong1(data)
{
	//EnemyChong1:EnemyChong1(data);
}

EnemyChong2::EnemyChong2(string jsonFile, string atlasFile, float scale) : EnemyChong1(jsonFile, atlasFile, scale)
{
}

EnemyChong2 * EnemyChong2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyChong2 *enemy = new EnemyChong2(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_CHONG2);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;

}

EnemyChong2 * EnemyChong2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyChong2(data);
	enemy->setAnimation(0, "idle", true);
	//enemy->initWithData(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_CHONG2);
	//enemy->setScaleX(1);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	return enemy;


}


void EnemyChong2::die()
{
	BaseEnemy::die();
	AudioManager::playSound(SOUND_TNBDIE);
	auto world = this->body->GetWorld();
	world->DestroyBody(this->body);
	this->body = nullptr;
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	this->clearTracks();
	this->setAnimation(0, "die", false);
	this->setToSetupPose();
}

void EnemyChong2::updateMe(BaseHero* hero)
{
	if (!isEndOfScreen && this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() - SCREEN_SIZE.width * 0.25f) {
		isEndOfScreen = true;
	}

	if (this->getB2Body()) {
		if (this->getPositionX() + this->getParent()->getPositionX() - hero->getPositionX() < SCREEN_SIZE.width / 2) {
			this->resume();
		}
	}

	if (hero->getIsKillAll() && this->getB2Body() != nullptr) {
		if (this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.75f &&
			this->getPositionX() + this->getParent()->getPositionX() > hero->getPositionX() - SCREEN_SIZE.width * 0.26f &&
			hero->getPositionY() + SCREEN_SIZE.height * 0.5f > this->getPositionY() &&
			hero->getPositionY() - SCREEN_SIZE.height * 0.35f < this->getPositionY()

			) {

			die();
		}
	}

	if (this->getIsDie() && this->getB2Body() != nullptr) {
		this->die();
	}

}

void EnemyChong2::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "appear") == 0 && loopCount == 1) {
			//this->removeFromParentAndCleanup(false);
			this->addAnimation(0, "idle", true);
		}
		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			this->setVisible(false);
			this->pauseSchedulerAndActions();
		}
	});
}