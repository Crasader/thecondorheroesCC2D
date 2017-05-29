#include "EnemyChong1.h"
#include "manager/AudioManager.h"
#include "BaseHero.h"

EnemyChong1::EnemyChong1() : EnemyTNB()
{
	
}

EnemyChong1::~EnemyChong1()
{
}

EnemyChong1::EnemyChong1(spSkeletonData * data) : EnemyTNB(data)
{
	//EnemyTNB:EnemyTNB(data);
}

EnemyChong1::EnemyChong1(string jsonFile, string atlasFile, float scale) : EnemyTNB(jsonFile, atlasFile, scale)
{
}

EnemyChong1 * EnemyChong1::create(string jsonFile, string atlasFile, float scale)
{
	EnemyChong1 *enemy = new EnemyChong1(jsonFile, atlasFile, scale);
	enemy->setAnimation(0, "idle", true);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TNB);
	enemy->setScaleX(1);
	enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	enemy->autorelease();
	return enemy;

}

EnemyChong1 * EnemyChong1::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyChong1(data);
	enemy->setAnimation(0, "idle", true);
	//enemy->initWithData(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_CHONG1);
	//enemy->setScaleX(1);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 5;
	enemy->autorelease();
	return enemy;


}


void EnemyChong1::die()
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

void EnemyChong1::updateMe(BaseHero* hero)
{
	if (!isEndOfScreen && this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() - SCREEN_SIZE.width * 0.25f) {
		isEndOfScreen = true;
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

void EnemyChong1::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			//this->removeFromParentAndCleanup(false);
			this->setVisible(false);
			/*this->clearTracks();
			this->setAnimation(0, "idle", true);
			this->setToSetupPose();*/
			this->pauseSchedulerAndActions();
		}
		if (strcmp(getCurrent()->animation->name, "appear") == 0 && loopCount == 1) {
			//this->removeFromParentAndCleanup(false);
		}
	});
}

void EnemyChong1::initBoxPhysic(b2World * world, Point pos)
{
	b2PolygonShape shape;
	auto size = this->getBoundingBox().size;

	shape.SetAsBox(size.width / 4 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;



	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(0);
}
