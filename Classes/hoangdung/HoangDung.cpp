#include "HoangDung.h"
#include "manager/RefManager.h"
#include "layer/GameScene.h"
#include "AudioEngine.h"


HoangDung::HoangDung(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
}

HoangDung * HoangDung::create(string jsonFile, string atlasFile, float scale)
{
	HoangDung* hoangDung = new HoangDung(jsonFile, atlasFile, scale);
	if (hoangDung && hoangDung->init())
	{
		hoangDung->autorelease();
		hoangDung->setTag(TAG_HERO);

		hoangDung->update(0.0f);

		hoangDung->stateMachine = new StateMachine(hoangDung);
		hoangDung->stateMachine->setCurrentState(MLand);

		hoangDung->setBoxHeight(hoangDung->getBoundingBox().size.height / 2.8f);

		//
		hoangDung->blash = Sprite::create("Animation/CoLong/blash.png");
		hoangDung->blash->setScale(scale / 2);
		hoangDung->blash->setPosition(hoangDung->getContentSize() / 2);
		hoangDung->blash->setVisible(false);
		hoangDung->addChild(hoangDung->blash);

		return hoangDung;
	}
	else
	{
		delete hoangDung;
		hoangDung = nullptr;
		return nullptr;
	}
}

void HoangDung::initSwordPhysic(b2World * world, Point position)
{
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(trueRadiusOfHero * 0.75f / PTM_RATIO, trueRadiusOfHero * 1.0f / PTM_RATIO);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY;

	bodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	swordBody = world->CreateBody(&bodyDef);
	swordBody->CreateFixture(&fixtureDef);

	spSkeleton* skeleton = this->getSkeleton();
}

void HoangDung::createThunderShield()
{
	auto scale = this->getTrueRadiusOfHero() * 1.4f / 250.0f;
	thunderShield = new SkeletonAnimation("Animation/HoangDung/Skill_1_effect.json", "Animation/HoangDung/Skill_1_effect.atlas", scale);
	thunderShield->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	thunderShield->update(0.0f);
	thunderShield->setVisible(false);
	thunderShield->clearTracks();
	thunderShield->addAnimation(0, "skill_1", true);
	thunderShield->setToSetupPose();
	this->addChild(thunderShield);
}

void HoangDung::createSkill2Effect()
{
	auto scale = this->getTrueRadiusOfHero() * 1.4f / 250.0f;
	Skill2Effect1 = new SkeletonAnimation("Animation/HoangDung/Skill_2_effect1.json", "Animation/HoangDung/Skill_2_effect1.atlas", scale);
	Skill2Effect1->autorelease();
	Skill2Effect1->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	Skill2Effect1->update(0.0f);
	Skill2Effect1->setVisible(false);
	Skill2Effect1->clearTracks();
	Skill2Effect1->addAnimation(0, "skill2", true);
	Skill2Effect1->setToSetupPose();
	this->addChild(Skill2Effect1, -1);

	Skill2Effect2 = new SkeletonAnimation("Animation/HoangDung/Skill_2_effect2.json", "Animation/HoangDung/Skill_2_effect2.atlas", scale);
	Skill2Effect2->autorelease();
	Skill2Effect2->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	Skill2Effect2->update(0.0f);
	Skill2Effect2->setVisible(false);
	Skill2Effect2->clearTracks();
	Skill2Effect2->addAnimation(0, "skill2", true);
	Skill2Effect2->setToSetupPose();
	this->addChild(Skill2Effect2, 1);

	Skill2Effect3 = new SkeletonAnimation("Animation/HoangDung/Skill_2_effect3.json", "Animation/HoangDung/Skill_2_effect3.atlas", scale);
	Skill2Effect3->autorelease();
	Skill2Effect3->setPosition(this->getContentSize().width, this->getContentSize().height);
	Skill2Effect3->update(0.0f);
	Skill2Effect3->setVisible(false);
	Skill2Effect3->clearTracks();
	Skill2Effect3->addAnimation(0, "skill2", true);
	Skill2Effect3->setToSetupPose();
	this->addChild(Skill2Effect3, 1);
}

