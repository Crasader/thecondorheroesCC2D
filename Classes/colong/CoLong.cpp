#include "CoLong.h"
#include "manager/JSonHeroManager.h"
#include "AudioEngine.h"

CoLong::CoLong(string p_sJsonFile, string p_sAtlasFile, float p_fScale) : BaseHero(p_sJsonFile, p_sAtlasFile, p_fScale) {
	checkDurationSkill1 = 0;
	checkDurationSkill2 = 0;
	checkDurationSkill3 = 0;
}

CoLong * CoLong::create(string p_sJsonFile, string p_sAtlasFile, float p_fScale) {
	CoLong *_pCoLong = new CoLong(p_sJsonFile, p_sAtlasFile, p_fScale);
	_pCoLong->setTag(TAG_HERO);

	_pCoLong->update(0.0f);

	_pCoLong->stateMachine = new StateMachine(_pCoLong);
	_pCoLong->stateMachine->setCurrentState(MLand);

	_pCoLong->setMoveVel(_pCoLong->SCREEN_SIZE.width / PTM_RATIO / 2.3f);
	_pCoLong->setJumpVel(_pCoLong->SCREEN_SIZE.height * 1.4f / PTM_RATIO);

	_pCoLong->health = JSHERO->getBaseHP();

	// set Duration here
	_pCoLong->setDurationSkill1(JSHERO->getDurationSkill1());
	_pCoLong->setDurationSkill2(JSHERO->getDurationSkill2());
	_pCoLong->setDurationSkill3(JSHERO->getDurationSkill3());

	_pCoLong->numberOfJump = 2;
	_pCoLong->coinExplored = 0;
	_pCoLong->score = 0;

	_pCoLong->setBoxHeight(_pCoLong->getBoundingBox().size.height / 3.4f);

	_pCoLong->setOnGround(false);
	_pCoLong->setIsPriorInjured(false);		// future, we need to add props into base class
	_pCoLong->setIsPriorAttack(false);
	_pCoLong->setIsPriorSkill1(false);
	_pCoLong->setIsPriorSkill2(false);
	_pCoLong->setIsPriorSkill3(false);

	_pCoLong->setIsDoneDuration1(true);
	_pCoLong->setIsDoneDuration2(true);
	_pCoLong->setIsDoneDuration3(true);

	return _pCoLong;
}

// SKILL 1
void CoLong::createDocPhongCham(Point p_ptStartPoint, Point p_ptEndPoint) {
	auto thc = TieuHonChuong::create("Animation/CoLong/skill1.png");
	thc->setScale(this->getTrueRadiusOfHero() * 1.5f / thc->getContentSize().width);
	auto world = this->getB2Body()->GetWorld();

	thc->setPosition(p_ptStartPoint);
	thc->initCirclePhysic(world, thc->getPosition());
	thc->changeBodyCategoryBits(BITMASK_SWORD);
	thc->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS);

	this->getParent()->addChild(thc, ZORDER_ENEMY);

	//thc->getB2Body()->SetTransform(thc->getB2Body()->GetPosition(), angle);
	thc->setAngel(atanf((p_ptEndPoint.y - p_ptStartPoint.y) / (p_ptEndPoint.x - p_ptStartPoint.x)));
	thc->getB2Body()->SetLinearVelocity(b2Vec2(thc->getB2Body()->GetLinearVelocity().x * 2.0f, thc->getB2Body()->GetLinearVelocity().y * 2.0f));
	thc->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), atanf((p_ptEndPoint.y - p_ptStartPoint.y) / (p_ptEndPoint.x - p_ptStartPoint.x)));
	m_lDocPhongCham.push_back(thc);
	m_lEnemiesSelectedBySkill1.pop_front();
}

void CoLong::doCounterSkill1() {
	this->schedule([&](float dt) {
		checkDurationSkill1++;
		if (checkDurationSkill1 >= getDurationSkill1() * 10) {
			setIsDoneDuration1(true);
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
			m_bEndSkill = true;
		}
	}, 0.1f, "KeySkill1");
}


