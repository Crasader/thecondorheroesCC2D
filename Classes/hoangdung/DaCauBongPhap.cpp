#include "DaCauBongPhap.h"

DaCauBongPhap::DaCauBongPhap(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile, atlasFile, scale) {
	isAdded = false;
	vel = b2Vec2(SCREEN_SIZE.width * 1.5f / PTM_RATIO, 0);
}

DaCauBongPhap * DaCauBongPhap::create(string jsonFile, string atlasFile, float scale) {
	DaCauBongPhap *dcbp = new DaCauBongPhap(jsonFile, atlasFile, scale);
	dcbp->setTag(TAG_SLASH);
	dcbp->setAnimation(0, "animation", true);
	dcbp->update(0.0f);
	dcbp->isDie = false;
	dcbp->setB2Body(nullptr);
	return dcbp;
}

void DaCauBongPhap::initCirclePhysic(b2World * world, Point pos) {
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_SWORD);
	this->changeBodyMaskBits(BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG | BITMASK_ENEMY);
}

void DaCauBongPhap::updateMe() {
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY((body->GetPosition().y - body->GetFixtureList()->GetShape()->m_radius) * PTM_RATIO);
		this->body->SetLinearVelocity(vel);
	}
}
