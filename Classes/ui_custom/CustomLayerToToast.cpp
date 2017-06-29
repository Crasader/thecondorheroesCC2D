#include "CustomLayerToToast.h"

CustomLayerToToast::CustomLayerToToast() {
}

CustomLayerToToast::~CustomLayerToToast() {
}

CustomLayerToToast * CustomLayerToToast::create(string p_sMessage, float p_fTimeOption) {
	CustomLayerToToast *_pToast = new CustomLayerToToast();
	if (_pToast) {
		_pToast->initWithTTF(p_sMessage.c_str(), "fonts/arial.ttf", 16);
		_pToast->setTextColor(Color4B::WHITE);
		_pToast->setMaxLineWidth(Director::getInstance()->getVisibleSize().width * 0.9f);

		_pToast->m_nBackground = Sprite::create("UI/toast.png");
		_pToast->m_nBackground->setOpacity(170);
		_pToast->m_nBackground->setPosition(_pToast->getContentSize().width / 2, _pToast->getContentSize().height / 2);
		
		_pToast->addChild(_pToast->m_nBackground, -1);

		_pToast->m_nBackground->setScaleX(_pToast->getContentSize().width / _pToast->m_nBackground->getContentSize().width * 1.2f);
		_pToast->m_nBackground->setScaleY(_pToast->getContentSize().height / _pToast->m_nBackground->getContentSize().height * 1.4f);
		

		_pToast->fading(p_fTimeOption);

		_pToast->autorelease();

		return _pToast;
	}
	CC_SAFE_DELETE(_pToast);

	return NULL;
}



void CustomLayerToToast::fading(float p_fTimeOption)
{
	this->m_nBackground->runAction(EaseOut::create(FadeTo::create(p_fTimeOption, 0), 0.5f));
	this->runAction(Sequence::createWithTwoActions(EaseOut::create(FadeTo::create(p_fTimeOption, 0), 0.5f), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	})));
}

