#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(string jsonFile, string atlasFile, float scale):B2Skeleton(jsonFile, atlasFile, scale)
{
}

BaseEnemy * BaseEnemy::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

void BaseEnemy::run()
{
}

void BaseEnemy::attack()
{
}

void BaseEnemy::die()
{
}

void BaseEnemy::update(float dt)
{
	B2Skeleton::update(dt);
}

void BaseEnemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_staticBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
}

