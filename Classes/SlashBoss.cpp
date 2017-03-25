#include "SlashBoss.h"


SlashBoss::SlashBoss(string json, string atlas, float scale):B2Skeleton(json,atlas,scale)
{
}

SlashBoss::~SlashBoss()
{
}

SlashBoss * SlashBoss::create(string json, string atlas, float scale)
{
	SlashBoss *slash = new SlashBoss(json, atlas, scale);
	slash->setTag(TAG_SLASH);
	slash->setScaleX(1);
	slash->setAnimation(0, "skill-boss", true);
	slash->update(0.0f);
	return slash;
}

void SlashBoss::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}


void SlashBoss::die()
{
	this->setVisible(false);
	auto world = this->getB2Body()->GetWorld();
	world->DestroyBody(this->getB2Body());
	this->setB2Body(nullptr);
}

void SlashBoss::setAngel(float radian)
{
	if (this->getB2Body() != nullptr) {
		float vx = SCREEN_SIZE.width / 1 / PTM_RATIO * cosf(radian);
		float vy = SCREEN_SIZE.width / 1 / PTM_RATIO * sinf(radian);
		this->body->SetLinearVelocity(b2Vec2(vx, vy));
	}
}
