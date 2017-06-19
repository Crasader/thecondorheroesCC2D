#include "Twinkle.h"

Twinkle::Twinkle() : Sprite() {
}

Twinkle::~Twinkle() {
}

Twinkle * Twinkle::create(float p_fDelay) {
	Twinkle *_pTwinkle = new Twinkle();
	if (_pTwinkle) {
		_pTwinkle->initWithFile("UI/twinkle.png");
		_pTwinkle->initTwinkle(p_fDelay);

		_pTwinkle->autorelease();

		return _pTwinkle;
	}
	CC_SAFE_DELETE(_pTwinkle);

	return NULL;
}

void Twinkle::initTwinkle(float p_fDelay) {
	ScaleBy *_pScaleDown = ScaleBy::create(0.3f, 0.01f);
	RotateBy *_pRotate = RotateBy::create(1.0f, 360.0f);
	this->runAction(RepeatForever::create(Sequence::create(_pScaleDown, DelayTime::create(p_fDelay), _pScaleDown->reverse(), DelayTime::create(1.0f), NULL)));
	this->runAction(RepeatForever::create(Sequence::create(_pRotate, NULL)));
}