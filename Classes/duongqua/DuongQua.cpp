#include "DuongQua.h"
#include "manager/RefManager.h"
#include "layer/GameScene.h"
#include "AudioEngine.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
	checkDurationSkill1 = 0;
	checkDurationSkill2 = 0;
	checkDurationSkill3 = 2;
}

DuongQua * DuongQua::create(string jsonFile, string atlasFile, float scale)
{
	DuongQua* duongQua = new DuongQua(jsonFile, atlasFile, scale);
	duongQua->setTag(TAG_HERO);

	duongQua->update(0.0f);

	duongQua->stateMachine = new StateMachine(duongQua);
	duongQua->stateMachine->setCurrentState(MLand);

	duongQua->setMoveVel(duongQua->SCREEN_SIZE.width / PTM_RATIO / 2.3f);
	duongQua->setJumpVel(duongQua->SCREEN_SIZE.height * 1.4f / PTM_RATIO);

	duongQua->health = REF->getCurrentHealth();
	duongQua->maxHealth = duongQua->health;

	// set Duration here
	duongQua->setDurationSkill1(REF->getDurationSkill_1());
	duongQua->setDurationSkill2(REF->getDurationSkill_2());
	duongQua->setDurationSkill3(REF->getDurationSkill_3());

	duongQua->setBoxHeight(duongQua->getBoundingBox().size.height / 6.7f);
	duongQua->numberOfJump = 2;
	duongQua->coinExplored = 0;
	duongQua->score = 0;


	duongQua->setOnGround(false);
	duongQua->setIsPriorInjured(false);		// future, we need to add props into base class
	duongQua->setIsPriorAttack(false);
	duongQua->setIsPriorSkill1(false);
	duongQua->setIsPriorSkill2(false);
	duongQua->setIsPriorSkill3(false);

	duongQua->setIsDoneDuration1(true);
	duongQua->setIsDoneDuration2(true);
	duongQua->setIsDoneDuration3(true);

	//

	duongQua->blash = Sprite::create("Animation/DuongQua/blash.png");
	duongQua->blash->setScale(scale / 2);
	duongQua->blash->setPosition(duongQua->getContentSize() / 2);
	duongQua->blash->setVisible(false);
	duongQua->addChild(duongQua->blash);

	return duongQua;
}


// SKILL 1
void DuongQua::createToanChanKiemPhap(Point posSword)
{
	auto tckp = (ToanChanKiemPhap*)poolSkill1->getObjectAtIndex(indexSkill1++);
	tckp->setVisible(true);
	auto gameLayer = (GameScene*) this->getParent();

	tckp->setPosition(posSword.x + this->getTrueRadiusOfHero() / 2, posSword.y);
	tckp->initCirclePhysic(gameLayer->world, tckp->getPosition());

	if (!tckp->getIsAdded()) {
		this->getParent()->addChild(tckp, ZORDER_SMT);
		tckp->setIsAdded(true);
	}

	listToanChanKiemPhap.push_back(tckp);

	if (indexSkill1 == 4) indexSkill1 = 0;
}

void DuongQua::slashToanChanKiemPhap()
{
	this->schedule([&](float dt) {
		checkDurationSkill1++;

		if (checkDurationSkill1 >= getDurationSkill1() * 60) {
			setIsDoneDuration1(true);
		}

		// only to update
		if (!listToanChanKiemPhap.empty()) {
			for (auto tckp : listToanChanKiemPhap) {
				if (!tckp->getB2Body()) continue;
				if (tckp->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.255f) > SCREEN_SIZE.width / 2) {
					auto gameLayer = (GameScene*) this->getParent();

					gameLayer->world->DestroyBody(tckp->getB2Body());
					tckp->setB2Body(nullptr);
					tckp->setVisible(false);
				}
				else
					tckp->updateMe();
			}
		}

		// get more time to update
		if (checkDurationSkill1 > getDurationSkill1() * 80) {
			listToanChanKiemPhap.clear();
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}

	}, 1.0f / 60, "KeySkill1");		//  run every delta second
}