// SKILL 2
void CoLong::createNgocNuKiemPhap(Point p_ptPoint) {
	auto scale = this->getTrueRadiusOfHero() * 1.4f / 250;
	SkeletonAnimation * clone = new SkeletonAnimation("Animation/CoLong/skill2.json", "Animation/CoLong/skill2.atlas", scale);
	clone->setPosition(Point(p_ptPoint.x - m_lEnemiesSelectedBySkill2.front()->getContentSize().width * m_lEnemiesSelectedBySkill2.front()->getScaleX(),
		p_ptPoint.y));

	this->getParent()->addChild(clone, ZORDER_SMT);
	clone->addAnimation(0, "skill2", false);
	auto enemy = m_lEnemiesSelectedBySkill2.front();

	auto parentGameScene = (GameScene*)this->getParent();
	parentGameScene->updateScore(15);
	m_lEnemiesSelectedBySkill2.pop_front();

	enemy->die();
	parentGameScene->updateMultiKills();
}

void CoLong::doCounterSkill2() {
	this->schedule([&](float dt) {
		checkDurationSkill2++;
		if (checkDurationSkill2 >= getDurationSkill2() * 10) {
			setIsDoneDuration2(true);
			checkDurationSkill2 = 0;
			unschedule("KeySkill2");
			m_bEndSkill = true;
		}
	}, 0.1f, "KeySkill2");
}

// SKILL 3
void CoLong::doCounterSkill3() {
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_SWORD);

	clearTracks();
	addAnimation(0, "skill3", true);
	setToSetupPose();

	EM->getSmokeRun()->setVisible(false);

	this->schedule([&](float dt) {
		checkDurationSkill3++;

		if (checkDurationSkill3 >= getDurationSkill3() * 10) {
			m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
			setIsDoneDuration3(true);
			checkDurationSkill3 = 0;
			unschedule("KeySkill3");
			m_bEndSkill = true;
		}
	}, 0.1f, "KeySkill3");
}

void CoLong::updateMe(float p_fDelta) {
	BaseHero::updateMe(p_fDelta);
	getFSM()->Update();

	if (!m_lDocPhongCham.empty()) {
		TieuHonChuong *_pTempDPC = m_lDocPhongCham.front();
		if (_pTempDPC->getIsCollide() || _pTempDPC->getPositionX() - (this->getPositionX() + SCREEN_SIZE.width * 0.25f) > SCREEN_SIZE.width / 2) {
			auto gameLayer = (GameScene*) this->getParent();

			gameLayer->world->DestroyBody(_pTempDPC->getB2Body());
			_pTempDPC->setB2Body(nullptr);
			_pTempDPC->removeFromParentAndCleanup(true);
		}
		else {
			_pTempDPC->updateMe(p_fDelta);
			m_lDocPhongCham.push_back(_pTempDPC);
		}
		m_lDocPhongCham.pop_front();
	}


	if (EM->getSmokeRun()->isVisible())
		EM->getSmokeRun()->setPosition(this->getPosition());

	/*if (m_bIsActiveSkill3) {
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel() * 1.5f, currentVelY));
	}
	else {
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));
	}*/

	if (getB2Body() == nullptr)
		return;

	m_pRadaSkill1->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
	m_pRadaSkill2->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
	m_pRadaSkill3->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);

	auto currentVelY = getB2Body()->GetLinearVelocity().y;
	if (getFSM()->currentState == MDie) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, currentVelY));
		return;
	}

	if (getFSM()->currentState == MLandRevive) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
		return;
	}

	/*if (getPositionY() + getTrueRadiusOfHero() * 2 < 0) {
		getB2Body()->SetTransform(b2Vec2(SCREEN_SIZE.width * 0.25f / PTM_RATIO,
			SCREEN_SIZE.height / PTM_RATIO), getB2Body()->GetAngle());
		return;
	}*/

	static float a = 0.0f;
	if (m_bEndSkill && a < 5.0f) {
		a += 0.1f;
		m_pRadaSkill1->setVisible(true);
		m_pRadaSkill1->setScale(a);
		m_pRadaSkill1->updateMe(p_fDelta);
	}
	else {
		m_bEndSkill = false;
		m_pRadaSkill1->setVisible(false);
		a = 0.0f;
	}

	if (!getIsDoneDuration1()) {
		if (!this->m_lEnemiesSelectedBySkill1.empty()) {
			createDocPhongCham(this->getPosition(), m_lEnemiesSelectedBySkill1.front()->getPosition());
		}
	}
	if (!getIsDoneDuration2()) {
		if (!this->m_lEnemiesSelectedBySkill2.empty()) {
			createNgocNuKiemPhap(m_lEnemiesSelectedBySkill2.front()->getPosition());
		}
	}

	getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

	if (getSlash()->isVisible())
		getSlash()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero(),
			this->getPositionY() + this->getTrueRadiusOfHero() * 0.7f);

	if (getSlashLand()->isVisible())
		getSlashLand()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero() * 0.3f,
			this->getPositionY() + this->getTrueRadiusOfHero() * 0.7f);

	if (!getIsPriorAttack() && !getIsPriorInjured() && getIsDoneDuration3()) {
		if (getB2Body()->GetLinearVelocity().y < 0) {
			getFSM()->changeState(MLand);
			return;
		}

		if (getOnGround() && !getIsDriverEagle()) {
			getFSM()->changeState(MRun);
		}
	}
}

