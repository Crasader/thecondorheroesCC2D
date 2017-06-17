#include "Twinkle.h"

Twinkle::Twinkle() : Layer() {
}

Twinkle::~Twinkle() {
}

Twinkle * Twinkle::create(float p_fDelay) {
	Twinkle *_pToast = new Twinkle();
	if (_pToast) {
		_pToast->autorelease();
		_pToast->initTwinkle(p_fDelay);
		_pToast->scheduleUpdate();

		return _pToast;
	}
	CC_SAFE_DELETE(_pToast);

	return NULL;
}

void Twinkle::initTwinkle(float p_fDelay) {
	m_pTwinkle = Sprite::create("UI/twinkle.png");
	m_pTwinkle->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(m_pTwinkle, 1);
	this->setContentSize(m_pTwinkle->getContentSize());

	ScaleBy *_pScaleDown = ScaleBy::create(0.3f, 0.01f);
	RotateBy *_pRotate = RotateBy::create(1.0f, 360.0f);
	m_pTwinkle->runAction(RepeatForever::create(Sequence::create(_pScaleDown, DelayTime::create(p_fDelay), _pScaleDown->reverse(), DelayTime::create(1.0f), NULL)));
	m_pTwinkle->runAction(RepeatForever::create(Sequence::create(_pRotate, NULL)));
}

void Twinkle::update(float p_fDelta) {
}