void DuongQua::doCounterSkill1()
{
	slashToanChanKiemPhap();
	//fastAndFurious();
}

void DuongQua::fastAndFurious()
{
	
	this->schedule([&](float dt) {
		checkDurationSkill1++;

		this->getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel() * 5, 0));

		if (checkDurationSkill1 >= getDurationSkill1() * 60) {
			auto currentVelY = getB2Body()->GetLinearVelocity().y;
			this->getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));
			setIsDoneDuration1(true);
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}

	}, 1.0f / 60, "KeySkill1");		//  run every delta second
}


// SKILL 2
void DuongQua::createKiemPhap(float posX)
{
	AudioManager::playSound(SOUND_DQSKILL2);
	auto kp = KiemPhap::create();

	kp->setScale(this->getTrueRadiusOfHero() * 1.75f / kp->getContentSize().width);
	auto gameLayer = (GameScene*) this->getParent();

	kp->setPosition(posX, this->getPositionY() + SCREEN_SIZE.height);
	kp->initBoxPhysic(gameLayer->world, kp->getPosition());

	gameLayer->addChild(kp, ZORDER_ENEMY);

	kp->landingEffect();

	listKiemPhap.push_back(kp);
}

void DuongQua::landKiemPhap()
{
	this->schedule([&](float dt) {
		if (checkDurationSkill2 % 8 == 0) {		// every 0.25 second
			float width = this->getPositionX() + SCREEN_SIZE.width * 0.5f;

			createKiemPhap(width);
		}

		checkDurationSkill2++;

		if ((checkDurationSkill2 >= getDurationSkill2() * 20)) {
			setIsDoneDuration2(true);
			checkDurationSkill2 = 0;
			unschedule("KeySkill2");
		}

	}, 0.05f, "KeySkill2");		//  run every 0.05 second
}

void DuongQua::doCounterSkill2()
{
	landKiemPhap();
}

// SKILL 3
void DuongQua::createSpiritHole()
{
	auto scale = this->getTrueRadiusOfHero() * 1.4f / 400;  // 400: height of spine
	spiritHole = new SkeletonAnimation("Effect/skill3.json", "Effect/skill3.atlas", scale);
	spiritHole->setPosition(this->getContentSize().width / 2 - 1.5f * this->getTrueRadiusOfHero(), 1.5f * this->getTrueRadiusOfHero());
	spiritHole->update(0.0f);
	spiritHole->setVisible(false);
	this->addChild(spiritHole);
}

void DuongQua::runSpiritHole()
{
	spiritHole->setVisible(true);
	auto scaleMe = ScaleBy::create(0.75f, 2.0f);
	spiritHole->runAction(scaleMe);
	spiritHole->clearTracks();
	spiritHole->addAnimation(0, "skill3", true);
	spiritHole->setToSetupPose();
}

Point DuongQua::getLocalSpiritBonePos(string boneName)
{
	auto bone = spiritHole->findBone(boneName);
	auto pos = Vec2(spiritHole->getScaleX()*bone->worldX, bone->worldY);
	pos = pos + spiritHole->getPosition();
	return pos;
}

void DuongQua::createTieuHonChuong(Point posHand, int Zoder)
{
	AudioManager::playSound(SOUND_DQSKILL3);
	auto thc = (TieuHonChuong*)poolSkill3->getObjectAtIndex(indexSkill3++);
	thc->setVisible(true);
	thc->setIsCollide(false);
	auto gameLayer = (GameScene*) this->getParent();

	auto worldPos = posHand + this->getPosition();
	thc->setPosition(worldPos.x + this->getTrueRadiusOfHero() / 2, worldPos.y);
	thc->initCirclePhysic(gameLayer->world, thc->getPosition());
	if (!thc->getIsAdded()) {
		this->getParent()->addChild(thc, Zoder);
		thc->setIsAdded(true);
	}

	listTieuHonChuong.push_back(thc);
	if (indexSkill3 == 10) indexSkill3 = 0;
}

