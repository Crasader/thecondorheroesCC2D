#include "Coin.h"

Coin::Coin()
{
}

Coin::~Coin()
{
}

Coin * Coin::create()
{
	Coin* coin = new Coin();
	coin->initWithSpriteFrameName("coin_01.png");
	coin->setTag(TAG_COIN);
	return coin;
}

void Coin::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_staticBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
}
