#include "HoangDuocSu.h"
#include "layer/GameScene.h"
#include "manager/RefManager.h"
#include "AudioEngine.h"


HoangDuocSu::HoangDuocSu(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
}

HoangDuocSu * HoangDuocSu::create(string jsonFile, string atlasFile, float scale)
{
	HoangDuocSu* hds = new HoangDuocSu(jsonFile, atlasFile, scale);
	if (hds && hds->init())
	{
		hds->autorelease();
		hds->setTag(TAG_HERO);

		hds->update(0.0f);

		hds->stateMachine = new StateMachine(hds);
		hds->stateMachine->setCurrentState(MLand);

		hds->setBoxHeight(hds->getBoundingBox().size.height / 5.0f);

		//
		hds->blash = Sprite::create("Animation/hds/blash.png");
		hds->blash->setScale(scale / 2);
		hds->blash->setPosition(hds->getContentSize() / 2);
		hds->blash->setVisible(false);
		hds->addChild(hds->blash);

		return hds;
	}
	else
	{
		delete hds;
		hds = nullptr;
		return nullptr;
	}
}

void HoangDuocSu::initCirclePhysic(b2World * world, Point pos)
{
	BaseHero::initCirclePhysic(world, pos);
}

void HoangDuocSu::initSwordPhysic(b2World * world, Point position)
{
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(trueRadiusOfHero * 0.75f / PTM_RATIO, trueRadiusOfHero * 1.1f / PTM_RATIO);

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

void HoangDuocSu::fastAndFurious()
{
	// create a rada to change bitmask instead
	this->isNoDie = true;
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_SWORD);
	this->changeBodyMaskBits(BITMASK_WOODER | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY);

	this->schedule([&](float dt) {
		checkDurationSkill1++;

		this->getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel() * 5, 0));

		if (checkDurationSkill1 >= getDurationSkill1() * 60) {
			this->setIsNoDie(false);
			this->changeBodyCategoryBits(BITMASK_HERO);
			this->changeBodyMaskBits(BITMASK_FLOOR | BITMASK_ENEMY | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION);
			this->getB2Body()->SetGravityScale(1);
			auto currentVelY = getB2Body()->GetLinearVelocity().y;
			this->getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));
			setIsDoneDuration1(true);
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}

	}, 1.0f / 60, "KeySkill1");		//  run every delta second
}


void HoangDuocSu::doCounterSkill1()
{

}

void HoangDuocSu::doCounterSkill2()
{
	
}
	

void HoangDuocSu::doCounterSkill3()
{
	
}


// SLASH
void HoangDuocSu::createSlash()
{
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;
	slash = SkeletonAnimation::createWithFile("Animation/HoangDuocSu/slash2.json", "Animation/HoangDuocSu/slash2.atlas", scale);
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/HoangDuocSu/slash1.json", "Animation/HoangDuocSu/slash1.atlas", scale);
	slashLand->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero() * 0.3f, this->getTrueRadiusOfHero() * 0.7f);
	slashLand->update(0.0f);
	slashLand->setVisible(false);

	this->addChild(slashLand);
}

void HoangDuocSu::runSlash()
{
	slash->setVisible(true);
	slash->clearTracks();
	slash->addAnimation(0, "slash2", false);
	slash->setToSetupPose();
}

void HoangDuocSu::runSlashLand()
{
	slashLand->setVisible(true);
	slashLand->clearTracks();
	slashLand->addAnimation(0, "slash1", false);
	slashLand->setToSetupPose();
}

void HoangDuocSu::addStuff()
{
	// slash here
	//createSlash();

	BaseHero::addStuff();
}

void HoangDuocSu::createPool()
{
	
}

void HoangDuocSu::run()
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

void HoangDuocSu::normalJump()
{
	BaseHero::normalJump();
	clearTracks();
	addAnimation(0, "jump", false);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("jump");
}

void HoangDuocSu::doubleJump()
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

void HoangDuocSu::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();

	getSmokeRun()->setVisible(false);

	//log("land");
}

void HoangDuocSu::die()
{
	BaseHero::die();
	AudioManager::playSound(SOUND_DQDIE);
}

void HoangDuocSu::attackNormal()
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

	//log("atttack*");
	setToSetupPose();

	getSlashBreak()->setVisible(false);
}

void HoangDuocSu::attackLanding()
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

void HoangDuocSu::injured()
{
	AudioManager::playSound(SOUND_DQHIT);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();

	//log("injured");

}

void HoangDuocSu::listener()
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
		else if (strcmp(getCurrent()->animation->name, "skill1") == 0) {
			getFSM()->revertToGlobalState();
			setIsPriorSkill1(false);
		}

		//// SKILL 2
		//else if (strcmp(getCurrent()->animation->name, "skill2") == 0) {
		//	getFSM()->revertToGlobalState();
		//	setIsPriorSkill2(false);
		//}

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

void HoangDuocSu::stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3)
{
	if (stopSkill1 && !getIsDoneDuration1()) {
		setIsDoneDuration1(true);
		unschedule("KeySkill1");
		checkDurationSkill1 = 0;
	}

	if (stopSkill2 && !getIsDoneDuration2()) {
		setIsDoneDuration2(true);
		unschedule("KeySkill2");
		checkDurationSkill2 = 0;
	}

	if (stopSkill3 && !getIsDoneDuration3()) {
		setIsDoneDuration3(true);
		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}
}

void HoangDuocSu::doDestroyBodies(b2World* world)
{
	BaseHero::doDestroyBodies(world);
}

void HoangDuocSu::updateMe(float dt)
{
	BaseHero::updateMe(dt);

	getFSM()->Update();

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

	if (!getIsPriorAttack() && !getIsPriorInjured() && !getIsPriorSkill1() /* && !getIsPriorSkill2()*/ && !getIsPriorSkill3()) {

		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround() && !getIsDriverEagle()) {
			getFSM()->changeState(MRun);
		}
	}

}