void CoLong::createRada(b2World *p_pWorld) {
	m_pRadaSkill1 = Rada::create("Animation/CoLong/circle-blash-blue.png");
	m_pRadaSkill1->setScale(SCREEN_SIZE.width / m_pRadaSkill1->getContentSize().width / 1.0f);
	m_pRadaSkill1->setVisible(false);
	m_pRadaSkill1->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill1->changeBodyCategoryBits(BITMASK_RADA_SKILL_1);
	m_pRadaSkill1->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2);

	this->getParent()->addChild(m_pRadaSkill1, ZORDER_SMT);
	m_pRadaSkill1->setAngel(0.0f);

	m_pRadaSkill2 = Rada::create("Animation/CoLong/circle-blash-blue.png");
	m_pRadaSkill2->setScale(SCREEN_SIZE.width / m_pRadaSkill2->getContentSize().width / 1.5f);
	m_pRadaSkill2->setVisible(false);
	m_pRadaSkill2->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill2->changeBodyCategoryBits(BITMASK_RADA_SKILL_2);
	m_pRadaSkill2->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2);

	this->getParent()->addChild(m_pRadaSkill2, ZORDER_SMT);
	m_pRadaSkill2->setAngel(0.0f);

	m_pRadaSkill3 = Rada::create("Animation/CoLong/circle-blash-blue.png");
	m_pRadaSkill3->setScale(SCREEN_SIZE.width / m_pRadaSkill3->getContentSize().width / 5.0f);
	m_pRadaSkill3->setVisible(false);
	m_pRadaSkill3->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
	m_pRadaSkill3->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG);

	this->getParent()->addChild(m_pRadaSkill3, ZORDER_SMT);
	m_pRadaSkill3->setAngel(0.0f);
}

// SLASH
void CoLong::createSlash() {
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;  // 400: hieght of spine
	slash = SkeletonAnimation::createWithFile("Animation/CoLong/slash4.json", "Animation/CoLong/slash4.atlas", scale);
	slash->update(0.0f);
	slash->setVisible(false);
	this->getParent()->addChild(slash, ZORDER_SMT);

	auto scaleLand = this->getTrueRadiusOfHero() * 1.8f / 400;  // 400: hieght of spine
	slashLand = SkeletonAnimation::createWithFile("Animation/CoLong/slash3.json", "Animation/CoLong/slash3.atlas", scaleLand);
	slashLand->update(0.0f);
	slashLand->setVisible(false);
	this->getParent()->addChild(slashLand, ZORDER_SMT);
}

void CoLong::initCirclePhysic(b2World * world, Point pos) {
	b2CircleShape circle_shape;
	circle_shape.m_radius = getBoxHeight() / PTM_RATIO;

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

	// rada
	createRada(world);
}

void CoLong::addStuff()
{
	// slash here
	createSlash();
}

