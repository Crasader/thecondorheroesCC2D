#include "Hud.h"

bool Hud::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	addButton();

	return true;
}


void Hud::addButton()
{
	btnAttack = Button::create("ui/btn_attack.png", Point(SCREEN_SIZE.width * 0.15f, SCREEN_SIZE.height * 0.165f));
	btnAttack->setTimeCoolDown(3.0f);
	btnAttack->setScale(SCREEN_SIZE.height / 5.5f / btnAttack->getContentSize().height);
	addChild(btnAttack);

	btnSKill_1 = Button::create("ui/btn_jump.png", Point(SCREEN_SIZE.width * 0.3f, SCREEN_SIZE.height * 0.165f));
	btnSKill_1->setTimeCoolDown(0.77f);
	btnSKill_1->setScale(SCREEN_SIZE.height / 5.5f / btnSKill_1->getContentSize().height);
	addChild(btnSKill_1);
}