void DuongQua::shootTieuHonChuong()
{
	runSpiritHole();
	this->schedule([&](float dt) {
		if (checkDurationSkill3 % 3 == 0) {
			if ((checkDurationSkill3 / 3) % 3 == 0) {
				createTieuHonChuong(getLocalSpiritBonePos("bone10"), ZORDER_HERO);
			}
			else if ((checkDurationSkill3 / 3) % 3 == 1) {
				createTieuHonChuong(getLocalSpiritBonePos("bone"), ZORDER_HERO);
			}
			else if ((checkDurationSkill3 / 3) % 3 == 2) {
				createTieuHonChuong(getLocalSpiritBonePos("bone11"), ZORDER_ENEMY);
			}
		}

		checkDurationSkill3++;

		if (checkDurationSkill3 >= getDurationSkill3() * 10) {
			auto hide = CallFunc::create([&]() {
				spiritHole->setVisible(false);
			});

			auto scale = ScaleBy::create(0.75f, 0.5f);
			spiritHole->runAction(Sequence::create(scale, hide, nullptr));
			setIsDoneDuration3(true);
			checkDurationSkill3 = 0;
			unschedule("KeySkill3");
		}

	}, 0.1f, "KeySkill3");		//  run every 0.1f second
}

void DuongQua::doCounterSkill3()
{
	shootTieuHonChuong();
}


// SLASH
void DuongQua::createSlash()
{
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;
	slash = SkeletonAnimation::createWithFile("Animation/DuongQua/slash2.json", "Animation/DuongQua/slash2.atlas", scale);
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/DuongQua/slash1.json", "Animation/DuongQua/slash1.atlas", scale);
	slashLand->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero() * 0.3f, this->getTrueRadiusOfHero() * 0.7f);
	slashLand->update(0.0f);
	slashLand->setVisible(false);

	this->addChild(slashLand);
}

void DuongQua::runSlash()
{
	slash->setVisible(true);
	slash->clearTracks();
	slash->addAnimation(0, "slash2", false);
	slash->setToSetupPose();
}

void DuongQua::runSlashLand()
{
	slashLand->setVisible(true);
	slashLand->clearTracks();
	slashLand->addAnimation(0, "slash1", false);
	slashLand->setToSetupPose();
}

void DuongQua::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = getBoxHeight() / PTM_RATIO;

	// True radius of hero is here
	setTrueRadiusOfHero(circle_shape.m_radius * PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.filter.categoryBits = BITMASK_HERO;

	fixtureDef.filter.maskBits = BITMASK_FLOOR |
		BITMASK_ENEMY | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);


	// connect sword with body
	initSwordPhysic(world, Point(pos.x + trueRadiusOfHero * 2.2f, pos.y), trueRadiusOfHero);
}

void DuongQua::addStuff()
{
	// spirit hole
	createSpiritHole();

	// slash here
	createSlash();

	BaseHero::addStuff();
}

void DuongQua::createPool()
{
	poolSkill1 = CCArray::createWithCapacity(4);
	poolSkill1->retain();

	for (int i = 0; i < 4; ++i) {
		auto tckp = ToanChanKiemPhap::create();
		tckp->setScale(this->getTrueRadiusOfHero() * 3 / tckp->getContentSize().width);
		poolSkill1->addObject(tckp);
	}

	poolSkill3 = CCArray::createWithCapacity(10);
	poolSkill3->retain();

	for (int i = 0; i < 10; ++i) {
		auto thc = TieuHonChuong::create("tieuhonchuong.png");
		thc->setScale(this->getTrueRadiusOfHero() * 1.5f / thc->getContentSize().width);
		poolSkill3->addObject(thc);
	}
}

void DuongQua::idle()
{
	clearTracks();
	addAnimation(0, "idle", true);
	setToSetupPose();

	getSmokeRun()->setVisible(false);
}

