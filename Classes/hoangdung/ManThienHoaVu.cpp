#include "ManThienHoaVu.h"

ManThienHoaVu::ManThienHoaVu() {
	isCollide = false;
	isAdded = false;
	vel = b2Vec2(SCREEN_SIZE.width * 2.0f / PTM_RATIO, 0);
}

ManThienHoaVu::~ManThienHoaVu()
{
}

ManThienHoaVu * ManThienHoaVu::create(string frameName) {
	ManThienHoaVu* mthv = new ManThienHoaVu();
	mthv->setAnchorPoint(Vec2(1.0f, 0.5f));
	mthv->initWithSpriteFrameName(frameName);
	mthv->setTag(TAG_DQ_TIEU_HON_CHUONG);
	return mthv;
}

void ManThienHoaVu::initCirclePhysic(b2World * world, Point pos) {
	b2CircleShape circle_shape;
	//circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
	circle_shape.m_radius = this->getBoundingBox().size.height > this->getBoundingBox().size.width ? this->getBoundingBox().size.width / 2 / PTM_RATIO :
		this->getBoundingBox().size.height / 2 / PTM_RATIO;
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

	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_SWORD);
	this->changeBodyMaskBits(BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG | BITMASK_ENEMY);
}

void ManThienHoaVu::updateMe() {
	if (getB2Body() != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO);
		this->body->SetLinearVelocity(vel);
	}
}

