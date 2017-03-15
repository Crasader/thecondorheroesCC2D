#include "Slash.h"





Slash::Slash(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile,atlasFile,scale)
{

}

Slash * Slash::create(string jsonFile, string atlasFile, float scale)
{
	Slash *slash = new Slash(jsonFile, atlasFile, scale);
	slash->setTag(TAG_SLASH);
	slash->setScaleX(1);
	slash->setAnimation(0, "detu2slash", true);
	slash->update(0.0f);
	return slash;
}

void Slash::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	auto a = this->getBoundingBox().size.height;
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}

void Slash::updateMe(float dt)
{
	if (body != nullptr) {
		this->setPositionX(this->getB2Body()->GetPosition().x * PTM_RATIO);
		this->setPositionY(this->getB2Body()->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height / 4.0f);
	}
}


//void Slash::die()
//{
//	this->setVisible(false);
//}
