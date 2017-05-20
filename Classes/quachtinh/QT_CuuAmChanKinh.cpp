#include "QT_CuuAmChanKinh.h"

QT_CuuAmChanKinh::QT_CuuAmChanKinh()
{
	isAdded = false;
}

QT_CuuAmChanKinh::~QT_CuuAmChanKinh()
{
}

QT_CuuAmChanKinh * QT_CuuAmChanKinh::create()
{
	QT_CuuAmChanKinh* ck = new QT_CuuAmChanKinh();
	ck->initWithFile("Animation/QuachTinh/skill1_firedown_1.png");
	ck->setTag(TAG_QT_CUU_AM_CHAN_KINH);

	ck->angle = -45.0;
	ck->setRotation(ck->angle);

	return ck;
}

void QT_CuuAmChanKinh::initCirclePhysic(b2World * world, Point pos)
{
	auto size = this->getBoundingBox().size;

	// config physic manually
	b2BodyDef bodyDef;
	b2CircleShape shape;
	b2FixtureDef fixtureDef;

	shape.m_radius = size.width / 6 / PTM_RATIO;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_SWORD;
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(0);

	body->SetFixedRotation(true);
}

void QT_CuuAmChanKinh::hitGround()
{

}


void QT_CuuAmChanKinh::updateMe()
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getBoundingBox().size.width / 5);
		this->setPositionY(body->GetPosition().y * PTM_RATIO + this->getBoundingBox().size.height / 5);	
	}
}

