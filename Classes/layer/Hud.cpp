#include "Hud.h"
#include "GameScene.h"
#include "manager/JSonHeroManager.h"
#include "manager/RefManager.h"


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

	if ((winSize.width / winSize.height) < 1.5f) is43Ratio = true;

	tmxMap = TMXTiledMap::create("UI/UI_config.tmx");
	tmxMap->setPosition(Vec2::ZERO);
	tmxMap->setScaleX(winSize.width / tmxMap->getContentSize().width);
	tmxMap->setScaleY(winSize.height / tmxMap->getContentSize().height);
	tmxMap->setVisible(false);
	this->addChild(tmxMap);


	addProfile();
	createBloodBar();

	blur = LayerColor::create(Color4B(0, 0, 0, 170));
	blur->setVisible(false);
	addChild(blur, 1);
	addButton();

	btnCallingHintDone = false;


	return true;
}

void Hud::addEvents()
{
	btnAttack->addEvents();
	btnSkill_1->addEvents();
	btnSkill_2->addEvents();
	btnSkill_3->addEvents();
}


void Hud::addProfile()
{
	float scaleRatio = 1.0f;
	if (is43Ratio)
		scaleRatio = 1.3f;

	auto groupAvatar = tmxMap->getObjectGroup("avatar");
	auto mObject_1 = groupAvatar->getObject("avatar");
	Point origin_1 = Point(mObject_1["x"].asFloat() * tmxMap->getScaleX(),
		mObject_1["y"].asFloat()* tmxMap->getScaleY());


	auto avatar = Sprite::createWithSpriteFrameName(JSHERO->getavatarPath());
	
	avatar->setScale(SCREEN_SIZE.height / (4.7f * scaleRatio) / avatar->getContentSize().height);
	avatar->setPosition(origin_1);
	addChild(avatar, 2);


	// BLOOD BAR
	bloodBoard = Sprite::createWithSpriteFrameName("blood_board.png");
	bloodBoard->setAnchorPoint(Vec2::ZERO);
	bloodBoard->setScale(SCREEN_SIZE.width / 4 / bloodBoard->getContentSize().width);
	bloodBoard->setPosition(avatar->getPositionX() + avatar->getBoundingBox().size.width * 0.2f,
		avatar->getPositionY() - avatar->getBoundingBox().size.height * 0.1f);
	addChild(bloodBoard);

	// MONEY BOARD HERE
	auto groupMoney = tmxMap->getObjectGroup("money_board");
	auto mObject_2 = groupMoney->getObject("money_board");
	float pos_2X = mObject_2["x"].asFloat() * tmxMap->getScaleX();

	auto moneyBoard = Sprite::createWithSpriteFrameName("money_board.png");
	moneyBoard->setAnchorPoint(Vec2::ZERO);
	moneyBoard->setScale(SCREEN_SIZE.height / (10 * scaleRatio) / moneyBoard->getContentSize().height);
	moneyBoard->setPosition(pos_2X, bloodBoard->getPositionY());

	addChild(moneyBoard);

	lbMoney = Label::createWithBMFont("fonts/font_coin-export.fnt", "0");
	lbMoney->setAnchorPoint(Vec2::ZERO);
	lbMoney->setBMFontSize(moneyBoard->getBoundingBox().size.height * 0.6f / (scaleRatio - 0.15f));
	lbMoney->setPosition(moneyBoard->getPositionX() + 1.35f * moneyBoard->getBoundingBox().size.width,
		moneyBoard->getPositionY() + moneyBoard->getBoundingBox().size.height * 0.25f);
	addChild(lbMoney);


	// SCORE BOARD HERE
	auto groupScore = tmxMap->getObjectGroup("score_board");
	auto mObject_3 = groupScore->getObject("score_board");
	float pos_3X = mObject_3["x"].asFloat() * tmxMap->getScaleX();

	scoreBoard = Sprite::createWithSpriteFrameName("score_board.png");
	scoreBoard->setAnchorPoint(Vec2::ZERO);
	scoreBoard->setScale(SCREEN_SIZE.height / (9 * scaleRatio) / scoreBoard->getContentSize().height);
	scoreBoard->setPosition(pos_3X, bloodBoard->getPositionY());

	addChild(scoreBoard);

	lbScore = Label::createWithBMFont("fonts/font_diamond-export.fnt", "0");
	lbScore->setAnchorPoint(Vec2::ZERO);
	lbScore->setBMFontSize(lbMoney->getBMFontSize());
	lbScore->setPosition(scoreBoard->getPositionX() + 1.15f * scoreBoard->getBoundingBox().size.width,
		lbMoney->getPositionY());
	addChild(lbScore);


	// DISTANCE BAR + CHARACTER POINT
	auto groupDistanceBar = tmxMap->getObjectGroup("distance_bar");
	auto mObject_4 = groupDistanceBar->getObject("distance_bar");
	Point origin_4 = Point(SCREEN_SIZE.width * 0.4f, mObject_4["y"].asFloat()* tmxMap->getScaleY());

	distanceBar = Sprite::create("UI/UI_info_ingame/distance.png");
	distanceBar->setScale(SCREEN_SIZE.width * 0.5f / distanceBar->getContentSize().width);
	distanceBar->setPosition(origin_4);


	addChild(distanceBar);

	characterPoint = Sprite::createWithSpriteFrameName(JSHERO->getCharacterPointPath());
	characterPoint->setScale(SCREEN_SIZE.height / (8 * scaleRatio) / characterPoint->getContentSize().width);
	characterPoint->setPosition(distanceBar->getPositionX() - distanceBar->getBoundingBox().size.width * 0.41f,
		distanceBar->getPositionY());

	addChild(characterPoint);

}

