#include "EnemyDatNhiBa2.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


EnemyDatNhiBa2::EnemyDatNhiBa2(spSkeletonData * data):EnemyDatNhiBa(data)
{
	
}

EnemyDatNhiBa2::EnemyDatNhiBa2(string jsonFile, string atlasFile, float scale):EnemyDatNhiBa(jsonFile, atlasFile,scale)
{
	
}

EnemyDatNhiBa2 * EnemyDatNhiBa2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyDatNhiBa2 *enemy = new EnemyDatNhiBa2(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_DATNHIBA2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 2;
	enemy->setDamage(2);
	enemy->exp = 12;
	enemy->autorelease();
	return enemy;

}


EnemyDatNhiBa2 * EnemyDatNhiBa2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyDatNhiBa2(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_DATNHIBA2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 2;
	enemy->setDamage(2);
	enemy->exp = 12;
	enemy->autorelease();
	return enemy;
}

void EnemyDatNhiBa2::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height /3 / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}