void DuongQua::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();

	if (getBloodScreen()->isVisible() && health > 1)
		getBloodScreen()->setVisible(false);

	if (!getSmokeRun()->isVisible()) {
		getSmokeRun()->setVisible(true);
	}

	//log("run");
}

void DuongQua::normalJump()
{
	BaseHero::normalJump();
	clearTracks();
	addAnimation(0, "jump", false);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("jump");
}

void DuongQua::doubleJump()
{
	BaseHero::doubleJump();
	clearTracks();
	addAnimation(0, "jumpx2", false);
	setToSetupPose();

	getSmokeJumpX2()->setPosition(this->getPosition());
	getSmokeJumpX2()->setVisible(true);
	smokeJumpX2Ani();
	//log("jumpx2");
}

void DuongQua::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("land");
}

void DuongQua::die()
{
	--dieHard;
	if (dieHard < 0) {
		return;
	}

	noActive = true;

	AudioManager::playSound(SOUND_DQDIE);

	if (spiritHole->isVisible()) {
		auto scale = this->getTrueRadiusOfHero() * 1.4f / 400;
		spiritHole->setScale(scale);
		spiritHole->setVisible(false);
	}

	if (!getIsDoneDuration1()) {
		setIsDoneDuration1(true);
		if (!listToanChanKiemPhap.empty()) {
			for (auto tckp : listToanChanKiemPhap) {
				if (!tckp->getB2Body()) continue;

				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(tckp->getB2Body());
				tckp->setB2Body(nullptr);
				tckp->setVisible(false);
			}
		}
		listToanChanKiemPhap.clear();
		checkDurationSkill1 = 0;
		unschedule("KeySkill1");
	}

	if (!getIsDoneDuration2()) {
		setIsDoneDuration2(true);
		unschedule("KeySkill2");
		checkDurationSkill2 = -1;
	}

	if (!getIsDoneDuration3()) {
		setIsDoneDuration3(true);
		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}
	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getB2Body()->SetLinearDamping(10);

	getSmokeRun()->setVisible(false);
}

void DuongQua::attackNormal()
{
	if (!getIsDoneDuration1()) {
		attackBySkill1();
		setIsPriorSkill1(true);			// move to attack
	}
	else {
		BaseHero::attackNormal();
		changeSwordCategoryBitmask(BITMASK_SWORD);

		setIsPriorAttack(true);

		runSlash();

		clearTracks();
		auto r = rand() % 2;

		if (r) {
			addAnimation(0, "attack1", false);
		}
		else {
			addAnimation(0, "attack2", false);
		}

		//log("atttack");
		setToSetupPose();

		getSlashBreak()->setVisible(false);
	}
}

void DuongQua::attackLanding()
{
	if (!getIsDoneDuration1()) {
		attackBySkill1();
		setIsPriorSkill1(true);			// move to attack
	}
	else {
		BaseHero::attackLanding();
		changeSwordCategoryBitmask(BITMASK_SWORD);
		setIsPriorAttack(true);
		runSlashLand();

		clearTracks();
		addAnimation(0, "attack3", false);
		setToSetupPose();

		//log("atttack");
		getSlashBreak()->setVisible(false);
	}
}

void DuongQua::attackBySkill1()
{
	AudioManager::playSound(SOUND_DQSKILL1);
	clearTracks();
	addAnimation(0, "attack4", false);
	setToSetupPose();

	createToanChanKiemPhap(getBoneLocation("bone52"));
}

void DuongQua::injured()
{
	AudioManager::playSound(SOUND_DQHIT);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();

	//log("injured");

}

void DuongQua::revive()
{
	clearTracks();
	addAnimation(0, "revive", false);
	setToSetupPose();

	getSmokeRun()->setVisible(false);
	getReviveMe()->setPosition(this->getPositionX() + getTrueRadiusOfHero() / 2, this->getPositionY());
	getReviveMe()->setVisible(true);
	reviveAni();
	//log("revive");
}

