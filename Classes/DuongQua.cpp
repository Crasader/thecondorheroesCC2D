#include "DuongQua.h"
#include "AudioEngine.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
	checkDurationSkill1 = 0;
	checkDurationSkill2 = 0;
	checkDurationSkill3 = 1;
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

	duongQua->health = 100;

	duongQua->facingRight = true;

	duongQua->numberOfJump = 2;

	duongQua->setOnGround(false);
	duongQua->setIsPrior(false);		// future, we need to add props into base class
	duongQua->setIsPriorSkill1(false);
	duongQua->setIsPriorSkill2(false);
	duongQua->setIsPriorSkill3(false);

	duongQua->setIsDoneDuration1(true);
	duongQua->setIsDoneDuration2(true);
	duongQua->setIsDoneDuration3(true);

	//
	return duongQua;
}


// SKILL 1
void DuongQua::createToanChanKiemPhap(Point posSword)
{
	auto tckp = ToanChanKiemPhap::create("Animation/DuongQua/skill1.png");
	tckp->setScale(this->getTrueRadiusOfHero() * 3 / tckp->getContentSize().width);
	auto world = this->getB2Body()->GetWorld();

	tckp->setPosition(posSword.x + this->getTrueRadiusOfHero() / 2, posSword.y);
	tckp->initCirclePhysic(world, tckp->getPosition());
	tckp->changeBodyCategoryBits(BITMASK_SWORD);
	tckp->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS);

	this->getParent()->addChild(tckp, ZORDER_SMT);

	tckp->setAngel(0);
	listToanChanKiemPhap.push_back(tckp);
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
				if (tckp->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.26f) > SCREEN_SIZE.width / 2) {
					this->getB2Body()->GetWorld()->DestroyBody(tckp->getB2Body());
					tckp->setB2Body(nullptr);

					tckp->removeFromParentAndCleanup(true);
				}
				else
					tckp->updateMe(dt);
			}
		}

		// get more time to update
		if (checkDurationSkill1 > getDurationSkill1() * 100) {
			listToanChanKiemPhap.clear();
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}

	}, 1.0f / 60, "KeySkill1");		//  run every delta second
}

void DuongQua::doCounterSkill1()
{
	slashToanChanKiemPhap();
}


// SKILL 2
void DuongQua::createKiemPhap(float posX)
{
	auto kp = KiemPhap::create("Animation/DuongQua/Sword.png");
	kp->setScale(this->getTrueRadiusOfHero() * 1.5f / kp->getContentSize().width);
	auto world = this->getB2Body()->GetWorld();

	kp->setPosition(posX, SCREEN_SIZE.height + kp->getBoundingBox().size.height);
	kp->initBoxPhysic(world, kp->getPosition());

	this->getParent()->addChild(kp, ZORDER_ENEMY);


	listKiemPhap.push_back(kp);
}

