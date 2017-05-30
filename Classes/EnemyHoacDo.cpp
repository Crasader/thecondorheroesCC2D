#include "EnemyHoacDo.h"
#include "SkeletonManager.h"
#include "AudioManager.h"


EnemyHoacDo::EnemyHoacDo(spSkeletonData * data):EnemyToanChanStudent(data)
{
}

EnemyHoacDo::EnemyHoacDo(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
}

EnemyHoacDo * EnemyHoacDo::create(string jsonFile, string atlasFile, float scale)
{
	EnemyHoacDo *enemy = new EnemyHoacDo(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_HOACDO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;

}


EnemyHoacDo * EnemyHoacDo::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyHoacDo(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_HOACDO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;
}

void EnemyHoacDo::playsoundAt()
{
	AudioManager::playSound(SOUND_HD1AT);
}

void EnemyHoacDo::playsoundDie()
{
	AudioManager::playSound(SOUND_HD1DIE);
}



void EnemyHoacDo::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}




//void EnemyToanChanStudent::genSplash()
//{
//	splash = Sprite::create("Animation/Enemy-DeTuToanChan1/slashenemy.png");
//	splash->setScale(SCREEN_SIZE.height/5/splash->getContentSize().height);
//	splash->setAnchorPoint(Point(1, 0));
//	
//	splash->setPosition(-this->getBody()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO, 0);
//	splash->setVisible(false);
//	this->addChild(splash);
//}

