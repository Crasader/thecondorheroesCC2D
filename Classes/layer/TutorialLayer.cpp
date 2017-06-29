#include "TutorialLayer.h"
#include "GameScene.h"


//bool TutorialLayer::onTouchBegan(Touch * touch, Event * unused_event)
//{
//	if (this->isVisible()) {
//
//		this->setVisible(false);
//		auto parentLayer = dynamic_cast<GameScene*> (this->getParent()->getChildByName("gameLayer"));
//		if (type == 1) {
//			parentLayer->resumeAfterTut(1);
//		}
//		else if(type == 2)
//			parentLayer->resumeAfterTut(2);
//		else
//			parentLayer->resumeAfterTut(4);
//
//
//		Director::getInstance()->getEventDispatcher()->removeEventListener(m_listener);
//		m_listener = nullptr;
//	}
//
//	return false;
//	
//}

void TutorialLayer::showJump()
{
	/*m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);*/

	this->setVisible(true);
}

void TutorialLayer::showAttack()
{
	/*m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
*/
	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut2.png");
}

void TutorialLayer::showSkills()
{
	/*m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);*/

	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut3.png");
}

void TutorialLayer::showBird()
{
	/*m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = CC_CALLBACK_2(TutorialLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);*/

	this->setVisible(true);
	main->setTexture("UI/UI_tutorial/imgtut4.png");
}


bool TutorialLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	main = Sprite::create("UI/UI_tutorial/imgtut1.png");
	main->setScale(visibleSize.height * 0.75f / main->getContentSize().height);
	main->setPosition(origin + visibleSize / 2);
	addChild(main);

	return true;
}

TutorialLayer* TutorialLayer::create()
{
	TutorialLayer* tut = new(std::nothrow) TutorialLayer();

	if (tut && tut->init())
	{
		tut->autorelease();
		return tut;
	}
	else
	{
		delete tut;
		tut = nullptr;
		return nullptr;
	}
}

