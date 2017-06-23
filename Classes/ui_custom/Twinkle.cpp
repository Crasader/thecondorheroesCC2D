#include "Twinkle.h"

Twinkle::Twinkle() : Sprite() {
}

Twinkle::~Twinkle() {
}

Twinkle * Twinkle::create() {
	Twinkle *_pTwinkle = new Twinkle();
	if (_pTwinkle) {
		_pTwinkle->initWithFile("UI/twinkle.png");
		_pTwinkle->initTwinkle();

		_pTwinkle->autorelease();

		return _pTwinkle;
	}
	CC_SAFE_DELETE(_pTwinkle);

	return NULL;
}

void Twinkle::initTwinkle() {
	ScaleBy *_pScaleDown = ScaleBy::create(0.3f, 0.01f);
	RotateBy *_pRotate = RotateBy::create(1.0f, 360.0f);
	this->runAction(RepeatForever::create(Sequence::create(_pScaleDown, DelayTime::create(CCRANDOM_0_1() * 2.0f + 2.0f), _pScaleDown->reverse(), DelayTime::create(0.5f), NULL)));
	this->runAction(RepeatForever::create(Sequence::create(_pRotate, NULL)));
}