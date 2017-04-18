#include "BaseEnemy.h"
#include "GameScene.h"

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
		this->setPositionX(body->GetPosition().x * PTM_RATIO- this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO
		-this->getParent()->getPositionY());
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

		if (!isOccur) {

		}
	}
}

void BaseEnemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

