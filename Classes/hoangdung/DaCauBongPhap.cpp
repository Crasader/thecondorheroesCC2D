#include "DaCauBongPhap.h"

DaCauBongPhap::DaCauBongPhap(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile, atlasFile, scale) {
	isAdded = false;
	vel = b2Vec2(SCREEN_SIZE.width * 1.5f / PTM_RATIO, 0);
}

DaCauBongPhap * DaCauBongPhap::create(string jsonFile, string atlasFile, float scale) {
	DaCauBongPhap *dcbp = new DaCauBongPhap(jsonFile, atlasFile, scale);
	dcbp->setTag(TAG_DA_CAU_BONG_PHAT);
	dcbp->setAnimation(0, "animation", true);
	dcbp->update(0.0f);
	dcbp->radius = dcbp->getBoundingBox().size.height / 2;
	dcbp->setB2Body(nullptr);
	dcbp->autorelease();
	return dcbp;
}

void DaCauBongPhap::initCirclePhysic(b2World * world, Point pos) {
	b2CircleShape circle_shape;
	//circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
	circle_shape.m_radius = radius / PTM_RATIO;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = BITMASK_SWORD;
	fixtureDef.filter.maskBits = BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG | BITMASK_ENEMY;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->SetGravityScale(0);
	body->CreateFixture(&fixtureDef);
}

void DaCauBongPhap::updateMe() {
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY((body->GetPosition().y - body->GetFixtureList()->GetShape()->m_radius) * PTM_RATIO);
		this->body->SetLinearVelocity(vel);
	}
}
