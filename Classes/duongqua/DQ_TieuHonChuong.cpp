#include "DQ_TieuHonChuong.h"

DQ_TieuHonChuong::DQ_TieuHonChuong()
{
	isCollide = false;
	isAdded = false;
	vel = b2Vec2(SCREEN_SIZE.width * 1.3f / PTM_RATIO, 0);
}

DQ_TieuHonChuong::~DQ_TieuHonChuong()
{
}

DQ_TieuHonChuong * DQ_TieuHonChuong::create(string frameName)
{
	DQ_TieuHonChuong* thc = new DQ_TieuHonChuong();
	thc->setAnchorPoint(Vec2(1.0f, 0.5f));
	thc->initWithSpriteFrameName(frameName);
	thc->setTag(TAG_DQ_TIEU_HON_CHUONG);
	return thc;
}

void DQ_TieuHonChuong::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initBoxPhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}

void DQ_TieuHonChuong::updateMe()
{
	if (getB2Body() != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));	// rotate

		this->body->SetLinearVelocity(vel);
	}
}

