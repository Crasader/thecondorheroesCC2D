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

	tmxMap = TMXTiledMap::create("UI/UI_config.tmx");
	tmxMap->setPosition(0, 0);
	tmxMap->setScaleX(winSize.width / tmxMap->getContentSize().width);
	tmxMap->setScaleY(winSize.height / tmxMap->getContentSize().height);
	this->addChild(tmxMap);


	addProfile();
	addButton();

	return true;
}


void Hud::addProfile()
{
	auto groupAvatar = tmxMap->getObjectGroup("avatar");
	for (auto child : groupAvatar->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		avatar = Sprite::create("UI/UI_info_ingame/avatar_DuongQua.png");
		avatar->setScale(SCREEN_SIZE.height / 4.7f / avatar->getContentSize().height);
		avatar->setPosition(origin);

		addChild(avatar);
	}


	auto groupMoney = tmxMap->getObjectGroup("money_board");
	for (auto child : groupMoney->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		moneyBoard = Sprite::create("UI/UI_info_ingame/money_board.png");
		moneyBoard->setScale(SCREEN_SIZE.width / 4.3f / moneyBoard->getContentSize().width);
		moneyBoard->setPosition(origin);

		addChild(moneyBoard);
	}

	auto groupScore = tmxMap->getObjectGroup("score_board");
	for (auto child : groupScore->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		scoreBoard = Sprite::create("UI/UI_info_ingame/score_board.png");
		scoreBoard->setScale(SCREEN_SIZE.width / 3.8f / scoreBoard->getContentSize().width);
		scoreBoard->setPosition(origin);

		addChild(scoreBoard);
	}
	
}

void Hud::addButton()
{
	auto groupAttack = tmxMap->getObjectGroup("btn_attack");
	for (auto child : groupAttack->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		btnAttack = Button::create("UI/btn_attack.png", "UI/btn_attack.png", origin);
		btnAttack->setTimeCoolDown(0.5f);
		btnAttack->setScale(SCREEN_SIZE.height / 4.5f / btnAttack->getContentSize().height);
		btnAttack->getCoolDownSprite()->setScale(btnAttack->getScale());
		addChild(btnAttack);
		addChild(btnAttack->getCoolDownSprite());
	}


	auto groupBtnSkill1 = tmxMap->getObjectGroup("btn_skill1");
	for (auto child : groupBtnSkill1->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		btnSkill_1 = Button::create("UI/btnDQ_skill1_up.png", "UI/btnDQ_skill1_down.png", origin);
		btnSkill_1->setTimeCoolDown(8.77f);
		btnSkill_1->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_1->getContentSize().height);
		btnSkill_1->getCoolDownSprite()->setScale(btnSkill_1->getScale());
		addChild(btnSkill_1);
		addChild(btnSkill_1->getCoolDownSprite());
	}

	auto groupBtnSkill2 = tmxMap->getObjectGroup("btn_skill2");
	for (auto child : groupBtnSkill2->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		btnSkill_2 = Button::create("UI/btnDQ_skill2_up.png", "UI/btnDQ_skill2_down.png", origin);
		btnSkill_2->setTimeCoolDown(7.77f);
		btnSkill_2->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_2->getContentSize().height);
		btnSkill_2->getCoolDownSprite()->setScale(btnSkill_2->getScale());
		addChild(btnSkill_2);
		addChild(btnSkill_2->getCoolDownSprite());
	}

	auto groupBtnSkill3 = tmxMap->getObjectGroup("btn_skill3");
	for (auto child : groupBtnSkill3->getObjects()) {
		auto mObject = child.asValueMap();
		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

		btnSkill_3 = Button::create("UI/btnDQ_skill3_up.png", "UI/btnDQ_skill3_down.png", origin);
		btnSkill_3->setTimeCoolDown(10.77f);
		btnSkill_3->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_3->getContentSize().height);
		btnSkill_3->getCoolDownSprite()->setScale(btnSkill_3->getScale());
		addChild(btnSkill_3);
		addChild(btnSkill_3->getCoolDownSprite());
	}

}
