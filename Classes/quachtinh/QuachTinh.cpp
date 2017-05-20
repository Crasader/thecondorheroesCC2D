#include "QuachTinh.h"
#include "manager/RefManager.h"
#include "layer/GameScene.h"
#include "AudioEngine.h"


QuachTinh::QuachTinh(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
}

QuachTinh * QuachTinh::create(string jsonFile, string atlasFile, float scale)
{
	QuachTinh* quachTinh = new QuachTinh(jsonFile, atlasFile, scale);
	quachTinh->setTag(TAG_HERO);

	quachTinh->update(0.0f);

	quachTinh->stateMachine = new StateMachine(quachTinh);
	quachTinh->stateMachine->setCurrentState(MLand);

	quachTinh->setBoxHeight(quachTinh->getBoundingBox().size.height / 8.8f);

	//
	quachTinh->blash = Sprite::create("Animation/QuachTinh/blash.png");
	quachTinh->blash->setScale(scale / 2);
	quachTinh->blash->setPosition(quachTinh->getContentSize() / 2);
	quachTinh->blash->setVisible(false);
	quachTinh->addChild(quachTinh->blash);

	return quachTinh;
}


void QuachTinh::createRock(float posX)
{
	auto rock = (ChanKinh*)poolSkill1->getObjectAtIndex(indexSkill1++);
	rock->setVisible(true);

	auto gameLayer = (GameScene*) this->getParent();

	rock->setPosition(posX, this->getPositionY() + SCREEN_SIZE.height);
	rock->initCirclePhysic(gameLayer->world, rock->getPosition());
	rock->setAngle(rock->getmAngle() / 180 * PI);

	if (!rock->getIsAdded()) {
		gameLayer->addChild(rock, ZORDER_ENEMY);
		rock->setIsAdded(true);
	}


	listRock.push_back(rock);
	if (indexSkill1 == 5) indexSkill1 = 0;
}

void QuachTinh::landRocks()
{
	this->schedule([&](float dt) {
		if (checkDurationSkill1 % 8 == 0) {		// every 0.25 second
			float width = this->getPositionX() + SCREEN_SIZE.width * random(0.0f, 0.3f);

			createRock(width);
		}

		checkDurationSkill1++;

		if ((checkDurationSkill1 >= getDurationSkill1() * 20)) {
			setIsDoneDuration1(true);
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}

	}, 0.05f, "KeySkill1");		//  run every 0.05 second
}

void QuachTinh::doCounterSkill1()
{
	//getFSM()->changeState(MSKill1);
	landRocks();
}

void QuachTinh::doCounterSkill2()
{
	this->schedule([&](float dt) {
		checkDurationSkill2++;
		if (checkDurationSkill2 >= getDurationSkill2() * 10) {
			setIsDoneDuration2(true);
			checkDurationSkill2 = 0;
			unschedule("KeySkill2");
		}
	}, 0.1f, "KeySkill2");
}

void QuachTinh::doCounterSkill3()
{
	this->schedule([&](float dt) {
		checkDurationSkill3++;
		if (checkDurationSkill3 >= getDurationSkill3() * 10) {
			setIsDoneDuration3(true);
			checkDurationSkill3 = 0;
			unschedule("KeySkill3");
		}
	}, 0.1f, "KeySkill3");
}


// SLASH
void QuachTinh::createSlash()
{
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;
	slash = SkeletonAnimation::createWithFile("Animation/QuachTinh/slash2.json", "Animation/QuachTinh/slash2.atlas", scale);
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/QuachTinh/slash1.json", "Animation/QuachTinh/slash1.atlas", scale);
	slashLand->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero() * 0.3f, this->getTrueRadiusOfHero() * 0.7f);
	slashLand->update(0.0f);
	slashLand->setVisible(false);

	this->addChild(slashLand);
}

void QuachTinh::runSlash()
{
	slash->setVisible(true);
	slash->clearTracks();
	slash->addAnimation(0, "slash2", false);
	slash->setToSetupPose();
}

void QuachTinh::runSlashLand()
{
	slashLand->setVisible(true);
	slashLand->clearTracks();
	slashLand->addAnimation(0, "slash1", false);
	slashLand->setToSetupPose();
}