void Hud::addButton()
{
	float scaleRatio = 1.0f;
	if (is43Ratio)
		scaleRatio = 1.3f;

	menu = Menu::create();

	if (!REF->getIsFirstPlay() && !REF->getIsLockedHero()) {
		addAttack();

		addSkills();
		if (REF->getNumberItemBird() > 0) {
			addBird();
			menu->addChild(btnCalling);
		}
	}

	Point origin_5 = Point(SCREEN_SIZE.width - (SCREEN_SIZE.height - scoreBoard->getPositionY()), scoreBoard->getPositionY());

	auto pauseItemDisable = Sprite::create("UI/btn_pause.png");
	pauseItemDisable->setColor(Color3B(128, 128, 128));
	pauseItem = MenuItemImage::create("UI/btn_pause.png", "UI/btn_pause.png", CC_CALLBACK_0(Hud::doPause, this));
	pauseItem->setDisabledImage(pauseItemDisable);
	pauseItem->setAnchorPoint(Vec2::ZERO);
	pauseItem->setScale(scoreBoard->getBoundingBox().size.height / scaleRatio / pauseItem->getContentSize().height);
	pauseItem->setPosition(origin_5);

	//showSpecialButton();

	menu->addChild(pauseItem);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	multiKills = SkeletonAnimation::createWithFile("Effect/textkill.json", "Effect/textkill.atlas", 0.8f);
	multiKills->setPosition(Vec2(SCREEN_SIZE.width / 2, SCREEN_SIZE.height * 0.8f));
	multiKills->setAnimation(0, "textkill", false);
	multiKills->setSkin("default");
	addChild(multiKills, 1);

	coverSkill = Sprite::createWithSpriteFrameName(JSHERO->getPathMainImageSkill1());
	coverItemMagnet = Sprite::createWithSpriteFrameName("item3_magnet.png");
	coverItemDC = Sprite::createWithSpriteFrameName("item4_doublecoin.png");

	auto groupIcon = tmxMap->getObjectGroup("icon");
	for (auto child : groupIcon->getObjects()) {
		auto mObjectX = child.asValueMap();

		Point origin_X = Point(mObjectX["x"].asFloat() * tmxMap->getScaleX(), mObjectX["y"].asFloat()* tmxMap->getScaleY());

		switch (mObjectX["order"].asInt())
		{
		case 1:
			icon_Skill = ProgressTimer::create(coverSkill);
			icon_Skill->setPosition(coverSkill->getContentSize() / 2);
			icon_Skill->setPercentage(100.0f);
			icon_Skill->setReverseDirection(true);
			icon_Skill->setType(ProgressTimer::Type::RADIAL);

			coverSkill->addChild(icon_Skill);

			coverSkill->setOpacity(50);
			coverSkill->setPosition(origin_X);
			coverSkill->setVisible(false);
			coverSkill->setScale(SCREEN_SIZE.height / (12.5f * scaleRatio) / coverSkill->getContentSize().height);

			addChild(coverSkill);

			break;

		case 2:
			icon_Item_Magnet = ProgressTimer::create(Sprite::createWithSpriteFrameName("item3_magnet.png"));
			icon_Item_Magnet->setPosition(coverItemMagnet->getContentSize() / 2);
			icon_Item_Magnet->setPercentage(100.0f);
			icon_Item_Magnet->setReverseDirection(true);
			icon_Item_Magnet->setType(ProgressTimer::Type::RADIAL);

			coverItemMagnet->addChild(icon_Item_Magnet);

			coverItemMagnet->setOpacity(50);
			coverItemMagnet->setPosition(origin_X);
			coverItemMagnet->setVisible(false);
			coverItemMagnet->setScale(SCREEN_SIZE.height / (12.0f * scaleRatio) / coverItemMagnet->getContentSize().height);

			addChild(coverItemMagnet);

			break;
		case 3:
			icon_Item_DC = ProgressTimer::create(Sprite::createWithSpriteFrameName("item4_doublecoin.png"));
			icon_Item_DC->setPosition(coverItemMagnet->getContentSize() / 2);
			icon_Item_DC->setPercentage(100.0f);
			icon_Item_DC->setReverseDirection(true);
			icon_Item_DC->setType(ProgressTimer::Type::RADIAL);

			coverItemDC->addChild(icon_Item_DC);

			coverItemDC->setOpacity(50);
			coverItemDC->setPosition(origin_X);
			coverItemDC->setVisible(false);
			coverItemDC->setScale(SCREEN_SIZE.height / (12.0f * scaleRatio) / coverItemDC->getContentSize().height);

			addChild(coverItemDC);
			break;
		}
	}

}


