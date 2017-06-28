#include "EnemyLinhCamGiao.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


EnemyLinhCamGiao::EnemyLinhCamGiao(spSkeletonData * data) :EnemyHoacDo(data)
{
}

EnemyLinhCamGiao::EnemyLinhCamGiao(string jsonFile, string atlasFile, float scale) : EnemyHoacDo(jsonFile, atlasFile, scale)
{
}

EnemyLinhCamGiao * EnemyLinhCamGiao::create(string jsonFile, string atlasFile, float scale)
{
	EnemyLinhCamGiao *enemy = new EnemyLinhCamGiao(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 12;
	enemy->autorelease();
	return enemy;

}


EnemyLinhCamGiao * EnemyLinhCamGiao::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhCamGiao(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 12;
	enemy->autorelease();
	return enemy;
}

void EnemyLinhCamGiao::run()
{

}

void EnemyLinhCamGiao::updateMe(BaseHero * hero)
{
	//log("linh cam giao %d", getTag());
	BaseEnemy::updateMe(hero);
	if (getIsDie() && this->getB2Body() != nullptr) {
		die();
	}
}

void EnemyLinhCamGiao::initBoxPhysic(b2World * world, Point pos)
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

	fixtureDef.filter.maskBits = BITMASK_SWORD | BITMASK_FLOOR | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	this->addSquareFixture(Size(size.width / 4, size.height / 8 / PTM_RATIO),
		Point(-size.width / 3, 0), BITMASK_ENEMY, BITMASK_HERO);
}

void EnemyLinhCamGiao::playsoundAt()
{
	AudioManager::playSound(SOUND_GIAOAT);
}

void EnemyLinhCamGiao::playsoundDie()
{
	AudioManager::playSound(SOUND_GIAODIE);
}

void EnemyLinhCamGiao::addSquareFixture(Size size, Vec2 pos, unsigned int category, unsigned int mask)
{
	b2PolygonShape polygonShape;
	b2FixtureDef  myFixtureDef;

	//add foot sensor fixture
	polygonShape.SetAsBox(size.width / PTM_RATIO, size.height / PTM_RATIO, b2Vec2(pos.x / PTM_RATIO, pos.y / PTM_RATIO), 0);
	myFixtureDef.shape = &polygonShape;
	myFixtureDef.isSensor = true;
	myFixtureDef.filter.categoryBits = category;
	myFixtureDef.filter.maskBits = mask;
	b2Fixture* footSensorFixture = body->CreateFixture(&myFixtureDef);
	footSensorFixture->SetUserData(this);
}

void EnemyLinhCamGiao::prepare()
{
	BaseEnemy::prepare();
}

void EnemyLinhCamGiao::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1) {
			this->clearTracks();
			this->addAnimation(0, "idle", true);
			this->setToSetupPose();
		}

		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			this->setVisible(false);
			this->pauseSchedulerAndActions();
		}

	});
}


//void EnemyLinhCamGiao::genSplash()
//{
//	splash = Sprite::create("Animation/Enemy-DeTuToanChan1/slashenemy.png");
//	splash->setScale(SCREEN_SIZE.height/5/splash->getContentSize().height);
//	splash->setAnchorPoint(Point(1, 0));
//	
//	splash->setPosition(-this->getBody()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO, 0);
//	splash->setVisible(false);
//	this->addChild(splash);
//}

