#include "Slash.h"

Slash::Slash()
{
}

Slash::~Slash()
{
}

Slash * Slash::create(string file)
{
	Slash* slash = new Slash();
	slash->initWithFile(file);
	slash->setTag(TAG_SLASH);
	return slash;
}

void Slash::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}


void Slash::die()
{
	this->setVisible(false);
}