void HoangDung::doCounterSkill1()
{
	thunderShield->setVisible(true);

	changeBodyMaskBits(BITMASK_FLOOR | BITMASK_COIN_BULLION | BITMASK_BOSS);
	m_pRadaShield->changeBodyCategoryBits(BITMASK_SWORD);

	this->schedule([&](float dt) {
		checkDurationSkill1++;

		if (checkDurationSkill1 >= getDurationSkill1() * 10) {
			if (isReviveAfterDead) {
				auto gameLayer = (GameScene*) this->getParent();
				if(isDoneDuration2 && isDoneDuration3)	// if using skill 2 or 3, do not enableCaling
					gameLayer->enableCalling();
				auto hud = gameLayer->getHud();
				hud->getBtnSkill_1()->setCanTouch(true);
				hud->getBtnSkill_1()->getMain()->setVisible(true);
				isReviveAfterDead = false;
			}

			setIsDoneDuration1(true);
			if (getOnGround()) {
				getFSM()->changeState(MRun);
				run();
			}
			checkDurationSkill1 = 0;
			thunderShield->setVisible(false);
			changeBodyMaskBits(BITMASK_FLOOR | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION | BITMASK_ENEMY);
			m_pRadaShield->changeBodyCategoryBits(BITMASK_WOODER);
			unschedule("KeySkill1");
		}
	}, 0.1f, "KeySkill1");
}


void HoangDung::createManThienHoaVu(Point posHand, int Zoder, float angle)
{
	AudioManager::playSound(SOUND_DQSKILL2);
	auto mthv = (ManThienHoaVu*)poolSkill2->getObjectAtIndex(indexSkill2++);
	mthv->setVisible(true);
	auto gameLayer = (GameScene*) this->getParent();

	mthv->setPosition(posHand.x + this->getTrueRadiusOfHero() / 2, posHand.y);
	mthv->initCirclePhysic(gameLayer->world, mthv->getPosition());
	//angle = MATH_DEG_TO_RAD(angle);
	mthv->setVel(b2Vec2(mthv->getVel().Length() * cosf(MATH_DEG_TO_RAD(angle)), mthv->getVel().Length() * sinf(MATH_DEG_TO_RAD(angle))));
	//mthv->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), angle);
	mthv->setRotation(-angle);
	if (!mthv->getIsAdded()) {
		this->getParent()->addChild(mthv, Zoder);
		mthv->setIsAdded(true);
	}

	listManThienHoaVu.push_back(mthv);
	if (indexSkill2 == 7) indexSkill2 = 0;
}

void HoangDung::shootManThienHoaVu()
{
	for (int i = 0; i < 7; i++) {
		Point _ptTempPoint = getBoneLocation("bone6");
		_ptTempPoint.x += this->getTrueRadiusOfHero() * 1.0f;
		_ptTempPoint.y -= this->getTrueRadiusOfHero() * 1.0f;
		_ptTempPoint.y += this->getTrueRadiusOfHero() * 0.3f * i;
		createManThienHoaVu(_ptTempPoint, ZORDER_HERO, -45.0f + (i * 15.0f));
	}

	this->schedule([&](float dt) {
		for (int i = 0; i < 7; i++) {
			Point _ptTempPoint = getBoneLocation("bone6");
			_ptTempPoint.x += this->getTrueRadiusOfHero() * 1.0f;
			_ptTempPoint.y -= this->getTrueRadiusOfHero() * 1.0f;
			_ptTempPoint.y += this->getTrueRadiusOfHero() * 0.3f * i;
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, -45.0f + (i * 15.0f));
		}
		/*if (checkDurationSkill2 % 5 == 0) {
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, 30.0f);
		}
		else if (checkDurationSkill2 % 5 == 1) {
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, 10.0f);
		}
		else if (checkDurationSkill2 % 5 == 2) {
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, -10.0f);
		}
		else if (checkDurationSkill2 % 5 == 3) {
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, 0.0f);
		}
		else if (checkDurationSkill2 % 5 == 4) {
			createManThienHoaVu(_ptTempPoint, ZORDER_HERO, 20.0f);
		}*/

		checkDurationSkill2++;

		if (checkDurationSkill2 >= getDurationSkill2()) {
			Skill2Effect1->setVisible(false);
			Skill2Effect2->setVisible(false);
			Skill2Effect3->setVisible(false);
			setIsDoneDuration2(true);
			checkDurationSkill2 = 0;
			unschedule("KeySkill2");
		}

	}, 1.0f, "KeySkill2");		//  run every 0.1f second
}

