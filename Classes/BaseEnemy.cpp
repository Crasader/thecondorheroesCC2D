#include "BaseEnemy.h"
#include "layer/GameScene.h"


BaseEnemy::BaseEnemy() :B2Skeleton()
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
}

BaseEnemy::~BaseEnemy()
{
}

BaseEnemy::BaseEnemy(spSkeletonData * data):B2Skeleton(data)
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
}

BaseEnemy::BaseEnemy(string jsonFile, string atlasFile, float scale):B2Skeleton(jsonFile, atlasFile, scale)
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
}

BaseEnemy * BaseEnemy::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

BaseEnemy * BaseEnemy::create(spSkeletonData * data)
{
	auto skeleton = new BaseEnemy(data);
	//skeleton->initWithData(data);
	skeleton->update(1.0f);
	return skeleton;
}

void BaseEnemy::run()
{
}

void BaseEnemy::attack()
{
}

void BaseEnemy::die()
{
	if (!isEndOfScreen) {
		auto gameLayer = (GameScene*) this->getParent()->getParent();
		gameLayer->updateMultiKills();
	}
}

void BaseEnemy::updateMe(BaseHero* hero)
{
	if (body != nullptr /*&& body->GetType() == b2_staticBody*/) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO - this->getParent()->getPositionY());
	}


	if (hero->getIsKillAll() && this->getB2Body() != nullptr) {
		if (this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.75f &&
			this->getPositionX() + this->getParent()->getPositionX() > hero->getPositionX() - SCREEN_SIZE.width * 0.28f &&
			hero->getPositionY() + SCREEN_SIZE.height * 0.5f > this->getPositionY() &&
			hero->getPositionY() - SCREEN_SIZE.height * 0.75f < this->getPositionY()

			) {

			die();
		}
	}
}

void BaseEnemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

void BaseEnemy::initBoxPhysic(b2World * world, Point pos)
{
	B2Skeleton::initBoxPhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