void Hud::createBloodBar()
{
	int baseHP = REF->getCurrentHealth();
	if (!REF->getIsLockedHero() && REF->getNumberItemHealth() > 0) {
		baseHP++;
		REF->setUpNumberQuest(INDEX_QUEST_HEALTH, 1);
		REF->decreaseNumberItemHealth();
	}

	listBlood = CCArray::createWithCapacity(baseHP);
	listBlood->retain();

	int arraySize = listBlood->capacity();
	for (int i = 0; i < arraySize; ++i) {
		auto blood = Sprite::createWithSpriteFrameName("blood.png");
		blood->setAnchorPoint(Vec2::ZERO);
		blood->setScaleX(bloodBoard->getBoundingBox().size.width * (8.0f / 11.4f) / arraySize / blood->getContentSize().width);
		blood->setScaleY(bloodBoard->getBoundingBox().size.height * 0.35f / blood->getContentSize().height);
		blood->setPosition(Point(i * blood->getBoundingBox().size.width + bloodBoard->getBoundingBox().size.width / 11 + bloodBoard->getPositionX(),
			bloodBoard->getPositionY() + bloodBoard->getBoundingBox().size.height * 0.33f));
		addChild(blood, 1);
		listBlood->addObject(blood);
	}
}

void Hud::addAttack()
{
	float scaleRatio = 1.0f;
	if (is43Ratio)
		scaleRatio = 1.3f;
	float posY = SCREEN_SIZE.height * 0.165f / scaleRatio;
	Point origin = Point(SCREEN_SIZE.width - posY, posY);

	btnAttack = MyButton::create(JSHERO->getPathButtonAttack(), "attack_1b.png", origin);
	btnAttack->getMain()->setScale(btnAttack->getContentSize().height * 0.925f / btnAttack->getMain()->getContentSize().height);
	btnAttack->setTimeCoolDown(0.33f);
	btnAttack->setScale(SCREEN_SIZE.height / (3.7f * scaleRatio) / btnAttack->getContentSize().height);
	addChild(btnAttack, 2);
}