void HoangDung::doCounterSkill2() {
	Skill2Effect1->setVisible(true);
	Skill2Effect2->setVisible(true);
	Skill2Effect3->setVisible(true);
	shootManThienHoaVu();
}

// SKILL 3
void HoangDung::createDaCauBongPhap(Point posSword)
{
	auto dcbp = (DaCauBongPhap*)poolSkill3->getObjectAtIndex(indexSkill3++);
	dcbp->setVisible(true);
	auto gameLayer = (GameScene*) this->getParent();

	dcbp->setPosition(posSword.x + this->getTrueRadiusOfHero() / 2, posSword.y);
	dcbp->initCirclePhysic(gameLayer->world, dcbp->getPosition());

	if (!dcbp->getIsAdded()) {
		this->getParent()->addChild(dcbp, ZORDER_SMT);
		dcbp->setIsAdded(true);
	}

	listDaCauBongPhap.push_back(dcbp);

	if (indexSkill3 == 4) indexSkill3 = 0;
}

void HoangDung::slashDaCauBongPhap()
{
	this->schedule([&](float dt) {
		checkDurationSkill3++;

		if (!isDoneDuration3 && checkDurationSkill3 >= getDurationSkill3() * 60) {
			setIsDoneDuration3(true);
		}

		// get more time to update
		if (checkDurationSkill3 > getDurationSkill3() * 85) {
			checkDurationSkill3 = 0;
			unschedule("KeySkill3");
		}

	}, 1.0f / 60, "KeySkill3");		//  run every delta second
}

void HoangDung::doCounterSkill3()
{
	slashDaCauBongPhap();
}

// SLASH
void HoangDung::createSlash()
{
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;
	slash = SkeletonAnimation::createWithFile("Animation/HoangDung/slash2.json", "Animation/HoangDung/slash2.atlas", scale);
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/HoangDung/slash1.json", "Animation/HoangDung/slash1.atlas", scale);
	slashLand->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero() * 0.3f, this->getTrueRadiusOfHero() * 0.7f);
	slashLand->update(0.0f);
	slashLand->setVisible(false);

	this->addChild(slashLand);
}

void HoangDung::runSlash()
{
	slash->setVisible(true);
	slash->clearTracks();
	slash->addAnimation(0, "slash2", false);
	slash->setToSetupPose();
}

void HoangDung::runSlashLand()
{
	slashLand->setVisible(true);
	slashLand->clearTracks();
	slashLand->addAnimation(0, "slash1", false);
	slashLand->setToSetupPose();
}

void HoangDung::initCirclePhysic(b2World * world, Point pos) {
	BaseHero::initCirclePhysic(world, pos);

	m_pRadaShield = Rada::create("Animation/CoLong/blash.png");
	m_pRadaShield->setScale(this->getTrueRadiusOfHero() / m_pRadaShield->getContentSize().width * 2.5f);
	m_pRadaShield->setVisible(false);
	m_pRadaShield->initCirclePhysic(world, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaShield->changeBodyCategoryBits(BITMASK_WOODER);
	m_pRadaShield->changeBodyMaskBits(BITMASK_ENEMY | BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG);
}

void HoangDung::addStuff()
{
	// slash here
	//createSlash();

	BaseHero::addStuff();

	createThunderShield();
	createSkill2Effect();
}

void HoangDung::createPool()
{
	/*poolSkill2 = CCArray::createWithCapacity(11);
	poolSkill2->retain();

	for (int i = 0; i < 11; ++i) {
		auto scale = 0.1f;
		auto mthv = DaCauBongPhap::create("Animation/HoangDung/Skill_3_effect.json", "Animation/HoangDung/Skill_3_effect.atlas", scale);
		poolSkill2->addObject(mthv);
	}*/

	poolSkill2 = CCArray::createWithCapacity(7);
	poolSkill2->retain();

	for (int i = 0; i < 7; ++i) {
		auto mthv = ManThienHoaVu::create("mthv.png");
		mthv->setScale(this->getTrueRadiusOfHero() * 5.0f / mthv->getContentSize().width);
		poolSkill2->addObject(mthv);
	}

	poolSkill3 = CCArray::createWithCapacity(4);
	poolSkill3->retain();

	for (int i = 0; i < 4; ++i) {
		auto scale = 0.25f;
		auto dcbp = DaCauBongPhap::create("Animation/HoangDung/Skill_3_effect.json", "Animation/HoangDung/Skill_3_effect.atlas", scale);
		poolSkill3->addObject(dcbp);
	}
}

void HoangDung::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();

	if (!getSmokeRun()->isVisible()) {
		getSmokeRun()->setVisible(true);
	}

	if (!isUseSpecial) {
		setIsDoneDuration1(false);
		doCounterSkill1();
		isUseSpecial = true;
	}
		
	//log("run");
}

