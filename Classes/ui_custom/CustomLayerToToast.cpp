#include "CustomLayerToToast.h"

CustomLayerToToast::CustomLayerToToast() : Layer() {
}

CustomLayerToToast::~CustomLayerToToast() {
}

CustomLayerToToast * CustomLayerToToast::create(string p_sMessage, float p_fTimeOption) {
	CustomLayerToToast *_pToast = new CustomLayerToToast();
	if (_pToast) {
		_pToast->autorelease();
		_pToast->setMessage(p_sMessage, p_fTimeOption);
		_pToast->scheduleUpdate();

		return _pToast;
	}
	CC_SAFE_DELETE(_pToast);

	return NULL;
}

void CustomLayerToToast::setMessage(string p_sMessage, float p_fTimeOption) {
	m_pLabelMessage = Label::create(p_sMessage.c_str(), "fonts/BAUHS93.TTF", 16);
	m_pLabelMessage->setTextColor(Color4B::WHITE);
	m_pLabelMessage->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_pLabelMessage->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	m_pLabelMessage->setMaxLineWidth(Director::getInstance()->getVisibleSize().width * 0.9f);
	this->addChild(m_pLabelMessage, 1);

	m_pToastBackground = Sprite::create("UI/toast.png");
	m_pToastBackground->setScaleX(m_pLabelMessage->getContentSize().width / m_pToastBackground->getContentSize().width * 1.2f);
	m_pToastBackground->setScaleY(m_pLabelMessage->getContentSize().height / m_pToastBackground->getContentSize().height * 1.2f);
	m_pToastBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_pToastBackground->setPosition(m_pLabelMessage->getContentSize().width / 2, m_pLabelMessage->getContentSize().height / 2);
	m_pToastBackground->setOpacity(120);
	m_pLabelMessage->addChild(m_pToastBackground, -1);

	m_fMessageOpacity = 240.0f;
	m_fBackgroundOpacity = 120.0f;
	m_fTimeExist = p_fTimeOption;
}

void CustomLayerToToast::update(float p_fDelta) {
	if (m_fTimeExist > 0.0f) {
		m_fTimeExist -= p_fDelta;
		return;
	}
	if (m_fMessageOpacity <= 0.0f) {
		m_fMessageOpacity = 0.0f;
		this->removeFromParentAndCleanup(true);
		return;
	}
	if (m_fBackgroundOpacity <= 0.0f) {
		m_fBackgroundOpacity = 0.0f;
	}
	m_pLabelMessage->setOpacity(m_fMessageOpacity);
	m_pToastBackground->setOpacity(m_fBackgroundOpacity);
	m_fMessageOpacity -= 2.0f;
	m_fBackgroundOpacity--;
}