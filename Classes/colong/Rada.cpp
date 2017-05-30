#include "Rada.h"

Rada::Rada() {
}

Rada::~Rada() {
}

Rada * Rada::create(string p_sFileName) {
	Rada *_pRada = new Rada();
	_pRada->initWithFile(p_sFileName);
	_pRada->setTag(TAG_CL_RADA);
	return _pRada;

}

void Rada::initCirclePhysic(b2World * world, Point pos) {
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);

}