void Hud::addSkills()
{
	float scaleRatio = 1.0f;
	if (is43Ratio)
		scaleRatio = 1.3f;

	auto btnAttackPos = btnAttack->getPosition();
	auto mainPosY = SCREEN_SIZE.height * 0.35f /scaleRatio;
	Point origin_1 = Point(btnAttackPos.x - SCREEN_SIZE.height / (4 * scaleRatio), SCREEN_SIZE.height * 0.135f / scaleRatio);

	btnSkill_1 = MyButton::create(JSHERO->getPathMainImageSkill1(), JSHERO->getPathSubImageSkill1(), origin_1);

	float coolDownS1 = REF->getCoolDownSkill_1();
	float coolDownS2 = REF->getCoolDownSkill_2();
	float coolDownS3 = REF->getCoolDownSkill_3();

	if (!REF->getIsLockedHero() && REF->getNumberItemCoolDown() > 0) {
		coolDownS1 -= coolDownS1 * 0.15f;
		coolDownS2 -= coolDownS2 * 0.15f;
		coolDownS3 -= coolDownS3 * 0.15f;

		REF->setUpNumberQuest(INDEX_QUEST_COOLDOWN, 1);
		REF->decreaseNumberItemCoolDown();
	}

	btnSkill_1->setTimeCoolDown(coolDownS1);
	btnSkill_1->addNumberOfUse(REF->getNumberUseSkill_1());
	btnSkill_1->setScale(SCREEN_SIZE.height / (6.0f * scaleRatio) / btnSkill_1->getContentSize().height);
	addChild(btnSkill_1, 2);

	//////////////////////////////////////////////////////////////////////////
	Point origin_2 = Point(SCREEN_SIZE.width - mainPosY, mainPosY);
	btnSkill_2 = MyButton::create(JSHERO->getPathMainImageSkill2(), JSHERO->getPathSubImageSkill2(), origin_2);

	btnSkill_2->setTimeCoolDown(coolDownS2);
	btnSkill_2->addNumberOfUse(REF->getNumberUseSkill_2());
	btnSkill_2->setScale(SCREEN_SIZE.height / (6.0f * scaleRatio) / btnSkill_2->getContentSize().height);
	addChild(btnSkill_2, 2);

	//////////////////////////////////////////////////////////////////////////

	Point origin_3 = Point(SCREEN_SIZE.width - SCREEN_SIZE.height * 0.135f / scaleRatio, btnAttackPos.y + SCREEN_SIZE.height / (4 *scaleRatio));

	btnSkill_3 = MyButton::create(JSHERO->getPathMainImageSkill3(), JSHERO->getPathSubImageSkill3(), origin_3);

	btnSkill_3->setTimeCoolDown(coolDownS3);
	btnSkill_3->addNumberOfUse(REF->getNumberUseSkill_3());
	btnSkill_3->setScale(SCREEN_SIZE.height / (6.0f * scaleRatio) / btnSkill_3->getContentSize().height);
	addChild(btnSkill_3, 2);
}

