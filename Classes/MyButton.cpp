#include "MyButton.h"

MyButton::MyButton() {

}

MyButton * MyButton::create(string file_name_main, string file_name_CoolDown, Point pos)
{
	MyButton *mNode = new MyButton();
	mNode->initWithFile(file_name_main);
	mNode->coolDown = Sprite::create(file_name_CoolDown);

	mNode->setPosition(pos);
	mNode->coolDown->setPosition(pos);
	mNode->coolDown->setVisible(false);

	/*mNode->effectCoolDown = Sprite::create("UI/effect_fullskill.png");
	mNode->effectCoolDown->setVisible(false);
	mNode->effectCoolDown->setPosition(mNode->getBoundingBox().size.width / 2, mNode->getBoundingBox().size.height / 2);
	mNode->addChild(mNode->effectCoolDown);*/

	mNode->number = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 32);
	mNode->number->setPosition(pos);
	mNode->number->setVisible(false);


	mNode->isBlocked = false;
	mNode->canTouch = true;
	mNode->setIsActive(false);
	//mNode->addEvents();
	return mNode;
}


// add listener to sprite
void MyButton::addEvents()
{
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);								// preventing other listener from using it

	listener->onTouchBegan = [&](Touch *mTouch, Event *mEvent)
	{
		auto p = mTouch->getLocation();
		convertToNodeSpace(p);
		Rect rect = this->getBoundingBox();
		
		if (rect.containsPoint(p) && !isBlocked)	// if this button is blocked (smt while another button is active), cannot active
		{	
			if (canTouch) {
				runTimer();
				coolDown->setVisible(true);
				this->schedule(schedule_selector(MyButton::checkInterval), timeCoolDown, 1, 0);
				canTouch = false;
				isActive = true;
			}
			else {
				//log("Cool Down, B*tch");
			}
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyButton::pauseListener()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
}

void MyButton::refresh()
{
	canTouch = true;
	isActive = false;
	coolDown->setVisible(false);
	number->setVisible(false);
	this->unscheduleAllCallbacks();
}

void MyButton::checkInterval(float dt)
{
	this->unschedule(schedule_selector(MyButton::checkInterval));
	canTouch = true;
	isActive = false;
	if(!isBlocked)
		coolDown->setVisible(false);
}

void MyButton::runTimer()
{
	if (timeCoolDown < 1)
		return;
	timer = timeCoolDown;
	number->setString(StringUtils::format("%i", timer));
	number->setVisible(true);
	this->schedule([&](float dt) {
		--timer;
		number->setString(StringUtils::format("%i", timer));
		if (timer <= 0) {
			number->setVisible(false);
			this->unschedule("Key_timer");
		}
	}, 1.0f, "Key_timer");

}

