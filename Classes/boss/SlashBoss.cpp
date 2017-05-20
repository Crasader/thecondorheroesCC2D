#include "boss/SlashBoss.h"


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
	b2CircleShape circle_shape;
	//circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
	this->getBoundingBox().size.height > this->getBoundingBox().size.width ? circle_shape.m_radius = this->getBoundingBox().size.width / 4 / PTM_RATIO :
		circle_shape.m_radius = this->getBoundingBox().size.height / 4 / PTM_RATIO;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetType(b2_dynamicBody);
	body->GetFixtureList()->SetSensor(true);
	body->SetGravityScale(0);
	body->SetUserData(this);
}

void SlashBoss::updateMe(float dt)
{
	if (body != nullptr) {
		this->setPositionX(this->getB2Body()->GetPosition().x * PTM_RATIO);
		this->setPositionY(this->getB2Body()->GetPosition().y * PTM_RATIO);
	}
	if (this->getIsDie() && this->getB2Body()) {
		this->die();
		this->setIsDie(false);
	}
}


void SlashBoss::die()
{
	this->setVisible(false);
	auto world = this->getB2Body()->GetWorld();
	world->DestroyBody(this->getB2Body());
	this->setB2Body(nullptr);
}

void SlashBoss::setAngle(float radian)// radian
{
	if (this->getB2Body() != nullptr) {
		float vx = SCREEN_SIZE.width  / PTM_RATIO * cosf(radian);
		float vy = SCREEN_SIZE.width  / PTM_RATIO * sinf(radian);
		this->body->SetLinearVelocity(b2Vec2(vx, vy));
	}
}