void DuongQua::die(Point posOfCammera)
{

}

void DuongQua::listener()
{

	this->setEndListener([&](int trackIndex) {

		if (strcmp(getCurrent()->animation->name, "injured") == 0) {

			this->getBloodScreen()->setVisible(false);

			if (getFSM()->globalState == MAttack) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MRun);
			}

			else if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();
			setIsPriorInjured(false);
		}

		else if (strcmp(getCurrent()->animation->name, "revive") == 0) {
			getReviveMe()->setVisible(false);
			getFSM()->changeState(MLand);
			auto gameLayer = (GameScene*) this->getParent();
			initCirclePhysic(gameLayer->world, this->getPosition());

			gameLayer->getHud()->resumeIfVisible();
			gameLayer->enableCalling();

			noActive = false;
		}


		else if ((strcmp(getCurrent()->animation->name, "attack1") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack2") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack3") == 0)) {

			changeSwordCategoryBitmask(BITMASK_ENEMY);

			setIsPriorAttack(false);
			if (getFSM()->globalState == MInjured) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MRun);
			}

			else if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();

		}

		// SKILL 1
		else if (strcmp(getCurrent()->animation->name, "attack4") == 0) {
			getFSM()->revertToGlobalState();
			setIsPriorSkill1(false);
		}

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			this->pause();
			auto gamelayer = (GameScene*)this->getParent();
			gamelayer->dieGame();
		}
	});


	/*this->setCompleteListener([&](int trackIndex, int loopCount) {

	});*/
}

void DuongQua::doDestroyBodies(b2World* world)
{
	BaseHero::doDestroyBodies(world);
}

void DuongQua::updateMe(float dt)
{
	BaseHero::updateMe(dt);

	getFSM()->Update();

	if (!listKiemPhap.empty()) {
		if (numberOfDeadSword == listKiemPhap.size()) {
			numberOfDeadSword = 0;
			listKiemPhap.clear();
		}

		for (auto kp : listKiemPhap) {
			if (!kp->getB2Body()) continue;

			if (this->getPositionX() - kp->getPositionX() > SCREEN_SIZE.width * 0.25f ||
				kp->getPositionY() + kp->getBoundingBox().size.height / 2 < 0) {

				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(kp->getB2Body());
				kp->setB2Body(nullptr);
				kp->removeFromParentAndCleanup(true);
				numberOfDeadSword++;
			}
			else
				kp->updateMe();

			if (kp->getIsCollide()) {
				kp->changeBodyCategoryBits(BITMASK_UNDER_GROUND);
				kp->setIsCollide(false);
			}
		}
	}

	if (!listTieuHonChuong.empty()) {
		if (numberOfDeadTHC == listTieuHonChuong.size()) {
			for (auto thc : listTieuHonChuong)	// for sure
				thc->setVisible(false);
			numberOfDeadTHC = 0;
			listTieuHonChuong.clear();
		}

		for (auto thc : listTieuHonChuong) {
			if (!thc->getB2Body()) continue;
			if (thc->getIsCollide() ||
				thc->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.25f) > SCREEN_SIZE.width / 2) {
				thc->setVisible(false);
				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(thc->getB2Body());
				thc->setB2Body(nullptr);
				numberOfDeadTHC++;
			}
			else
				thc->updateMe();

		}
	}


	if (getB2Body() == nullptr)
		return;

	auto currentVelY = getB2Body()->GetLinearVelocity().y;

	if (getFSM()->currentState == MDie) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, currentVelY));
		return;
	}

	if (this->getPositionY() < 0) {
		return;
	}

	if (!isDriverEagle/* || !isDoneDuration1*/)
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

	if (!getIsPriorAttack() && !getIsPriorInjured() && !getIsPriorSkill1()) {

		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround() && !getIsDriverEagle()) {
			getFSM()->changeState(MRun);
		}
	}

}