void CoLong::listener() {
	this->setEndListener([&](int trackIndex) {

		if ((strcmp(getCurrent()->animation->name, "injured") == 0)) {

			this->getBloodScreen()->setVisible(false);

			setIsPriorInjured(false);
			if (getFSM()->globalState == MSKill3 || getFSM()->globalState == MAttack) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MRun);
			}

			if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();

		}

		else if (strcmp(getCurrent()->animation->name, "revive") == 0) {
			EM->getReviveMe()->setVisible(false);
			getFSM()->changeState(MLand);
			auto gameLayer = (GameScene*) this->getParent();
			initCirclePhysic(gameLayer->world, this->getPosition());
		}

		else if ((strcmp(getCurrent()->animation->name, "attack1") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack2") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack3") == 0)) {

			changeSwordCategoryBitmask(BITMASK_ENEMY);

			setIsPriorAttack(false);
			if (getFSM()->globalState == MSKill3 || getFSM()->globalState == MInjured) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MRun);
			}

			if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();

		}

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			auto gamelayer = (GameScene*)this->getParent();
			gamelayer->dieGame();
		}

	});
}

void CoLong::doDestroyBodies(b2World *world)
{
	BaseHero::doDestroyBodies(world);
	world->DestroyBody(m_pRadaSkill1->getB2Body());
	world->DestroyBody(m_pRadaSkill2->getB2Body());
	world->DestroyBody(m_pRadaSkill3->getB2Body());

	m_pRadaSkill1->setB2Body(nullptr);
	m_pRadaSkill2->setB2Body(nullptr);
	m_pRadaSkill3->setB2Body(nullptr);
}

void CoLong::run() {
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

void CoLong::runSlash() {
	slash->setVisible(true);
	slash->clearTracks();
	slash->addAnimation(0, "slash4", false);
	slash->setToSetupPose();
}

void CoLong::runSlashLand() {
	slashLand->setVisible(true);
	slashLand->clearTracks();
	slashLand->addAnimation(0, "slash3", false);
	slashLand->setToSetupPose();
}

void CoLong::normalJump() {
	if (!getIsDoneDuration3()) {
	}
	else {
		clearTracks();
		addAnimation(0, "jump", false);
		setToSetupPose();
		EM->getSmokeRun()->setVisible(false);
	}
}

void CoLong::doubleJump() {
	if (!getIsDoneDuration3()) {
	}
	else {
		clearTracks();
		addAnimation(0, "jumpx2", false);
		setToSetupPose();

		EM->getSmokeJumpX2()->setPosition(this->getPosition());
		EM->getSmokeJumpX2()->setVisible(true);
		EM->smokeJumpX2Ani();
	}
}

void CoLong::landing() {
	if (!getIsDoneDuration3()) {
	}
	else {
		clearTracks();
		addAnimation(0, "landing", true);
		setToSetupPose();

		EM->getSmokeRun()->setVisible(false);
	}
}

void CoLong::idle() {
	clearTracks();
	addAnimation(0, "idle", false);
	setToSetupPose();
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	EM->getSmokeRun()->setVisible(false);
}

void CoLong::die() {
	--dieHard;
	if (dieHard < 0) {
		log("Die Hard");
		return;
	}

	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getB2Body()->SetLinearDamping(10);

	EM->getSmokeRun()->setVisible(false);

	log("die");
}

void CoLong::attackNormal() {
	if (!getIsDoneDuration3()) {		// is in Skill 3

	}
	else {
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

		log("atttack");
		setToSetupPose();

		EM->getSlashBreak()->setVisible(false);
	}

}

void CoLong::attackLanding() {
	if (!getIsDoneDuration3()) {		// is in Skill 3

	}
	else {
		changeSwordCategoryBitmask(BITMASK_SWORD);
		setIsPriorAttack(true);
		runSlashLand();

		clearTracks();
		addAnimation(0, "attack3", false);
		setToSetupPose();

		EM->getSlashBreak()->setVisible(false);
	}
}

void CoLong::injured() {
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();
}

void CoLong::revive()
{
	clearTracks();
	addAnimation(0, "revive", false);
	setToSetupPose();

	EM->getSmokeRun()->setVisible(false);
	EM->getReviveMe()->setPosition(this->getPositionX() + getTrueRadiusOfHero() / 2, this->getPositionY());
	EM->getReviveMe()->setVisible(true);
	EM->reviveAni();

	log("revive");
}

void CoLong::die(Point p_ptPositionOfCammera) {
}