//bool TutorialJump::onTouchBegan(Touch * touch, Event * unused_event)
//{
//	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	auto location = touch->getLocation();
//	if (location.x < visibleSize.width / 2) {
//		//log("Done Jump Tutorial");
//		parentLayer->jump();
//	}
//
//	parentLayer->resumeAfterTut(1);
//
//	return false;
//}
//
//bool TutorialJump::init(int type)
//{
//	//////////////////////////////
//	// 1. super init first
//	if (!Layer::init())
//	{
//		return false;
//	}
//	auto origin = Director::getInstance()->getVisibleOrigin();
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	
//	if (type == 1) {
//		auto guideImage = Sprite::create("UI/UI_tutorial/imgtut1.png");
//		guideImage->setScale(visibleSize.height * 0.75f / guideImage->getContentSize().height);
//		guideImage->setPosition(origin + visibleSize / 2);
//		addChild(guideImage);
//	}
//	else {
//		auto lbText = Label::createWithTTF("Don't let hero fall down into abyss", "fonts/Marker Felt.ttf", 32);
//		lbText->setScale(visibleSize.height * 0.06f / lbText->getContentSize().height);
//
//		lbText->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.73f);
//		addChild(lbText);
//	}
//
//	this->schedule([&](float dt) {
//		counter++;
//		if (counter >= 5) {
//			counter = 0;
//			listener = EventListenerTouchOneByOne::create();
//			listener->onTouchBegan = CC_CALLBACK_2(TutorialJump::onTouchBegan, this);
//			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//			unschedule("tut_key");
//		}
//	}, 0.1f, "tut_key");
//
//	return true;
//}
//
//TutorialJump * TutorialJump::create(int type)
//{
//	TutorialJump* tut = new(std::nothrow) TutorialJump();
//
//	if (tut && tut->init(type))
//	{
//		tut->autorelease();
//		return tut;
//	}
//	else
//	{
//		delete tut;
//		tut = nullptr;
//		return nullptr;
//	}
//}
//
//bool TutorialAttack::init()
//{
//	TutorialLayer::init();
//
//	auto origin = Director::getInstance()->getVisibleOrigin();
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	/*auto lbText = Label::createWithTTF("Press Attack Button to hit Enemy nearby", "fonts/Marker Felt.ttf", 32);
//	lbText->setScale(SCREEN_SIZE.height * 0.06f / lbText->getContentSize().height);
//	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
//	addChild(lbText);*/
//
//	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut2.png");
//	guideImage->setScale(visibleSize.height * 0.75f / guideImage->getContentSize().height);
//	guideImage->setPosition(origin + visibleSize / 2);
//	addChild(guideImage);
//
//	return true;
//}
//
//TutorialAttack * TutorialAttack::create()
//{
//	TutorialAttack* tut = new(std::nothrow) TutorialAttack();
//
//	if (tut && tut->init())
//	{
//		tut->autorelease();
//		return tut;
//	}
//	else
//	{
//		delete tut;
//		tut = nullptr;
//		return nullptr;
//	}
//}
//
//bool TutorialSkill::init()
//{
//	TutorialLayer::init();
//	auto origin = Director::getInstance()->getVisibleOrigin();
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//
//	/*auto lbText = Label::createWithTTF("Use Skill Button to active special hero's skill", "fonts/Marker Felt.ttf", 32);
//	lbText->setScale(SCREEN_SIZE.height * 0.06f / lbText->getContentSize().height);
//	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
//	addChild(lbText);*/
//
//	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut3.png");
//	guideImage->setScale(visibleSize.height * 0.75f / guideImage->getContentSize().height);
//	guideImage->setPosition(origin + visibleSize / 2);
//	addChild(guideImage);
//
//	return true;
//}
//
//TutorialSkill * TutorialSkill::create()
//{
//	TutorialSkill* tut = new(std::nothrow) TutorialSkill();
//
//	if (tut && tut->init())
//	{
//		tut->autorelease();
//		return tut;
//	}
//	else
//	{
//		delete tut;
//		tut = nullptr;
//		return nullptr;
//	}
//}
//
//bool TutorialIntroBird::init()
//{
//	//////////////////////////////
//	// 1. super init first
//	if (!Layer::init())
//	{
//		return false;
//	}
//	
//	auto origin = Director::getInstance()->getVisibleOrigin();
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//
//	/*auto lbText = Label::createWithTTF("Call your Cordon and you will fly with him.\nYou will be protected", "fonts/Marker Felt.ttf", 32);
//	lbText->setAlignment(TextHAlignment::CENTER);
//	lbText->setScale(SCREEN_SIZE.height * 0.12f / lbText->getContentSize().height);
//	lbText->setPosition(origin.x + SCREEN_SIZE.width / 2, origin.y + SCREEN_SIZE.height * 0.73f);
//	addChild(lbText);*/
//
//	auto guideImage = Sprite::create("UI/UI_tutorial/imgtut4.png");
//	guideImage->setScale(visibleSize.height * 0.75f / guideImage->getContentSize().height);
//	guideImage->setPosition(origin + visibleSize / 2);
//	addChild(guideImage);
//
//	this->schedule([&](float dt) {
//		counter++;
//		if (counter >= 5) {
//			counter = 0;
//			listener = EventListenerTouchOneByOne::create();
//			listener->onTouchBegan = CC_CALLBACK_2(TutorialIntroBird::onTouchBegan, this);
//			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//			unschedule("tut_key");
//		}
//	}, 0.1f, "tut_key");
//	return true;
//}
//
//TutorialIntroBird * TutorialIntroBird::create()
//{
//	TutorialIntroBird* tut = new(std::nothrow) TutorialIntroBird();
//
//	if (tut && tut->init())
//	{
//		tut->autorelease();
//		return tut;
//	}
//	else
//	{
//		delete tut;
//		tut = nullptr;
//		return nullptr;
//	}
//}
//
//bool TutorialIntroBird::onTouchBegan(Touch * touch, Event * unused_event)
//{
//	auto parentLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
//	parentLayer->resumeAfterTut(4);
//	return false;
//}
