#include "Darts.h"

Darts::Darts(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile, atlasFile, scale) {
}

Darts * Darts::create(string jsonFile, string atlasFile, float scale) {
	Darts *darts = new Darts(jsonFile, atlasFile, scale);
	darts->setTag(TAG_SLASH);
	darts->setScaleX(1);
	darts->setAnimation(0, "attack", true);
	darts->update(0.0f);
	darts->isDie = false;
	darts->setB2Body(nullptr);
	darts->autorelease();
	return darts;
}

void Darts::initCirclePhysic(b2World * world, Point pos) {
	B2Skeleton::initCirclePhysic(world, pos);
	auto a = this->getBoundingBox().size.height;
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_SLASH);
	this->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
}

void Darts::updateMe(BaseHero * hero) {
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO
			- this->getParent()->getPositionY()+this->getBoundingBox().size.height / 4.0f);
	}
}
