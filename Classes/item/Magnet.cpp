
#include "Magnet.h"

Magnet::Magnet()
{
	body = nullptr;
}

Magnet::~Magnet()
{
}

Magnet * Magnet::create()
{
	Magnet* magnet = new Magnet();
	magnet->initWithSpriteFrameName("coin_01.png");
	magnet->setTag(TAG_MAGNET);
	return magnet;
}


void Magnet::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	//this->chang
}


void Magnet::picked()
{

}




void Magnet::updateMe(BaseHero *hero)
{
	B2Sprite::updateMe(hero);
	
}

