#include "DuongQua.h"
#include "manager/RefManager.h"
#include "layer/GameScene.h"
#include "AudioEngine.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{

}

DuongQua * DuongQua::create(string jsonFile, string atlasFile, float scale)
{
	DuongQua* duongQua = new DuongQua(jsonFile, atlasFile, scale);
	if (duongQua && duongQua->init())
	{
		duongQua->setTag(TAG_HERO);

		duongQua->update(0.0f);

		duongQua->stateMachine = new StateMachine(duongQua);
		duongQua->stateMachine->setCurrentState(MLand);

		duongQua->setBoxHeight(duongQua->getBoundingBox().size.height / 6.4f);

		//
		duongQua->blash = Sprite::create("Animation/DuongQua/blash.png");
		duongQua->blash->setScale(scale / 2);
		duongQua->blash->setPosition(duongQua->getContentSize() / 2);
		duongQua->blash->setVisible(false);
		duongQua->addChild(duongQua->blash);

		duongQua->autorelease();
		return duongQua;
	}
	else
	{
		delete duongQua;
		duongQua = nullptr;
		return nullptr;
	}
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

		if (!isDoneDuration1 && checkDurationSkill1 >= getDurationSkill1() * 60) {
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
}

// SKILL 2
void DuongQua::createKiemPhap(float posX)
{
	AudioManager::playSound(SOUND_DQSKILL2);
	//auto kp = (KiemPhap*)poolSkill2->getObjectAtIndex(indexSkill2++);
	auto kp = KiemPhap::create();
	kp->setScale(this->getTrueRadiusOfHero() * 1.75f / kp->getContentSize().width);
	//kp->setVisible(true);
	//kp->setIsCollide(false);

	auto gameLayer = (GameScene*) this->getParent();

	kp->setPosition(posX, this->getPositionY() + SCREEN_SIZE.height);
	kp->initBoxPhysic(gameLayer->world, kp->getPosition());

	//if (!kp->getIsAdded()) {
	gameLayer->addChild(kp, ZORDER_ENEMY);
	//kp->setIsAdded(true);
//}
/*else {
	kp->setKPOpacity(255);
	kp->unscheduleAllCallbacks();
	kp->getSubImage()->setVisible(true);
	kp->getEffectLand()->setVisible(true);
}*/

	kp->landingEffect();

	listKiemPhap.push_back(kp);
	//if (indexSkill2 == 5) indexSkill2 = 0;
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
	spiritHole = SkeletonAnimation::createWithFile("Effect/skill3.json", "Effect/skill3.atlas", scale);
	//spiritHole->autorelease();
	spiritHole->setPosition(this->getContentSize().width / 2 - 1.5f * this->getTrueRadiusOfHero(), 1.5f * this->getTrueRadiusOfHero());
	spiritHole->update(0.0f);
	spiritHole->setVisible(false);
	spiritHole->clearTracks();
	spiritHole->addAnimation(0, "skill3", true);
	spiritHole->setToSetupPose();
	this->addChild(spiritHole);
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
	spiritHole->setVisible(true);
	auto scaleMe = ScaleBy::create(0.75f, 2.0f);
	spiritHole->runAction(scaleMe);

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
			auto scaleRe = ScaleBy::create(0.75f, 0.5f);

			spiritHole->runAction(Sequence::create(scaleRe, hide, nullptr));
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
	//slash->autorelease();
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/DuongQua/slash1.json", "Animation/DuongQua/slash1.atlas", scale);
	//slashLand->autorelease();
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
		tckp->setScale(this->getTrueRadiusOfHero() * 2.2f / tckp->getContentSize().width);
		poolSkill1->addObject(tckp);
	}

	/*poolSkill2 = CCArray::createWithCapacity(5);
	poolSkill2->retain();

	for (int i = 0; i < 5; ++i) {
		auto kp = KiemPhap::create();
		kp->setScale(this->getTrueRadiusOfHero() * 1.75f / kp->getContentSize().width);
		poolSkill2->addObject(kp);
	}*/

	poolSkill3 = CCArray::createWithCapacity(10);
	poolSkill3->retain();

	for (int i = 0; i < 10; ++i) {
		auto thc = TieuHonChuong::create("tieuhonchuong.png");
		thc->setScale(this->getTrueRadiusOfHero() * 1.5f / thc->getContentSize().width);
		poolSkill3->addObject(thc);
	}
}

void DuongQua::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();

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
	BaseHero::die();
	AudioManager::playSound(SOUND_DQDIE);
}

void DuongQua::attackNormal()
{
	if (!getIsDoneDuration1()) {
		AudioManager::playSound(SOUND_DQSKILL1);
		clearTracks();
		addAnimation(0, "attack4", false);
		setToSetupPose();

		createToanChanKiemPhap(getBoneLocation("bone52"));

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
		AudioManager::playSound(SOUND_DQSKILL1);
		clearTracks();
		addAnimation(0, "attack4", false);
		setToSetupPose();

		createToanChanKiemPhap(getBoneLocation("bone52"));

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

void DuongQua::injured()
{
	AudioManager::playSound(SOUND_DQHIT);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();

	//log("injured");

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

void DuongQua::stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3)
{
	if (stopSkill1 && !getIsDoneDuration1()) {
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

	if (stopSkill2 && !getIsDoneDuration2()) {
		setIsDoneDuration2(true);
		unschedule("KeySkill2");
		checkDurationSkill2 = -1;
	}

	if (stopSkill3 && !getIsDoneDuration3()) {
		spiritHole->setVisible(false);
		auto scaleRe = ScaleBy::create(0.1f, 0.5f);
		spiritHole->runAction(scaleRe);

		setIsDoneDuration3(true);
		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}
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
			//for (auto kp : listKiemPhap)	// for sure
				//kp->setVisible(false);
			numberOfDeadSword = 0;
			listKiemPhap.clear();
		}

		for (auto kp : listKiemPhap) {
			if (!kp->getB2Body()) continue;

			if (this->getPositionX() - kp->getPositionX() > SCREEN_SIZE.width * 0.25f ||
				kp->getPositionY() + kp->getBoundingBox().size.height < 0) {

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

	if (!isDriverEagle/* || !isDoneDuration1*/) {
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));
	}

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



