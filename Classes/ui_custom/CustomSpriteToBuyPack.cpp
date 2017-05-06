#include "CustomSpriteToBuyPack.h"


CustomSpriteToBuyPack::CustomSpriteToBuyPack() : Sprite() {
}

CustomSpriteToBuyPack::~CustomSpriteToBuyPack() {
}

CustomSpriteToBuyPack * CustomSpriteToBuyPack::create(string p_sFilePath, ccMenuCallback p_pCallBack, ScrollView *p_pParent) {
	CustomSpriteToBuyPack *_pCustomSprite = new CustomSpriteToBuyPack();
	if (_pCustomSprite->initWithFile(p_sFilePath)) {
		_pCustomSprite->autorelease();
		_pCustomSprite->initOptions(p_pParent, p_pCallBack);
		_pCustomSprite->addEvents();
		return _pCustomSprite;
	}
	CC_SAFE_DELETE(_pCustomSprite);
	return NULL;
}

void CustomSpriteToBuyPack::initOptions(ScrollView *p_pParent, ccMenuCallback p_pCallBack) {
	m_bTouchMoved = false;
	m_pParent = p_pParent;
	m_pCallBack = p_pCallBack;
	m_fBigScale = m_pParent->getContentSize().height / this->getContentSize().height * 0.9;
	m_fSmallScale = m_pParent->getContentSize().height / this->getContentSize().height * 0.8f;
}

void CustomSpriteToBuyPack::addEvents() {
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CustomSpriteToBuyPack::touchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CustomSpriteToBuyPack::touchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(CustomSpriteToBuyPack::touchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CustomSpriteToBuyPack::touchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();
}

bool CustomSpriteToBuyPack::touchBegan(Touch *p_pTouch, Event *p_pEvent) {
	Vec2 _v2TouchPoint = p_pTouch->getLocation() - m_pParent->getInnerContainerPosition();
	Rect _rBoundingBox = this->getBoundingBox();
	m_bTouchMoved = false;
	if (_rBoundingBox.containsPoint(_v2TouchPoint)) {
		this->runAction(ScaleTo::create(0.1f, m_fSmallScale, m_fSmallScale));
		return true;
	}
	return false;
}

void CustomSpriteToBuyPack::touchCancelled(Touch *p_pTouch, Event *p_pEvent) {
	if (m_bTouchMoved) {
		return;
	}
	this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
	m_bTouchMoved = true;
}

void CustomSpriteToBuyPack::touchEnded(Touch *p_pTouch, Event *p_pEvent) {
	if (m_bTouchMoved) {
		return;
	}
	Vec2 _v2TouchPoint = p_pTouch->getLocation() - m_pParent->getInnerContainerPosition();
	Rect _rBoundingBox = this->getBoundingBox();
	if (_rBoundingBox.containsPoint(_v2TouchPoint)) {
		this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
		this->runAction(CallFuncN::create(m_pCallBack));
	}
}