void Hud::addBird()
{
	float scaleRatio = 1.0f;
	if (is43Ratio)
		scaleRatio = 1.3f;
	auto groupBtnCalling = tmxMap->getObjectGroup("btn_special");
	auto mObject_4 = groupBtnCalling->getObject("btn_special");
	Point origin_4 = Point(mObject_4["x"].asFloat() * tmxMap->getScaleX(), mObject_4["y"].asFloat()* tmxMap->getScaleY());

	auto btn_bird_normal = Sprite::createWithSpriteFrameName("btn_callbird.png");
	auto btn_bird_active = Sprite::createWithSpriteFrameName("btn_callbird_off.png");
	btnCalling = MenuItemSprite::create(btn_bird_normal, btn_bird_active, CC_CALLBACK_0(Hud::doCalling, this));
	btnCalling->setEnabled(false);
	btnCalling->setDisabledImage(Sprite::createWithSpriteFrameName("btn_callbird_off.png"));
	btnCalling->setPosition(origin_4);
	btnCalling->setScale(SCREEN_SIZE.height / (7 * scaleRatio) / btnCalling->getContentSize().height);
}

//void Hud::doSuctionCoin(Ref * pSender)
//{
//	log("Suction");
//}
//
//void Hud::doDoublingCoin(Ref * pSender)
//{
//	log("Doubling");
//}

void Hud::doCalling()
{
	if(!REF->getIsLockedHero())
		REF->decreaseNumberItemBird();

	btnCalling->setVisible(false);
	btnCalling->setEnabled(false);
	if (btnSkill_1->isVisible())
		hideButton();

	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->callingBird();
}

void Hud::doPause()
{
	//log("do pause");
	auto gameLayer = (GameScene*) this->getParent()->getChildByName("gameLayer");
	gameLayer->pauseGame();
}

//void Hud::showSpecialButton()
//{
//	vector<int> list = getListIndexOfTypeItemBuy();
//	if (!list.empty()) {
//		auto groupBtnSpecial = tmxMap->getObjectGroup("btn_special");
//		auto mObject = groupBtnSpecial->getObject("btn_special");
//		Point origin = Point(mObject["x"].asFloat() * tmxMap->getScaleX(), mObject["y"].asFloat()* tmxMap->getScaleY());
//
//		for (int i = 0; i < list.size(); ++i) {
//			createButtonX(list[i], Point(origin.x, origin.y - i * SCREEN_SIZE.height * 0.15f));
//		}
//	}
//}
//
//void Hud::createButtonX(int index, Point position)
//{
//	switch (index)
//	{
//	case 0:
//		btnCalling = MenuItemImage::create("UI/btn_callbird.png", "UI/btn_callbird_off.png", CC_CALLBACK_1(Hud::doCalling, this));
//		btnCalling->setEnabled(false);
//		btnCalling->setPosition(position);
//		btnCalling->setScale(SCREEN_SIZE.height / 7 / btnCalling->getContentSize().height);
//		menu->addChild(btnCalling);
//		break;
//
//		/*case 1:
//			btnMagnet = MenuItemImage::create("UI/btn_callbird.png", "UI/btn_callbird_off.png", CC_CALLBACK_1(Hud::doSuctionCoin, this));
//			btnMagnet->setEnabled(false);
//			btnMagnet->setPosition(position);
//			btnMagnet->setScale(SCREEN_SIZE.height / 7 / btnMagnet->getContentSize().height);
//			menu->addChild(btnMagnet);
//			break;
//
//		case 2:
//			btnDouleGold = MenuItemImage::create("UI/btn_callbird.png", "UI/btn_callbird_off.png", CC_CALLBACK_1(Hud::doDoublingCoin, this));
//			btnDouleGold->setEnabled(false);
//			btnDouleGold->setPosition(position);
//			btnDouleGold->setScale(SCREEN_SIZE.height / 7 / btnDouleGold->getContentSize().height);
//			menu->addChild(btnDouleGold);
//			break;*/
//	}
//}
//
//vector<int> Hud::getListIndexOfTypeItemBuy()
//{
//	vector<int> list;
//	if (REF->getNumberItemBird() > 0) {
//		list.push_back(0);
//	}
//
//	if (REF->getNumberItemMagnet() > 0) {
//		list.push_back(1);
//	}
//
//	if (REF->getNumberItemDoubleGold() > 0) {
//		list.push_back(2);
//	}
//
//	return list;
//}

void Hud::disableBlur()
{
	blur->setVisible(false);
}

