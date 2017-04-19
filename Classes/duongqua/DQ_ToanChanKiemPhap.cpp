#include "DQ_ToanChanKiemPhap.h"

DQ_ToanChanKiemPhap::DQ_ToanChanKiemPhap()
{
	isAdded = false;
}

DQ_ToanChanKiemPhap::~DQ_ToanChanKiemPhap()
{
}

DQ_ToanChanKiemPhap * DQ_ToanChanKiemPhap::create()
{
	DQ_ToanChanKiemPhap* tckp = new DQ_ToanChanKiemPhap();
	tckp->initWithSpriteFrameName("skill1.png");
	tckp->setTag(TAG_DQ_TOAN_CHAN_KIEM_PHAP);
	return tckp;
}

void DQ_ToanChanKiemPhap::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}

