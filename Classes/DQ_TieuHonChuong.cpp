#include "DQ_TieuHonChuong.h"
#include "DQ_TieuHonChuong.h"

DQ_TieuHonChuong::DQ_TieuHonChuong()
{
}

DQ_TieuHonChuong::~DQ_TieuHonChuong()
{
}

DQ_TieuHonChuong * DQ_TieuHonChuong::create(string file)
{
	DQ_TieuHonChuong* thc = new DQ_TieuHonChuong();
	thc->initWithFile(file);
	thc->setTag(TAG_DQ_TIEU_HON_CHUONG);
	return thc;
}

void DQ_TieuHonChuong::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->getB2Body()->SetUserData(this);
}


void DQ_TieuHonChuong::die()
{
	this->setVisible(false);
}

