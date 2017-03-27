#include "Hud.h"
#include "JSonHeroManager.h"


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

	createBloodBar();

	return true;
}


void Hud::addProfile()
{
	auto groupAvatar = tmxMap->getObjectGroup("avatar");
	auto mObject_1 = groupAvatar->getObject("avatar");
	Point origin_1 = Point(mObject_1["x"].asFloat() * tmxMap->getScaleX(), mObject_1["y"].asFloat()* tmxMap->getScaleY());

	avatar = Sprite::create(JSHERO->getavatarPath());
	avatar->setScale(SCREEN_SIZE.height / 4.7f / avatar->getContentSize().height);
	avatar->setPosition(origin_1);
	addChild(avatar, 1);

	// BLOOD BAR
	bloodBoard = Sprite::create("UI/UI_info_ingame/blood_board.png");
	bloodBoard->setAnchorPoint(Vec2::ZERO);
	bloodBoard->setScale(SCREEN_SIZE.width / 4 / bloodBoard->getContentSize().width);
	bloodBoard->setPosition(avatar->getPositionX() + avatar->getBoundingBox().size.width * 0.2f, 
							avatar->getPositionY() - avatar->getBoundingBox().size.height * 0.1f);
	addChild(bloodBoard);

	// MONEY BOARD HERE
	auto groupMoney = tmxMap->getObjectGroup("money_board");
	auto mObject_2 = groupMoney->getObject("money_board");
	float pos_2X = mObject_2["x"].asFloat() * tmxMap->getScaleX();

	moneyBoard = Sprite::create("UI/UI_info_ingame/money_board.png");
	moneyBoard->setAnchorPoint(Vec2::ZERO);
	moneyBoard->setScale(SCREEN_SIZE.height / 10 / moneyBoard->getContentSize().height);
	moneyBoard->setPosition(pos_2X, bloodBoard->getPositionY());
	addChild(moneyBoard);

	lbMoney = Label::createWithTTF("0", "fonts/BAUHS93.TTF", 200);
	lbMoney->setAnchorPoint(Vec2::ZERO);
	lbMoney->setScale(moneyBoard->getBoundingBox().size.height / lbMoney->getContentSize().height);
	lbMoney->setPosition(moneyBoard->getPositionX() + 1.4f * moneyBoard->getBoundingBox().size.width, 
						moneyBoard->getPositionY());
	addChild(lbMoney);


	// SCORE BOARD HERE
	auto groupScore = tmxMap->getObjectGroup("score_board");
	auto mObject_3 = groupScore->getObject("score_board");
	float pos_3X = mObject_3["x"].asFloat() * tmxMap->getScaleX();

	scoreBoard = Sprite::create("UI/UI_info_ingame/score_board.png");
	scoreBoard->setAnchorPoint(Vec2::ZERO);
	scoreBoard->setScale(SCREEN_SIZE.height / 9 / scoreBoard->getContentSize().height);
	scoreBoard->setPosition(pos_3X, bloodBoard->getPositionY());
	addChild(scoreBoard);

	lbScore = Label::createWithTTF("0", "fonts/BAUHS93.TTF", 200);
	lbScore->setAnchorPoint(Vec2::ZERO);
	lbScore->setScale(lbMoney->getScale());
	lbScore->setPosition(scoreBoard->getPositionX() + 1.2f * scoreBoard->getBoundingBox().size.width, 
						scoreBoard->getPositionY());
	addChild(lbScore);


	// DISTANCE BAR + CHARACTER POINT
	auto groupDistanceBar = tmxMap->getObjectGroup("distance_bar");
	auto mObject_4 = groupDistanceBar->getObject("distance_bar");
	Point origin_4 = Point(mObject_4["x"].asFloat() * tmxMap->getScaleX(), mObject_4["y"].asFloat()* tmxMap->getScaleY());

	distanceBar = Sprite::create("UI/UI_info_ingame/distance.png");
	distanceBar->setScale(SCREEN_SIZE.width * 0.5f / distanceBar->getContentSize().width);
	distanceBar->setPosition(origin_4);

	addChild(distanceBar);

	characterPoint = Sprite::create(JSHERO->getCharacterPointPath());
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
	btnAttack->setTimeCoolDown(0.33f);
	btnAttack->setScale(SCREEN_SIZE.height / 4.5f / btnAttack->getContentSize().height);
	btnAttack->getCoolDownSprite()->setScale(btnAttack->getScale());
	addChild(btnAttack);
	addChild(btnAttack->getCoolDownSprite());


	auto groupBtnSkill1 = tmxMap->getObjectGroup("btn_skill1");
	auto mObject_1 = groupBtnSkill1->getObject("btn_skill1");
	Point origin_1 = Point(mObject_1["x"].asFloat() * tmxMap->getScaleX(), mObject_1["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_1 = Button::create(JSHERO->getPathMainImageSkill1(), JSHERO->getPathSubImageSkill1(), origin_1);
	btnSkill_1->setTimeCoolDown(JSHERO->getCoolDownSkill1());
	btnSkill_1->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_1->getContentSize().height);
	btnSkill_1->getCoolDownSprite()->setScale(btnSkill_1->getScale());
	btnSkill_1->getNumberCoolDown()->setScale(btnSkill_1->getBoundingBox().size.height / 2 / btnSkill_1->getNumberCoolDown()->getContentSize().height);
	addChild(btnSkill_1);
	addChild(btnSkill_1->getCoolDownSprite());
	addChild(btnSkill_1->getNumberCoolDown());


	auto groupBtnSkill2 = tmxMap->getObjectGroup("btn_skill2");
	auto mObject_2 = groupBtnSkill2->getObject("btn_skill2");
	Point origin_2 = Point(mObject_2["x"].asFloat() * tmxMap->getScaleX(), mObject_2["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_2 = Button::create(JSHERO->getPathMainImageSkill2(), JSHERO->getPathSubImageSkill2(), origin_2);
	btnSkill_2->setTimeCoolDown(JSHERO->getCoolDownSkill2());
	btnSkill_2->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_2->getContentSize().height);
	btnSkill_2->getCoolDownSprite()->setScale(btnSkill_2->getScale());
	btnSkill_2->getNumberCoolDown()->setScale(btnSkill_2->getBoundingBox().size.height / 2 / btnSkill_2->getNumberCoolDown()->getContentSize().height);
	addChild(btnSkill_2);
	addChild(btnSkill_2->getCoolDownSprite());
	addChild(btnSkill_2->getNumberCoolDown());


	auto groupBtnSkill3 = tmxMap->getObjectGroup("btn_skill3");
	auto mObject_3 = groupBtnSkill3->getObject("btn_skill3");
	Point origin_3 = Point(mObject_3["x"].asFloat() * tmxMap->getScaleX(), mObject_3["y"].asFloat()* tmxMap->getScaleY());

	btnSkill_3 = Button::create(JSHERO->getPathMainImageSkill3(), JSHERO->getPathSubImageSkill3(), origin_3);
	btnSkill_3->setTimeCoolDown(JSHERO->getCoolDownSkill3());
	btnSkill_3->setScale(SCREEN_SIZE.height / 6.5f / btnSkill_3->getContentSize().height);
	btnSkill_3->getCoolDownSprite()->setScale(btnSkill_3->getScale());
	btnSkill_3->getNumberCoolDown()->setScale(btnSkill_3->getBoundingBox().size.height / 2 / btnSkill_3->getNumberCoolDown()->getContentSize().height);
	addChild(btnSkill_3);
	addChild(btnSkill_3->getCoolDownSprite());
	addChild(btnSkill_3->getNumberCoolDown());

	auto groupBtnCalling = tmxMap->getObjectGroup("btn_calling");
	auto mObject_4 = groupBtnCalling->getObject("btn_calling");
	Point origin_4 = Point(mObject_4["x"].asFloat() * tmxMap->getScaleX(), mObject_4["y"].asFloat()* tmxMap->getScaleY());

	btnCalling = Button::create("UI/btn_callbird.png", "UI/btn_callbird_off.png", origin_4);
	btnCalling->setTimeCoolDown(40);
	btnCalling->setScale(SCREEN_SIZE.height / 7 / btnCalling->getContentSize().height);
	btnCalling->getCoolDownSprite()->setScale(btnCalling->getScale());
	btnCalling->getNumberCoolDown()->setScale(btnCalling->getBoundingBox().size.height / 2 / btnCalling->getNumberCoolDown()->getContentSize().height);
	addChild(btnCalling);
	addChild(btnCalling->getCoolDownSprite());
	addChild(btnCalling->getNumberCoolDown());

}


void Hud::createBloodBar()
{
	listBlood = CCArray::createWithCapacity(JSHERO->getBaseHP());
	listBlood->retain();

	int arraySize = listBlood->capacity();
	for (int i = 0; i < arraySize; ++i) {
		auto blood = Sprite::create("UI/UI_info_ingame/blood.png");
		blood->setAnchorPoint(Vec2::ZERO);
		blood->setScaleX(bloodBoard->getBoundingBox().size.width * (8.0f / 11.4f) / arraySize / blood->getContentSize().width);
		blood->setScaleY(bloodBoard->getBoundingBox().size.height * 0.4f / blood->getContentSize().height);
		blood->setPosition(Point(i * blood->getBoundingBox().size.width + bloodBoard->getBoundingBox().size.width / 11 + bloodBoard->getPositionX(),
								bloodBoard->getPositionY() + bloodBoard->getBoundingBox().size.height * 0.25f));
		addChild(blood, 1);
		listBlood->addObject(blood);
	}
}