void DuongQua::landKiemPhap()
{
	this->schedule([&](float dt) {
		if (checkDurationSkill2 % 5 == 0) {		// every 0.2 second
			createKiemPhap(this->getPositionX() + SCREEN_SIZE.width * (0.05f * (checkDurationSkill2 / 5 + 6)));
		}

		checkDurationSkill2++;

		if ((checkDurationSkill2 >= getDurationSkill2() * 20)) {
			numberOfDeadSword = 0;
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
	auto scale = this->getTrueRadiusOfHero() * 1.25f / 400;  // 400: height of spine
	spiritHole = new SkeletonAnimation("Effect/skill3.json", "Effect/skill3.atlas", scale);
	spiritHole->update(0.0f);
	spiritHole->setVisible(false);
	this->getParent()->addChild(spiritHole, ZORDER_SMT);
}

void DuongQua::runSpiritHole()
{
	spiritHole->setVisible(true);
	auto scaleMe = ScaleBy::create(0.5f, 2.0f);
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
	auto thc = TieuHonChuong::create("Animation/DuongQua/tieuhonchuong.png");
	thc->setScale(this->getTrueRadiusOfHero() * 1.5f / thc->getContentSize().width);
	auto world = this->getB2Body()->GetWorld();

	thc->setPosition(posHand.x + this->getTrueRadiusOfHero() / 2, posHand.y);
	thc->initCirclePhysic(world, thc->getPosition());
	thc->changeBodyCategoryBits(BITMASK_SWORD);
	thc->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS);

	this->getParent()->addChild(thc, Zoder);

	//thc->getB2Body()->SetTransform(thc->getB2Body()->GetPosition(), angle);
	thc->setAngel(0);

	listTieuHonChuong.push_back(thc);
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

			auto scale = ScaleBy::create(0.5f, 0.5f);
			spiritHole->runAction(Sequence::create(scale, hide, nullptr));
			setIsDoneDuration3(true);
			numberOfDeadTHC = 0;
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
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;  // 400: hieght of spine
	slash = SkeletonAnimation::createWithFile("Animation/DuongQua/slash2.json", "Animation/DuongQua/slash2.atlas", scale);
	slash->update(0.0f);
	slash->setVisible(false);
	this->getParent()->addChild(slash, ZORDER_SMT);

	auto scaleLand = this->getTrueRadiusOfHero() * 1.8f / 400;  // 400: hieght of spine
	slashLand = SkeletonAnimation::createWithFile("Animation/DuongQua/slash1.json", "Animation/DuongQua/slash1.atlas", scaleLand);
	slashLand->update(0.0f);
	slashLand->setVisible(false);
	this->getParent()->addChild(slashLand, ZORDER_SMT);
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
	circle_shape.m_radius = this->getBoundingBox().size.height / 6.7f / PTM_RATIO;

	// True radius of hero is here
	setTrueRadiusOfHero(circle_shape.m_radius * PTM_RATIO);
	//

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.filter.categoryBits = BITMASK_HERO;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR | BITMASK_WOODER | BITMASK_COIN |
		BITMASK_TOANCHAN1 | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);


	// connect sword with body
	initSwordPhysic(world, Point(pos.x + trueRadiusOfHero * 2.2f, pos.y), trueRadiusOfHero);

	// spirit hole
	createSpiritHole();

	// slash here
	createSlash();
}

void DuongQua::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();

	if (getBloodScreen()->isVisible())
		getBloodScreen()->setVisible(false);

	if (!EM->getSmokeRun()->isVisible()) {
		EM->getSmokeRun()->setVisible(true);
		EM->smokeRunAni();
	}
}

void DuongQua::normalJump()
{
	clearTracks();
	addAnimation(0, "jump", false);
	setToSetupPose();

	EM->getSmokeRun()->setVisible(false);
}

void DuongQua::doubleJump()
{
	clearTracks();
	addAnimation(0, "jumpx2", false);
	setToSetupPose();

	EM->getSmokeJumpX2()->setPosition(this->getPosition());
	EM->getSmokeJumpX2()->setVisible(true);
	EM->smokeJumpX2Ani();
}

void DuongQua::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();

	EM->getSmokeRun()->setVisible(false);
}

void DuongQua::die()
{
	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getB2Body()->SetLinearDamping(10);

	EM->getSmokeRun()->setVisible(false);
}

void DuongQua::attackNormal()
{
	runSlash();

	clearTracks();
	auto r = rand() % 2;

	if (r) {
		addAnimation(0, "attack1", false);
	}
	else {
		addAnimation(0, "attack2", false);
	}


	setToSetupPose();

	EM->getSlashBreak()->setVisible(false);
}

void DuongQua::attackLanding()
{
	runSlashLand();

	clearTracks();
	addAnimation(0, "attack3", false);
	setToSetupPose();

	EM->getSlashBreak()->setVisible(false);
}

void DuongQua::attackBySkill1()
{
	clearTracks();
	addAnimation(0, "attack4", false);
	setToSetupPose();

	createToanChanKiemPhap(getBoneLocation("bone52"));
}

void DuongQua::attackBySkill2()
{
	/*clearTracks();
	addAnimation(0, "skill2", false);
	setToSetupPose();*/
}

void DuongQua::attackBySkill3()
{
	/*clearTracks();
	addAnimation(0, "skill3", false);
	setToSetupPose();*/
}

void DuongQua::injured()
{
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();
}

void DuongQua::die(Point posOfCammera)
{

}

