#include "BaseEnemy.h"

Enemy::Enemy(string jsonFile, string atlasFile, float scale):B2Skeleton(jsonFile, atlasFile, scale)
{
}

Enemy * Enemy::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

void Enemy::getHit()
{
}

void Enemy::move()
{
}

void Enemy::shoot()
{
}

void Enemy::shoot(Point posOfHuman)
{
}

void Enemy::die()
{
}

void Enemy::updateEnemy(float dt)
{
}

void Enemy::initCirclePhysic(b2World * world, Point pos)
{
}

bool Enemy::checkOutScreen(Point posCamera)
{
	return false;
}

void Enemy::resetEnemy()
{
}
