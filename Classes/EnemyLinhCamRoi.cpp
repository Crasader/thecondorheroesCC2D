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

void EnemyLinhCamRoi::initBoxPhysic(b2World * world, Point pos)
{
	b2PolygonShape shape;
	auto size = this->getBoundingBox().size;

	shape.SetAsBox(size.width / 8 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.filter.categoryBits = BITMASK_ENEMY;
	fixtureDef.filter.maskBits = BITMASK_SWORD | BITMASK_FLOOR;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	this->addSquareFixture(Size(size.width / 3, size.height / 8 / PTM_RATIO),
		Point(-size.width / 3, 0), BITMASK_ENEMY, BITMASK_HERO);
}

void EnemyLinhCamRoi::playsoundAt()
{
	AudioManager::playSound(SOUND_ROIAT);
}

void EnemyLinhCamRoi::playsoundDie()
{
	AudioManager::playSound(SOUND_ROIDIE);
}
