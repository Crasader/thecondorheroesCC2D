#include "CustomSpriteToBuyPack.h"
#include "CustomLayerToToast.h"

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
	m_fBigScale = m_pParent->getContentSize().height / this->getContentSize().height * 0.9f;
	m_fSmallScale = m_pParent->getContentSize().height / this->getContentSize().height * 0.8f;
}

void CustomSpriteToBuyPack::addEvents() {
	/*auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CustomSpriteToBuyPack::touchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CustomSpriteToBuyPack::touchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(CustomSpriteToBuyPack::touchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CustomSpriteToBuyPack::touchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/

	// button diamond tab
	/*auto _pNormal = Sprite::create("UI/UI_main_menu/UI_shop/pack_background.png");
	auto _pSelected = Sprite::create("UI/UI_main_menu/UI_shop/pack_background.png");
	m_pButton = Button::create();
	m_pButton->setScaleX(this->getContentSize().width / m_pButton->getContentSize().width);
	m_pButton->setScaleY(this->getContentSize().height / m_pButton->getContentSize().height);
	m_pButton->addTouchEventListener(CC_CALLBACK_2(CustomSpriteToBuyPack::receiveButtonPressEvent, this));
	m_pButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_pButton->setPosition(Vec2(0.0f, 0.0f));
	m_pButton->setSwallowTouches(true);
	this->addChild(m_pButton, 1);*/

	//Menu *_pMenu = Menu::create(m_pButton, NULL);
	//_pMenu->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
	//_pMenu->setAnchorPoint(Vec2(0.0f, 0.0f));
	//_pMenu->setPosition(Vec2(0.0f, 0.0f));
	//_pMenu->setSwallowsTouches(false);
	//this->addChild(_pMenu, 1);
}

void CustomSpriteToBuyPack::receiveButtonPressEvent(Ref *pSender, ui::Widget::TouchEventType type) {
	switch (type) {
	case ui::Widget::TouchEventType::BEGAN:{
											   break;
	}

	case ui::Widget::TouchEventType::MOVED:{
											   break;
	}

	case ui::Widget::TouchEventType::ENDED:{
											   //Some code here if the button is pressed…”
											   break;
	}

	case ui::Widget::TouchEventType::CANCELED:{
												  break;
	}

	default:{
				break;
	}
	}//End switch(type)
}

bool g_bShowToastAble = true;

bool CustomSpriteToBuyPack::touchBegan(Touch *p_pTouch, Event *p_pEvent) {
	Vec2 _v2TouchPoint = Vec2(p_pTouch->getLocation().x - m_pParent->getInnerContainerPosition().x - m_pParent->getPosition().x,
		p_pTouch->getLocation().y - m_pParent->getPosition().y);
	Rect _rBoundingBox = this->getBoundingBox();
	m_bTouchMoved = false;

	if (g_bShowToastAble == true) {
		CustomLayerToToast *_pToast = CustomLayerToToast::create(
			String::createWithFormat("Touch: %d, %d, rec: %d, %d : %d, %d",
			_v2TouchPoint.x, _v2TouchPoint.y, _rBoundingBox.getMinX(), _rBoundingBox.getMaxX(), _rBoundingBox.getMinY(), _rBoundingBox.getMaxY())->getCString(),
			TOAST_LONG * 10.0f);
		_pToast->setPosition(Vec2(-100.0f, 50.0f));
		this->addChild(_pToast, 10);
		g_bShowToastAble = false;
	}

	if (_rBoundingBox.containsPoint(_v2TouchPoint)) {
		this->runAction(ScaleTo::create(0.1f, m_fSmallScale, m_fSmallScale));
		return true;
	}

	return false;

	/*this->runAction(ScaleTo::create(0.1f, m_fSmallScale, m_fSmallScale));
	Vec2 _v2TouchLocation = p_pTouch->getLocation();
	Vec2 _v2NodeSpaceLocation = this->getParent()->convertToNodeSpace(_v2TouchLocation);
	m_bTouchMoved = false;
	Rect _rTempRect = this->getBoundingBox();
	if (_rTempRect.containsPoint(_v2NodeSpaceLocation)) {
		this->runAction(ScaleTo::create(0.1f, m_fSmallScale, m_fSmallScale));
		return true;
	}

	return false;*/
}

void CustomSpriteToBuyPack::touchCancelled(Touch *p_pTouch, Event *p_pEvent) {
	if (m_bTouchMoved == true) {
		return;
	}
	this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
	m_bTouchMoved = true;
}

void CustomSpriteToBuyPack::touchEnded(Touch *p_pTouch, Event *p_pEvent) {
	g_bShowToastAble = true;
	if (m_bTouchMoved == true) {
		return;
	}
	Vec2 _v2TouchPoint = Vec2(p_pTouch->getLocation().x - m_pParent->getInnerContainerPosition().x - m_pParent->getPosition().x,
		p_pTouch->getLocation().y - m_pParent->getPosition().y);
	Rect _rBoundingBox = this->getBoundingBox();
	if (_rBoundingBox.containsPoint(_v2TouchPoint)) {
		this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
		this->runAction(CallFuncN::create(m_pCallBack));
	}

	/*if (m_bTouchMoved == true) {
		return;
	}
	this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
	Vec2 _v2TouchLocation = p_pTouch->getLocation();
	Vec2 _v2NodeSpaceLocation = this->getParent()->convertToNodeSpace(_v2TouchLocation);
	Rect _rTempRect = this->getBoundingBox();
	if (_rTempRect.containsPoint(_v2NodeSpaceLocation)) {
		this->runAction(ScaleTo::create(0.1f, m_fBigScale, m_fBigScale));
		this->runAction(CallFuncN::create(m_pCallBack));
	}*/
}