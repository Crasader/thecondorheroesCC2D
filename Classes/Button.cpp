#include "Button.h"

Button::Button() {

}

Button * Button::create(string file_name_main, string file_name_CoolDown, Point pos)
{
	Button *mNode = new Button();
	mNode->initWithFile(file_name_main);
	mNode->coolDown = Sprite::create(file_name_CoolDown);

	mNode->setPosition(pos);
	mNode->coolDown->setPosition(pos);
	mNode->coolDown->setVisible(false);

	mNode->isBlocked = false;
	mNode->canTouch = true;
	mNode->setIsActive(false);
	mNode->addEvents();
	return mNode;
}


// add listener to sprite
void Button::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);								// preventing other listener from using it

	listener->onTouchBegan = [&](Touch *mTouch, Event *mEvent)
	{
		auto p = mTouch->getLocation();
		convertToNodeSpace(p);
		Rect rect = this->getBoundingBox();
		
		if (rect.containsPoint(p) && !isBlocked)	// if this button is blocked (smt while another button is active), cannot active
		{	
			if (canTouch) {
				coolDown->setVisible(true);
				this->schedule(schedule_selector(Button::checkInterval), timeCoolDown, 1, 0);
				canTouch = false;
				isActive = true;
			}
			else {
				log("Cool Down, B*tch");
			}
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Button::checkInterval(float dt)
{
	this->unschedule(schedule_selector(Button::checkInterval));
	canTouch = true;
	isActive = false;
	coolDown->setVisible(false);
}

