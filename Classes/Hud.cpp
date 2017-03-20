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
	btnAttack = Button::create("UI/btn_attack.png", "UI/btn_attack.png",
		Point(SCREEN_SIZE.width * 0.9f, SCREEN_SIZE.height * 0.14f));
	btnAttack->setTimeCoolDown(0.5f);
	btnAttack->setScale(SCREEN_SIZE.height / 4.5f / btnAttack->getContentSize().height);
	btnAttack->getCoolDownSprite()->setScale(btnAttack->getScale());
	addChild(btnAttack);
	addChild(btnAttack->getCoolDownSprite());

	btnSkill_1 = Button::create("UI/btnDQ_skill1_up.png", "UI/btnDQ_skill1_down.png",
					Point(btnAttack->getPositionX() - btnAttack->getBoundingBox().size.width,
							btnAttack->getPositionY() - btnAttack->getBoundingBox().size.height / 5));
	btnSkill_1->setTimeCoolDown(8.77f);
	btnSkill_1->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_1->getContentSize().height);
	btnSkill_1->getCoolDownSprite()->setScale(btnSkill_1->getScale());
	addChild(btnSkill_1);
	addChild(btnSkill_1->getCoolDownSprite());


	btnSkill_2 = Button::create("UI/btnDQ_skill2_up.png", "UI/btnDQ_skill2_down.png",
		Point(btnSkill_1->getPositionX() + btnSkill_1->getBoundingBox().size.width / 4,
			btnAttack->getPositionY() + btnAttack->getBoundingBox().size.height * 0.7f));
	btnSkill_2->setTimeCoolDown(7.77f);
	btnSkill_2->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_2->getContentSize().height);
	btnSkill_2->getCoolDownSprite()->setScale(btnSkill_2->getScale());
	addChild(btnSkill_2);
	addChild(btnSkill_2->getCoolDownSprite());


	btnSkill_3 = Button::create("UI/btnDQ_skill3_up.png", "UI/btnDQ_skill3_down.png",
		Point(btnAttack->getPositionX() + btnAttack->getBoundingBox().size.width / 7,
			btnSkill_2->getPositionY() + btnSkill_2->getBoundingBox().size.height / 2.5f));
	btnSkill_3->setTimeCoolDown(10.77f);
	btnSkill_3->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_3->getContentSize().height);
	btnSkill_3->getCoolDownSprite()->setScale(btnSkill_3->getScale());
	addChild(btnSkill_3);
	addChild(btnSkill_3->getCoolDownSprite());
}
