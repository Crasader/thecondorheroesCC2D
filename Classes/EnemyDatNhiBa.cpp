#include "EnemyDatNhiBa.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


EnemyDatNhiBa::EnemyDatNhiBa(spSkeletonData * data):EnemyToanChanStudent(data)
{
	canRun = false;
}

EnemyDatNhiBa::EnemyDatNhiBa(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
	canRun = false;
}

EnemyDatNhiBa * EnemyDatNhiBa::create(string jsonFile, string atlasFile, float scale)
{
	EnemyDatNhiBa *enemy = new EnemyDatNhiBa(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_DATNHIBA1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 2;
	enemy->setDamage(1);
	enemy->exp = 12;
	return enemy;

}


EnemyDatNhiBa * EnemyDatNhiBa::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyDatNhiBa(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_DATNHIBA1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 2;
	enemy->setDamage(1);
	enemy->exp = 12;
	return enemy;
}


void EnemyDatNhiBa::hit()
{
	
	 health--;
	 
	 if (health == 0) {
		 this->setIsDie(true);
	 }
	 else {
		 this->addAnimation(10, "injured-red", false);
		 canRun = true;
	 }
}

void EnemyDatNhiBa::updateMe(BaseHero* hero)
{
	EnemyToanChanStudent::updateMe(hero);
	if (canRun&& body != nullptr) {
		run();
	}
}

void EnemyDatNhiBa::run()
{
	body->SetLinearVelocity(b2Vec2(SCREEN_SIZE.width/PTM_RATIO, 0));
	body->SetLinearDamping(SCREEN_SIZE.width/PTM_RATIO / 4);
	canRun = false;
	this->changeBodyMaskBits(BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	this->schedule([&](float dt) {
		if (this->getB2Body() != nullptr) {
			this->changeBodyMaskBits((BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2));
		}
		this->unschedule("datmaAttack");
	}, 0.5f, "datmaAttack");
}

void EnemyDatNhiBa::playsoundAt()
{
	AudioManager::playSound(SOUND_HD1AT);
}

void EnemyDatNhiBa::playsoundDie()
{
	AudioManager::playSound(SOUND_HD1DIE);
}



void EnemyDatNhiBa::initCirclePhysic(b2World * world, Point pos)
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
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0;
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

