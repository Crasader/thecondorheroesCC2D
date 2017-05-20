#include "EnemyLinhCamGiao2.h"
#include "BaseHero.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


EnemyLinhCamGiao2::EnemyLinhCamGiao2(spSkeletonData * data) :EnemyLinhCamGiao(data)
{
}

EnemyLinhCamGiao2::EnemyLinhCamGiao2(string jsonFile, string atlasFile, float scale) : EnemyLinhCamGiao(jsonFile, atlasFile, scale)
{
}

EnemyLinhCamGiao2 * EnemyLinhCamGiao2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyLinhCamGiao2 *enemy = new EnemyLinhCamGiao2(jsonFile, atlasFile, scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;

}


EnemyLinhCamGiao2 * EnemyLinhCamGiao2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhCamGiao2(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;
}

void EnemyLinhCamGiao2::updateMe(BaseHero * hero)
{
	EnemyLinhCamGiao::updateMe(hero);
	if(this->getB2Body())
	if (hero->getPositionX() + SCREEN_SIZE.width * 2 / 3 > this->getPositionX() + this->getParent()->getPositionX()
		&& this->getB2Body()->GetType() == b2_staticBody) {
		this->run();
	}
}

void EnemyLinhCamGiao2::run()
{
	this->setAnimation(0, "run", true);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width/3/PTM_RATIO,0));
}

void EnemyLinhCamGiao2::initBoxPhysic(b2World * world, Point pos)
{
	b2PolygonShape shape;
	auto size = this->getBoundingBox().size;

	shape.SetAsBox(size.width / 8 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_SWORD|BITMASK_FLOOR;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	this->addSquareFixture(Size(size.width / 4, size.height / 8 / PTM_RATIO),
		Point(-size.width/3,0), BITMASK_ENEMY, BITMASK_HERO);
}



void EnemyLinhCamGiao2::playsoundAt()
{
	AudioManager::playSound(SOUND_TC1AT);
}

void EnemyLinhCamGiao2::playsoundDie()
{
	AudioManager::playSound(SOUND_TC1DIE);
}

void EnemyLinhCamGiao2::updatePos()
{
	if (body != nullptr ) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height/2 - this->getParent()->getPositionY());
	}
}

void EnemyLinhCamGiao2::makeMask()
{
	
}

void EnemyLinhCamGiao2::addSquareFixture(Size size, Vec2 pos, unsigned int category, unsigned int mask)
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



//void EnemyLinhCamGiao2::genSplash()
//{
//	splash = Sprite::create("Animation/Enemy-DeTuToanChan1/slashenemy.png");
//	splash->setScale(SCREEN_SIZE.height/5/splash->getContentSize().height);
//	splash->setAnchorPoint(Point(1, 0));
//	
//	splash->setPosition(-this->getBody()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO, 0);
//	splash->setVisible(false);
//	this->addChild(splash);
//}