void Hud::introAttack()
{
	blur->setVisible(true);
	if (coverSkill->isVisible()) {
		coverSkill->pause();
	}

	if (coverItemMagnet->isVisible()) {
		coverItemMagnet->pause();
	}

	if (coverItemDC->isVisible() && isItemDCActive) {
		coverItemDC->pause();
	}

	addAttack();
	btnAttack->addEvents();
}

void Hud::introSkills()
{
	btnAttack->setZOrder(0);
	blur->setVisible(true);
	if (coverSkill->isVisible()) {
		coverSkill->pause();
	}

	if (coverItemMagnet->isVisible()) {
		coverItemMagnet->pause();
	}

	if (coverItemDC->isVisible() && isItemDCActive) {
		coverItemDC->pause();
	}

	addSkills();
	btnSkill_1->addEvents();
	btnSkill_2->addEvents();
	btnSkill_3->addEvents();
}

void Hud::introBird()
{
	// block skills + attack
	btnSkill_1->setZOrder(0);
	btnSkill_2->setZOrder(0);
	btnSkill_3->setZOrder(0);
	menu->setZOrder(2);
	blur->setVisible(true);

	pauseIfVisible();

	addBird();
	menu->addChild(btnCalling);
}

void Hud::hideButton()
{
	btnAttack->pauseListener();
	btnSkill_1->pauseListener();
	btnSkill_2->pauseListener();
	btnSkill_3->pauseListener();

	// if not pause, in case: calling the bird

	btnAttack->setVisible(false);
	btnAttack->setIsBlocked(true);


	btnSkill_1->setVisible(false);


	btnSkill_2->setVisible(false);


	btnSkill_3->setVisible(false);
}

void Hud::showButton()
{
	addEvents();

	btnAttack->setVisible(true);
	btnAttack->getMain()->setVisible(true);
	btnAttack->setIsBlocked(false);

	btnSkill_1->setVisible(true);
	btnSkill_1->getMain()->setVisible(true);
	if (btnSkill_1->getNumberOfUseHasNotUsedYet() == 0 && !btnSkill_1->getCanTouch()) {
		btnSkill_1->getNumberCoolDown()->setVisible(true);
	}

	btnSkill_2->setVisible(true);
	btnSkill_2->getMain()->setVisible(true);
	if (btnSkill_2->getNumberOfUseHasNotUsedYet() == 0 && !btnSkill_2->getCanTouch()) {
		btnSkill_2->getNumberCoolDown()->setVisible(true);
	}
	
	btnSkill_3->setVisible(true);
	btnSkill_3->getMain()->setVisible(true);
	if (btnSkill_3->getNumberOfUseHasNotUsedYet() == 0 && !btnSkill_3->getCanTouch()) {
		btnSkill_3->getNumberCoolDown()->setVisible(true);
	}
}

void Hud::pauseIfVisible()
{
	if (coverSkill->isVisible()) {
		coverSkill->pause();
	}

	if (coverItemMagnet->isVisible()) {
		coverItemMagnet->pause();
	}

	if (coverItemDC->isVisible() && isItemDCActive) {
		coverItemDC->pause();
	}

	if (btnAttack != nullptr && btnAttack->isVisible())
		btnAttack->pauseListener();

	if (btnSkill_1 != nullptr && btnSkill_1->isVisible()) {

		btnSkill_1->pauseListener();
		btnSkill_2->pauseListener();
		btnSkill_3->pauseListener();

		btnSkill_1->pause();

		btnSkill_2->pause();

		btnSkill_3->pause();
	}
}