void DuongQua::listener()
{
	// set Duration here
	this->setDurationSkill1(4.0f);
	this->setDurationSkill2(2.5f);
	//this->setDurationSkill3(spSkeletonData_findAnimation(this->getSkeleton()->data, "skill3")->duration);
	this->setDurationSkill3(2.5f);


	this->setEndListener([&](int trackIndex) {
		if ((strcmp(getCurrent()->animation->name, "jumpx2") == 0)) {
			getFSM()->changeState(MLand);
		}

		else if ((strcmp(getCurrent()->animation->name, "injured") == 0)) {

			this->getBloodScreen()->setVisible(false);
			getFSM()->revertToGlobalState();

			setIsPrior(false);
		}

		else if ((strcmp(getCurrent()->animation->name, "attack1") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack2") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack3") == 0)) {

			changeSwordCategoryBitmask(BITMASK_ENEMY);

			getFSM()->revertToGlobalState();

			setIsPrior(false);

		}

		else if (strcmp(getCurrent()->animation->name, "attack4") == 0) {
			getFSM()->revertToGlobalState();
			setIsPriorSkill1(false);
		}

	});


	/*this->setCompleteListener([&](int trackIndex, int loopCount) {

	});*/
}

void DuongQua::updateMe(float dt)
{
	BaseHero::updateMe(dt);
	getFSM()->Update();


	auto currentVelY = getB2Body()->GetLinearVelocity().y;

	if (!listKiemPhap.empty()) {
		if (numberOfDeadSword == listKiemPhap.size()) {
			listKiemPhap.clear();
		}

		for (auto kp : listKiemPhap) {
			if (!kp->getB2Body()) continue;

			if (this->getPositionX() - kp->getPositionX() > SCREEN_SIZE.width * 0.255f ||
				kp->getPositionY() + kp->getBoundingBox().size.height / 2 < 0) {

				if (kp->getB2Body()) {
					this->getB2Body()->GetWorld()->DestroyBody(kp->getB2Body());
					kp->setB2Body(nullptr);
				}

				kp->removeFromParentAndCleanup(true);
				numberOfDeadSword++;
			}
			else
				kp->updateMe(dt);

			if (kp->getIsCollide()) {
				this->getB2Body()->GetWorld()->DestroyBody(kp->getB2Body());
				kp->setB2Body(nullptr);
			}
		}
	}

	if (!listTieuHonChuong.empty()) {
		if (numberOfDeadTHC == listTieuHonChuong.size()) {
			listTieuHonChuong.clear();
		}

		for (auto thc : listTieuHonChuong) {
			if (!thc->getB2Body()) continue;
			if (thc->getIsCollide() ||
				thc->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.255f) > SCREEN_SIZE.width / 2) {
				this->getB2Body()->GetWorld()->DestroyBody(thc->getB2Body());
				thc->setB2Body(nullptr);

				thc->removeFromParentAndCleanup(true);
				numberOfDeadTHC++;
			}
			else
				thc->updateMe(dt);
		}
	}

	if (getFSM()->currentState == MDie) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, currentVelY));
		return;
	}

	if (getPositionY() + getTrueRadiusOfHero() * 2 < 0) {
		getB2Body()->SetTransform(b2Vec2(SCREEN_SIZE.width * 0.25f / PTM_RATIO,
			SCREEN_SIZE.height / PTM_RATIO), getB2Body()->GetAngle());
		return;
	}

	if (EM->getSmokeRun()->isVisible())
		EM->getSmokeRun()->setPosition(this->getPosition());

	getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

	if (getSlash()->isVisible())
		getSlash()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero(),
			this->getPositionY() + this->getTrueRadiusOfHero() * 0.7f);

	if (getSlashLand()->isVisible())
		getSlashLand()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero() * 0.3f,
			this->getPositionY() + this->getTrueRadiusOfHero() * 0.7f);

	if (spiritHole->isVisible()) {
		spiritHole->setPosition(this->getPositionX() - 2 * this->getTrueRadiusOfHero(),
			this->getPositionY() + 1.5f * this->getTrueRadiusOfHero());
	}

	if (!getIsPrior() && !getIsPriorSkill1() /*&& !getIsPriorSkill2() && !getIsPriorSkill3()*/) {

		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround()) {
			getFSM()->changeState(MRun);
		}
	}

}