void QuachTinh::addStuff()
{
	// slash here
	//createSlash();

	BaseHero::addStuff();
}

void QuachTinh::createPool()
{
	poolSkill1 = CCArray::createWithCapacity(5);
	poolSkill1->retain();

	for (int i = 0; i < 5; ++i) {
		auto rock = ChanKinh::create();
		rock->setScale(this->getTrueRadiusOfHero() * 1.5f / rock->getContentSize().width);
		poolSkill1->addObject(rock);
	}
}

void QuachTinh::run()
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

void QuachTinh::normalJump()
{
	BaseHero::normalJump();
	clearTracks();
	addAnimation(0, "jump", false);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("jump");
}

void QuachTinh::doubleJump()
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

void QuachTinh::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();
	setIsPriorSkill1(true);

	getSmokeRun()->setVisible(false);

	//log("land");
}

void QuachTinh::die()
{
	BaseHero::die();
	AudioManager::playSound(SOUND_DQDIE);
}

void QuachTinh::attackNormal()
{
	BaseHero::attackNormal();
	changeSwordCategoryBitmask(BITMASK_SWORD);

	setIsPriorAttack(true);

	//runSlash();

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

void QuachTinh::attackLanding()
{
	BaseHero::attackLanding();
	changeSwordCategoryBitmask(BITMASK_SWORD);
	setIsPriorAttack(true);
	//runSlashLand();

	clearTracks();
	addAnimation(0, "attack3", false);
	setToSetupPose();

	//log("atttack");
	getSlashBreak()->setVisible(false);
}

void QuachTinh::attackBySkill1()
{
	//AudioManager::playSound(SOUND_DQSKILL1);
	clearTracks();
	addAnimation(0, "skill1", false);
	setToSetupPose();
	setIsPriorSkill1(true);
}

void QuachTinh::injured()
{
	AudioManager::playSound(SOUND_DQHIT);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();

	//log("injured");

}

void QuachTinh::listener()
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

		/*else if (strcmp(getCurrent()->animation->name, "skill1") == 0) {
			setIsPriorSkill1(false);
			if (getFSM()->globalState == MAttack) {
				getFSM()->setPreviousState(MSKill1);
				getFSM()->setGlobalState(MRun);
			}

			else if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MSKill1);
				getFSM()->setGlobalState(MLand);
			}
			

			getFSM()->revertToGlobalState();
		}*/

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			this->pause();
			auto gamelayer = (GameScene*)this->getParent();
			gamelayer->dieGame();
		}
	});


	/*this->setCompleteListener([&](int trackIndex, int loopCount) {

	});*/
}

void QuachTinh::stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3)
{
	if (stopSkill1 && !getIsDoneDuration1()) {
		setIsDoneDuration1(true);
	}

	if (stopSkill2 && !getIsDoneDuration2()) {
		setIsDoneDuration2(true);
	}

	if (stopSkill3 && !getIsDoneDuration3()) {
		setIsDoneDuration3(true);
	}
}

void QuachTinh::doDestroyBodies(b2World* world)
{
	BaseHero::doDestroyBodies(world);
}

void QuachTinh::updateMe(float dt)
{
	BaseHero::updateMe(dt);

	getFSM()->Update();

	if (!listRock.empty()) {
		if (numberOfDeadRock == listRock.size()) {
			for (auto rock : listRock)	// for sure
				rock->setVisible(false);
			numberOfDeadRock = 0;
			listRock.clear();
		}

		for (auto rock : listRock) {
			if (!rock->getB2Body()) continue;

			if (rock->getPositionY() + rock->getBoundingBox().size.height < 0) {
				rock->setVisible(false);
				auto gameLayer = (GameScene*) this->getParent();

				gameLayer->world->DestroyBody(rock->getB2Body());
				rock->setB2Body(nullptr);
				
				numberOfDeadRock++;
			}
			else
				rock->updateMe();
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

	if (!getIsPriorAttack() && !getIsPriorInjured()/* && !getIsPriorSkill1() */) {

		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround() && !getIsDriverEagle()) {
			getFSM()->changeState(MRun);
		}
	}

}