void Hud::resumeIfVisible()
{
	if (coverSkill->isVisible()) {
		coverSkill->resume();
	}

	if (coverItemMagnet->isVisible()) {
		coverItemMagnet->resume();
	}

	if (coverItemDC->isVisible() && isItemDCActive) {
		coverItemDC->resume();
	}

	if (btnAttack != nullptr && btnAttack->isVisible()) {
		btnAttack->addEvents();
		btnAttack->getMain()->setVisible(true);
	}

	if (btnSkill_1 != nullptr && btnSkill_1->isVisible()) {

		btnSkill_1->addEvents();
		btnSkill_1->getMain()->setVisible(true);
		if (!btnSkill_1->getCanTouch() && btnSkill_1->getNumberOfUseHasNotUsedYet() == 0) {	// in scheule
			btnSkill_1->getNumberCoolDown()->setVisible(true);
			btnSkill_1->resume();
		}

		btnSkill_2->addEvents();
		btnSkill_2->getMain()->setVisible(true);
		if (!btnSkill_2->getCanTouch() && btnSkill_2->getNumberOfUseHasNotUsedYet() == 0) {
			btnSkill_2->getNumberCoolDown()->setVisible(true);
			btnSkill_2->resume();
		}

		btnSkill_3->addEvents();
		btnSkill_3->getMain()->setVisible(true);
		if (!btnSkill_3->getCanTouch() && btnSkill_3->getNumberOfUseHasNotUsedYet() == 0) {
			btnSkill_3->getNumberCoolDown()->setVisible(true);
			btnSkill_3->resume();
		}
	}
}

void Hud::refreshControl()
{
	if (btnSkill_1 != nullptr) {
		btnSkill_1->refresh();
		btnSkill_2->refresh();
		btnSkill_3->refresh();
	}

	// item
	if (coverSkill->isVisible()) {
		coverSkill->setVisible(false);
		coverSkill->unscheduleAllCallbacks();
	}

	if (coverItemMagnet->isVisible()) {
		isItemMagnetActive = false;
		coverItemMagnet->setVisible(false);
		coverItemMagnet->unscheduleAllCallbacks();
	}

	if (coverItemDC->isVisible() && isItemDCActive) {
		isItemDCActive = false;
		coverItemDC->setVisible(false);
		coverItemDC->unscheduleAllCallbacks();
	}
}

void Hud::moveCallBirdToCenterScreen(Vec2 p_ptCenterScreen)
{

	MoveTo *_pStageSpecialButton = MoveTo::create(0.3f, p_ptCenterScreen);
	auto _aStageSpecialButtonCallback = CallFunc::create([&]() {
		btnCallingHintDone = true;
		btnCalling->runAction(ScaleBy::create(0.3f, 2.0f));
	});

	btnCalling->runAction(Sequence::create(_pStageSpecialButton, _aStageSpecialButtonCallback, nullptr));
	cooldownCallBird();
}

void Hud::cooldownCallBird() {

	float _fRadius = btnCalling->getContentSize().height / 2;
	for (int i = 0; i < 180; i++) {
		auto _aCooldownStep = Sprite::create("UI/green.png");
		_aCooldownStep->setScale(_fRadius / _aCooldownStep->getContentSize().height * 2 * 0.1f);
		float _fAngle = i * 2 * PI / 180.0;
		Vec2 _v2CenterButton = Vec2(btnCalling->getContentSize().height / 2, btnCalling->getContentSize().height / 2);
		_aCooldownStep->setPosition(Vec2(_fRadius * sinf(_fAngle), _fRadius * cosf(_fAngle)) + _v2CenterButton);
		btnCalling->addChild(_aCooldownStep, -1);
		g_lTemp.push_back(_aCooldownStep);
	}
}

bool Hud::callBirdCooldown() {
	if (g_lTemp.empty()) {
		return false;
	}
	btnCalling->removeChild(g_lTemp.front(), true);
	g_lTemp.pop_front();
	return true;
}

void Hud::updateMultiKills(int m_nCombo)
{
	switch (m_nCombo) {
	case 2:
		multiKills->setSkin("doublekill");
		break;
	case 3:
		multiKills->setSkin("triplekill");
		break;
	case 4:
		multiKills->setSkin("quadrakill");
		break;
	case 5:
		multiKills->setSkin("pentakill");
		break;
	case 6:
		multiKills->setSkin("ultrakill");
		break;
	case 7:
		multiKills->setSkin("rampage");
		break;
	default:
		break;
	}
	if (m_nCombo >= 7) {
		if(!REF->getIsLockedHero())
			REF->setUpNumberQuest(INDEX_QUEST_RAMPAGE, 1);
		multiKills->setSkin("rampage");
		m_nCombo = 0;
	}
	multiKills->setSlotsToSetupPose();
	multiKills->setAnimation(0, "textkill", false);
}

