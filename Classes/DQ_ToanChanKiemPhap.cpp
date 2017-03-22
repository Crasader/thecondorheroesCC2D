#include "DQ_ToanChanKiemPhap.h"

DQ_ToanChanKiemPhap::DQ_ToanChanKiemPhap()
{
}

DQ_ToanChanKiemPhap::~DQ_ToanChanKiemPhap()
{
}

DQ_ToanChanKiemPhap * DQ_ToanChanKiemPhap::create(string file)
{
	DQ_ToanChanKiemPhap* tckp = new DQ_ToanChanKiemPhap();
	tckp->initWithFile(file);
	tckp->setTag(TAG_DQ_TOAN_CHAN_KIEM_PHAP);
	return tckp;
}

void DQ_ToanChanKiemPhap::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}


void DQ_ToanChanKiemPhap::die()
{
	this->setVisible(false);
}