void HoangDung::normalJump()
{
	// weird
	/*if (!getIsDoneDuration2()) {
	}
	else {*/
		BaseHero::normalJump();
		clearTracks();
		addAnimation(0, "jump", false);
		setToSetupPose();

		getSmokeRun()->setVisible(false);
	//}

	//log("jump");
}

void HoangDung::doubleJump()
{
	/*if (!getIsDoneDuration2()) {
	}
	else {*/
		BaseHero::doubleJump();
		clearTracks();
		addAnimation(0, "jumpx2", false);
		setToSetupPose();

		getSmokeJumpX2()->setPosition(this->getPosition());
		getSmokeJumpX2()->setVisible(true);
		smokeJumpX2Ani();
	//}
	//log("jumpx2");
}

void HoangDung::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("land");
}

void HoangDung::die()
{
	BaseHero::die();
	AudioManager::playSound(SOUND_DQDIE);
}

void HoangDung::attackNormal()
{
	if (!getIsDoneDuration3()) {
		clearTracks();
		addAnimation(0, "skill3", false);
		setToSetupPose();

		createDaCauBongPhap(getBoneLocation("bone6"));

		setIsPriorSkill3(true);			// move to attack
	}
	else {
		BaseHero::attackNormal();
		changeSwordCategoryBitmask(BITMASK_SWORD);

		setIsPriorAttack(true);

		//runSlash();

		clearTracks();
		auto r = rand() % 2;

		if (r) {
			addAnimation(0, "attack2", false);
		}
		else {
			addAnimation(0, "attack3", false);
		}

		//log("atttack*");
		setToSetupPose();

		getSlashBreak()->setVisible(false);
	}
}

void HoangDung::attackLanding()
{
	if (!getIsDoneDuration3()) {
		clearTracks();
		addAnimation(0, "skill3", false);
		setToSetupPose();

		createDaCauBongPhap(getBoneLocation("bone6"));

		setIsPriorSkill3(true);			// move to attack
	}
	else {
		BaseHero::attackLanding();
		changeSwordCategoryBitmask(BITMASK_SWORD);
		setIsPriorAttack(true);
		//runSlashLand();

		clearTracks();
		addAnimation(0, "attack1", false);
		setToSetupPose();

		//log("atttack");
		getSlashBreak()->setVisible(false);
	}
}

void HoangDung::injured()
{
	AudioManager::playSound(SOUND_DQHIT);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();

	//log("injured");

}

void HoangDung::listener()
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
			isReviveAfterDead = true;
			isUseSpecial = false;
			getReviveMe()->setVisible(false);
			getFSM()->changeState(MLand);
			auto gameLayer = (GameScene*) this->getParent();
			initCirclePhysic(gameLayer->world, this->getPosition());

			auto hud = gameLayer->getHud();
			hud->resumeIfVisible();
			hud->getBtnSkill_1()->setCanTouch(false);
			hud->getBtnSkill_1()->getMain()->setVisible(false);

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

		// SKILL 3
		else if (strcmp(getCurrent()->animation->name, "skill3") == 0) {
			getFSM()->revertToGlobalState();
			setIsPriorSkill3(false);
		}

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			this->pause();
			auto gamelayer = (GameScene*)this->getParent();
			gamelayer->dieGame();
		}
	});
}

