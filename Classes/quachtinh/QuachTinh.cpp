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

	quachTinh->setBoxHeight(quachTinh->getBoundingBox().size.height / 6.7f);

	//
	quachTinh->blash = Sprite::create("Animation/QuachTinh/blash.png");
	quachTinh->blash->setScale(scale / 2);
	quachTinh->blash->setPosition(quachTinh->getContentSize() / 2);
	quachTinh->blash->setVisible(false);
	quachTinh->addChild(quachTinh->blash);

	return quachTinh;
}


void QuachTinh::doCounterSkill1()
{
}

void QuachTinh::doCounterSkill2()
{
}

void QuachTinh::doCounterSkill3()
{
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

void QuachTinh::initCirclePhysic(b2World * world, Point pos)
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

void QuachTinh::addStuff()
{
	// slash here
	createSlash();

	BaseHero::addStuff();
}

void QuachTinh::createPool()
{
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

void QuachTinh::attackLanding()
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

void QuachTinh::attackBySkill1()
{
	AudioManager::playSound(SOUND_DQSKILL1);
	clearTracks();
	addAnimation(0, "attack4", false);
	setToSetupPose();
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
