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
	btnAttack = Button::create("UI/btn_attack.png", Point(SCREEN_SIZE.width * 0.85f, SCREEN_SIZE.height * 0.165f));
	btnAttack->setTimeCoolDown(0.3f);
	btnAttack->setScale(SCREEN_SIZE.height / 6.3f / btnAttack->getContentSize().height);
	addChild(btnAttack);

	/*btnSkill_1 = Button::create("ui/btn_jump.png", Point(SCREEN_SIZE.width * 0.3f, SCREEN_SIZE.height * 0.165f));
	btnSkill_1->setTimeCoolDown(0.77f);
	btnSkill_1->setScale(SCREEN_SIZE.height / 5.5f / btnSkill_1->getContentSize().height);
	addChild(btnSkill_1);*/
}