void HoangDung::stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3)
{
	if (stopSkill1 && !getIsDoneDuration1()) {
		thunderShield->setVisible(false);
		changeBodyMaskBits(BITMASK_FLOOR | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION | BITMASK_ENEMY); // original
		m_pRadaShield->changeBodyCategoryBits(BITMASK_WOODER);

		setIsDoneDuration1(true);
		unschedule("KeySkill1");
		checkDurationSkill1 = 0;
	}

	if (stopSkill2 && !getIsDoneDuration2()) {
		Skill2Effect1->setVisible(false);
		Skill2Effect2->setVisible(false);
		Skill2Effect3->setVisible(false);

		setIsDoneDuration2(true);

		if (!listManThienHoaVu.empty()) {
			for (auto mthv : listManThienHoaVu) {
				if (!mthv->getB2Body()) continue;

				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(mthv->getB2Body());
				mthv->setB2Body(nullptr);
				mthv->setVisible(false);
			}
		}
		listManThienHoaVu.clear();

		Skill2Effect1->setVisible(false);
		Skill2Effect2->setVisible(false);
		Skill2Effect3->setVisible(false);

		unschedule("KeySkill2");
		checkDurationSkill2 = 0;
	}

	if (stopSkill3 && !getIsDoneDuration3()) {
		setIsDoneDuration3(true);

		if (!listDaCauBongPhap.empty()) {
			for (auto dcbp : listDaCauBongPhap) {
				if (!dcbp->getB2Body()) continue;

				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(dcbp->getB2Body());
				dcbp->setB2Body(nullptr);
				dcbp->setVisible(false);
			}
		}

		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}
}

void HoangDung::doDestroyBodies(b2World* world)
{
	BaseHero::doDestroyBodies(world);

	world->DestroyBody(m_pRadaShield->getB2Body());
	m_pRadaShield->setB2Body(nullptr);
}

void HoangDung::updateMe(float dt)
{
	BaseHero::updateMe(dt);

	getFSM()->Update();

	//log("Pos X: %f, pos Y: %f", getBoneLocation("bone9").x, getBoneLocation("bone9").y);

	if (getB2Body() == nullptr)
		return;

	if (m_pRadaShield != nullptr) {
		m_pRadaShield->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
	}

	if (!listManThienHoaVu.empty()) {
		if (numberOfDeadMTHV == listManThienHoaVu.size()) {
			for (auto thc : listManThienHoaVu)	// for sure
				thc->setVisible(false);
			numberOfDeadMTHV = 0;
			listManThienHoaVu.clear();
		}

		for (auto mthv : listManThienHoaVu) {
			if (!mthv->getB2Body()) continue;
			if (mthv->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.25f) > SCREEN_SIZE.width / 2) {
				mthv->setVisible(false);
				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(mthv->getB2Body());
				mthv->setB2Body(nullptr);
				numberOfDeadMTHV++;
			}
			else
				mthv->updateMe();

		}
	}

	if (!listDaCauBongPhap.empty()) {
		if (numberOfDeadDCBP == listDaCauBongPhap.size()) {
			for (auto dcbp : listDaCauBongPhap)	// for sure
				dcbp->setVisible(false);
			numberOfDeadDCBP = 0;
			listDaCauBongPhap.clear();
		}

		for (auto dcbp : listDaCauBongPhap) {
			if (!dcbp->getB2Body()) continue;
			if (dcbp->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.255f) > SCREEN_SIZE.width / 2) {
				dcbp->setVel(b2Vec2(-SCREEN_SIZE.width * 0.5f / PTM_RATIO, 0));
			}
			if (dcbp->getPositionX() < this->getPositionX()) {
				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(dcbp->getB2Body());
				dcbp->setB2Body(nullptr);
				dcbp->setVisible(false);
				numberOfDeadDCBP++;
			}
			else
				dcbp->updateMe();
		}
	}

	auto currentVelY = getB2Body()->GetLinearVelocity().y;

	if (getFSM()->currentState == MDie) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, currentVelY));
		return;
	}

	if (this->getPositionY() < 0) {
		return;
	}

	if (!isDriverEagle/* || !isDoneDuration1*/) {
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));
	}

	if (!getIsPriorAttack() && !getIsPriorInjured() && !getIsPriorSkill3()) {

		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround() && !getIsDriverEagle()) {
			getFSM()->changeState(MRun);
		}
	}

}