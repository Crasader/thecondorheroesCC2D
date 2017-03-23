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
	auto mObject_1 = groupAvatar->getObject("avatar");
	Point origin_1 = Point(mObject_1["x"].asFloat() * tmxMap->getScaleX(), mObject_1["y"].asFloat()* tmxMap->getScaleY());

	avatar = Sprite::create("UI/UI_info_ingame/avatar_DuongQua.png");
	avatar->setScale(SCREEN_SIZE.height / 4.7f / avatar->getContentSize().height);
	avatar->setPosition(origin_1);

	addChild(avatar, 1);


	bloodBoard = Sprite::create("UI/UI_info_ingame/blood_board.png");
	bloodBoard->setAnchorPoint(Vec2::ZERO);
	bloodBoard->setScale(SCREEN_SIZE.width / 4 / bloodBoard->getContentSize().width);
	bloodBoard->setPosition(avatar->getPositionX() + avatar->getBoundingBox().size.width * 0.2f, 
							avatar->getPositionY() - avatar->getBoundingBox().size.height * 0.1f);

	addChild(bloodBoard);


	auto groupMoney = tmxMap->getObjectGroup("money_board");
	auto mObject_2 = groupMoney->getObject("money_board");
	float pos_2X = mObject_2["x"].asFloat() * tmxMap->getScaleX();

	moneyBoard = Sprite::create("UI/UI_info_ingame/money_board.png");
	moneyBoard->setAnchorPoint(Vec2::ZERO);
	moneyBoard->setScale(SCREEN_SIZE.height / 10 / moneyBoard->getContentSize().height);
	moneyBoard->setPosition(pos_2X, bloodBoard->getPositionY());

	addChild(moneyBoard);

	auto groupScore = tmxMap->getObjectGroup("score_board");
	auto mObject_3 = groupScore->getObject("score_board");
	float pos_3X = mObject_3["x"].asFloat() * tmxMap->getScaleX();

	scoreBoard = Sprite::create("UI/UI_info_ingame/score_board.png");
	scoreBoard->setAnchorPoint(Vec2::ZERO);
	scoreBoard->setScale(SCREEN_SIZE.height / 9 / scoreBoard->getContentSize().height);
	scoreBoard->setPosition(pos_3X, bloodBoard->getPositionY());

	addChild(scoreBoard);

	auto groupDistanceBar = tmxMap->getObjectGroup("distance_bar");
	auto mObject_4 = groupDistanceBar->getObject("distance_bar");
	Point origin_4 = Point(mObject_4["x"].asFloat() * tmxMap->getScaleX(), mObject_4["y"].asFloat()* tmxMap->getScaleY());

	distanceBar = Sprite::create("UI/UI_info_ingame/distance.png");
	distanceBar->setScale(SCREEN_SIZE.width * 0.5f / distanceBar->getContentSize().width);
	distanceBar->setPosition(origin_4);

	addChild(distanceBar);

	characterPoint = Sprite::create("UI/UI_info_ingame/character_point.png");
	characterPoint->setScale(SCREEN_SIZE.height / 8 / characterPoint->getContentSize().width);
	characterPoint->setPosition(distanceBar->getPositionX() - distanceBar->getBoundingBox().size.width * 0.41f, 
								distanceBar->getPositionY());

	addChild(characterPoint);

}

void Hud::addButton()
{
	auto groupAttack = tmxMap->getObjectGroup("btn_attack");
	auto mObject = groupAttack->getObject("btn_attack");
	Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());

	btnAttack = Button::create("UI/btn_attack.png", "UI/btn_attack.png", origin);
	btnAttack->setTimeCoolDown(0.5f);
	btnAttack->setScale(SCREEN_SIZE.height / 4.5f / btnAttack->getContentSize().height);
	btnAttack->getCoolDownSprite()->setScale(btnAttack->getScale());
	btnAttack->getNumberCoolDown()->setScale(btnAttack->getScale() / 5);
	addChild(btnAttack);
	addChild(btnAttack->getCoolDownSprite());


	auto groupBtnSkill1 = tmxMap->getObjectGroup("btn_skill1");
	auto mObject_1 = groupBtnSkill1->getObject("btn_skill1");
	Point origin_1 = Point(mObject_1["x"].asFloat() * tmxMap->getScaleX(), mObject_1["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_1 = Button::create("UI/btnDQ_skill1_up.png", "UI/btnDQ_skill1_down.png", origin_1);
	btnSkill_1->setTimeCoolDown(9);
	btnSkill_1->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_1->getContentSize().height);
	btnSkill_1->getCoolDownSprite()->setScale(btnSkill_1->getScale());
	btnSkill_1->getNumberCoolDown()->setScale(btnSkill_1->getScale() / 5);
	addChild(btnSkill_1);
	addChild(btnSkill_1->getCoolDownSprite());


	auto groupBtnSkill2 = tmxMap->getObjectGroup("btn_skill2");
	auto mObject_2 = groupBtnSkill2->getObject("btn_skill2");
	Point origin_2 = Point(mObject_2["x"].asFloat() * tmxMap->getScaleX(), mObject_2["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_2 = Button::create("UI/btnDQ_skill2_up.png", "UI/btnDQ_skill2_down.png", origin_2);
	btnSkill_2->setTimeCoolDown(8);
	btnSkill_2->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_2->getContentSize().height);
	btnSkill_2->getCoolDownSprite()->setScale(btnSkill_2->getScale());
	btnSkill_2->getNumberCoolDown()->setScale(btnSkill_2->getScale() / 5);
	addChild(btnSkill_2);
	addChild(btnSkill_2->getCoolDownSprite());


	auto groupBtnSkill3 = tmxMap->getObjectGroup("btn_skill3");
	auto mObject_3 = groupBtnSkill3->getObject("btn_skill3");
	Point origin_3 = Point(mObject_3["x"].asFloat() * tmxMap->getScaleX(), mObject_3["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_3 = Button::create("UI/btnDQ_skill3_up.png", "UI/btnDQ_skill3_down.png", origin_3);
	btnSkill_3->setTimeCoolDown(11);
	btnSkill_3->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_3->getContentSize().height);
	btnSkill_3->getCoolDownSprite()->setScale(btnSkill_3->getScale());
	btnSkill_3->getNumberCoolDown()->setScale(btnSkill_3->getScale() / 5);
	addChild(btnSkill_3);
	addChild(btnSkill_3->getCoolDownSprite());

	auto groupBtnCalling = tmxMap->getObjectGroup("btn_calling");
	auto mObject_4 = groupBtnCalling->getObject("btn_calling");
	Point origin_4 = Point(mObject_4["x"].asFloat() * tmxMap->getScaleX(), mObject_4["y"].asFloat()* tmxMap->getScaleY());

	btnCalling = Button::create("UI/btn_callbird.png", "UI/btn_callbird_off.png", origin_4);
	btnCalling->setTimeCoolDown(40);
	btnCalling->setScale(SCREEN_SIZE.height / 6 / btnCalling->getContentSize().height);
	btnCalling->getCoolDownSprite()->setScale(btnCalling->getScale());
	btnCalling->getNumberCoolDown()->setScale(btnCalling->getScale() / 5);
	addChild(btnCalling);
	addChild(btnCalling->getCoolDownSprite());

}
