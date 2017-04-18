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
	slash->isDie = false;
	slash->setB2Body(nullptr);
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
	this->changeBodyCategoryBits(BITMASK_SLASH);
	this->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
}

void Slash::updateMe(BaseHero * hero)
{
	//B2Skeleton::updateMe(hero);
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO
			- this->getParent()->getPositionY()+this->getBoundingBox().size.height / 4.0f);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
}


//void Slash::die()
//{
//	this->setVisible(false);
//}