void Hud::runnerSkillDuration(int skillWhat, float duration)
{
	durationSkill = duration;
	switch (skillWhat)
	{
	case 1:
		coverSkill->setSpriteFrame(JSHERO->getPathMainImageSkill1());
		icon_Skill->setSprite(Sprite::createWithSpriteFrameName(JSHERO->getPathMainImageSkill1()));
		break;
	case 2:
		coverSkill->setSpriteFrame(JSHERO->getPathMainImageSkill2());
		icon_Skill->setSprite(Sprite::createWithSpriteFrameName(JSHERO->getPathMainImageSkill2()));
		break;
	case 3:
		coverSkill->setSpriteFrame(JSHERO->getPathMainImageSkill3());
		icon_Skill->setSprite(Sprite::createWithSpriteFrameName(JSHERO->getPathMainImageSkill3()));
		break;
	}

	timerSkill = durationSkill;

	coverSkill->setVisible(true);

	coverSkill->schedule([&](float dt) {
		timerSkill -= 0.1f;
		icon_Skill->setPercentage(timerSkill / durationSkill * 100.0f);
		if (timerSkill <= 0.0f) {
			coverSkill->setVisible(false);
			coverSkill->unschedule("durationKey");
		}
	}, 0.1f, "durationKey");

}

void Hud::runnerItemMagnet(int counter)
{
	durationItemMagnet = counter / 60.0f;
	if (isItemMagnetActive) {

		coverItemMagnet->unschedule("itemRunner");
	}
	else {
		coverItemMagnet->setVisible(true);
	}

	isItemMagnetActive = true;

	timerItemMagnet = durationItemMagnet;

	coverItemMagnet->schedule([&](float dt) {
		timerItemMagnet -= 0.1f;
		icon_Item_Magnet->setPercentage(timerItemMagnet / durationItemMagnet * 100.0f);
		if (timerItemMagnet <= 0.0f) {
			isItemMagnetActive = false;
			coverItemMagnet->setVisible(false);
			coverItemMagnet->unschedule("itemRunner");
		}
	}, 0.1f, "itemRunner");
}

void Hud::runnerItemDC(int counter)
{
	if (counter < 0) {
		coverItemDC->setVisible(true);
		return;
	}


	durationItemDC = counter / 60.0f;
	if (isItemDCActive) {
		coverItemDC->unschedule("itemRunnerDC");
	}
	else {
		coverItemDC->setVisible(true);
	}

	isItemDCActive = true;

	timerItemDC = durationItemDC;

	coverItemDC->schedule([&](float dt) {
		timerItemDC -= 0.1f;
		icon_Item_DC->setPercentage(timerItemDC / durationItemDC * 100.0f);
		if (timerItemDC <= 0.0f) {
			isItemDCActive = false;
			coverItemDC->setVisible(false);
			coverItemDC->unschedule("itemRunnerDC");
		}
	}, 0.1f, "itemRunnerDC");
}

void Hud::tryHud()
{
	addAttack();

	addSkills();

	addBird();
	menu->addChild(btnCalling);
}

void Hud::silence()
{
	btnSkill_1->silence();
	btnSkill_2->silence();
	btnSkill_3->silence();
}

void Hud::onExit()
{
	Layer::onExit();
	if (coverSkill->isVisible()) {
		coverSkill->unscheduleAllCallbacks();
	}

	if (coverItemDC->isVisible()) {
		coverItemDC->unscheduleAllCallbacks();
	}

	if (coverItemMagnet->isVisible()) {
		coverItemMagnet->unscheduleAllCallbacks();
	}
}
