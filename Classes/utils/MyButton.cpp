#include "MyButton.h"

MyButton::MyButton() {

}

MyButton * MyButton::create(string file_name_main, string file_name_CoolDown, Point pos)
{
	MyButton *mNode = new MyButton();
	mNode->initWithFile(file_name_CoolDown);
	mNode->main = ProgressTimer::create(Sprite::create(file_name_main));
	mNode->main->setType(ProgressTimer::Type::RADIAL);
	mNode->main->setPercentage(100.0f);

	mNode->setPosition(pos);

	mNode->main->setPosition(mNode->getContentSize() / 2);
	mNode->addChild(mNode->main);
	
	mNode->numberOfUse = 1; // default is 1, you can set Number of use follow hero's skill, use Hero.json to config 
							// and we need some props to got it.
	mNode->numberOfUseHasNotUsedYet = 1;

	mNode->number = Label::createWithBMFont("fonts/font_life-export.fnt", "0");
	mNode->number->setBMFontSize(mNode->getContentSize().height / 2.2f);
	mNode->number->setAnchorPoint(Vec2(0.5f, 0.15f)); // magic number (cause: resource)
	mNode->number->setPosition(mNode->getContentSize() / 2);
	mNode->addChild(mNode->number);
	mNode->number->setVisible(false);

	mNode->numberUseLb = Label::createWithBMFont("fonts/font_life-export.fnt", StringUtils::format("%i", mNode->numberOfUseHasNotUsedYet));
	mNode->numberUseLb->setBMFontSize(mNode->getContentSize().height / 2.2f);
	mNode->numberUseLb->setAnchorPoint(Vec2(0.5f, 0.15f));
	mNode->numberUseLb->setVisible(false);
	mNode->numberUseLb->setPosition(mNode->getContentSize().width * 0.7f, mNode->getContentSize().height * 0.3f);
	mNode->addChild(mNode->numberUseLb);


	mNode->isBlocked = false;
	mNode->canTouch = true;
	mNode->setIsActive(false);
	mNode->autorelease();
	//mNode->addEvents();
	return mNode;
}


void MyButton::addNumberOfUse(int number)
{
	setNumberOfUse(number);
	setNumberOfUseHasNotUsedYet(number);
	if (number > 1) {
		numberUseLb->setString(StringUtils::format("%i", number));
		numberUseLb->setVisible(true);
	}
}

// add listener to sprite
void MyButton::addEvents()
{
	//auto originX = Director::getInstance()->getVisibleOrigin().x;
	listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);								// preventing other listener from using it

	listener->onTouchBegan = [&](Touch *mTouch, Event *mEvent)
	{
		auto location = mTouch->getLocation();
		auto p = Vec2(location);
		//convertToNodeSpace(p);
		Rect rect = this->getBoundingBox();
		
		if (rect.containsPoint(p) && !isBlocked)	// if this button is blocked (smt while another button is active), cannot active
		{	
			if (canTouch) {
				--numberOfUseHasNotUsedYet;
				
				if (numberOfUseHasNotUsedYet <= 0) {
					if(numberOfUse > 1)
						numberUseLb->setVisible(false);
					runTimer();
					this->schedule(schedule_selector(MyButton::checkInterval), timeCoolDown, 1, 0);
				}

				if(numberOfUseHasNotUsedYet >= 1)
					numberUseLb->setString(StringUtils::format("%i", numberOfUseHasNotUsedYet));
							
				canTouch = false;
				isActive = true;
			}
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MyButton::pauseListener()
{
	main->setVisible(false);
	number->setVisible(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
	listener = nullptr;
}

void MyButton::refresh()
{
	canTouch = true;
	isActive = false;
	isBlocked = false;
	number->setVisible(false);
	main->setVisible(false);
	main->setPercentage(100.0f);
	numberOfUseHasNotUsedYet = numberOfUse;
	if (numberOfUse > 1) {
		numberUseLb->setVisible(true);
		numberUseLb->setString(StringUtils::format("%i", numberOfUseHasNotUsedYet));
	}
	this->unscheduleAllCallbacks();
}

void MyButton::silence()
{
	if (!canTouch) {		// schedule
		this->unschedule("Key_timer");
		this->unschedule(schedule_selector(MyButton::checkInterval));
		isActive = false;
	}

	if (isBlocked) {		// another is using
		if (!main->isVisible())
			main->setVisible(true);

		isBlocked = false;
	}

	numberOfUseHasNotUsedYet = 0;

	if (numberOfUseHasNotUsedYet <= 0) {
		if (numberOfUse > 1)
			numberUseLb->setVisible(false);
		runTimer();
		this->schedule(schedule_selector(MyButton::checkInterval), timeCoolDown, 1, 0);
	}

	if (numberOfUseHasNotUsedYet >= 1)
		numberUseLb->setString(StringUtils::format("%i", numberOfUseHasNotUsedYet));

	canTouch = false;
}

void MyButton::checkInterval(float dt)
{
	this->unschedule(schedule_selector(MyButton::checkInterval));
	numberOfUseHasNotUsedYet = numberOfUse;		// re_life
	
	if (numberOfUse > 1) {
		numberUseLb->setVisible(true);
		numberUseLb->setString(StringUtils::format("%i", numberOfUseHasNotUsedYet));
	}
	
	canTouch = true;
	isActive = false;
}

void MyButton::runTimer()
{
	timer = timeCoolDown;
	if (timeCoolDown < 1) {
		number->setVisible(false);
	} else {
		number->setVisible(true);
		number->setString(StringUtils::format("%i", (int)timer));
	}
	
	this->schedule([&](float dt) {
		timer -= 0.01f;
		main->setPercentage((timeCoolDown - timer) / timeCoolDown * 100.0f);
		if(timer > 1)
			number->setString(StringUtils::format("%i", (int)timer));

		if ((int)timer <= 0) {
			number->setVisible(false);
		}
		if (timer <= 0.0f) {
			this->unschedule("Key_timer");
		}
	}, 0.01f, "Key_timer");

}

void MyButton::runSpecialTimer(float dt)
{
	this->unschedule(schedule_selector(MyButton::runSpecialTimer));
	numberOfUseHasNotUsedYet++;		// increase 

	numberUseLb->setString(StringUtils::format("%i", numberOfUseHasNotUsedYet));
}

