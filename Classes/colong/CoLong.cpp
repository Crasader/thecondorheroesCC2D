#include "CoLong.h"
#include "RefManager.h"
#include "AudioManager.h"

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

	_pCoLong->health = REF->getCurrentHealth();

	// set Duration here
	_pCoLong->setDurationSkill1(REF->getDurationSkill_1());
	_pCoLong->setDurationSkill2(REF->getDurationSkill_2());
	_pCoLong->setDurationSkill3(REF->getDurationSkill_3());

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

	_pCoLong->blash = Sprite::create("Animation/CoLong/blash.png");
	_pCoLong->blash->setScale(p_fScale / 2);
	_pCoLong->blash->setPosition(_pCoLong->getContentSize() / 2);
	_pCoLong->blash->setVisible(false);
	_pCoLong->addChild(_pCoLong->blash);

	return _pCoLong;
}

// SKILL 1
void CoLong::createDocPhongCham(Point p_ptStartPoint, Point p_ptEndPoint) {
	AudioManager::playSound(SOUND_CLSKILL1);
	auto cham = (TieuHonChuong*) poolSkill1->getObjectAtIndex(indexSkill1++);
	cham->setVisible(true);
	cham->setIsCollide(false);
	float angle = atanf((p_ptEndPoint.y - p_ptStartPoint.y) / (p_ptEndPoint.x - p_ptStartPoint.x));
	cham->setVel(b2Vec2(SCREEN_SIZE.width * 1.3f / PTM_RATIO * cosf(angle) * 2, 
		SCREEN_SIZE.width * 1.3f / PTM_RATIO * sinf(angle) * 2));

	auto gameLayer = (GameScene*) this->getParent();

	cham->setPosition(p_ptStartPoint);
	cham->initCirclePhysic(gameLayer->world, cham->getPosition());
	cham->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), angle);
	cham->changeBodyCategoryBits(BITMASK_SWORD);
	cham->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS);

	if (!cham->getIsAdded()) {
		this->getParent()->addChild(cham, ZORDER_ENEMY);
		cham->setIsAdded(true);
	}
	
	m_lDocPhongCham.push_back(cham);
	if (!m_lEnemiesSelectedBySkill1.empty()) {
		m_lEnemiesSelectedBySkill1.pop_front();
	}

	if (indexSkill1 == 5) indexSkill1 = 0;
}

void CoLong::doCounterSkill1() {
	this->schedule([&](float dt) {
		checkDurationSkill1++;
		if (checkDurationSkill1 >= getDurationSkill1() * 10) {
			setIsDoneDuration1(true);
			checkDurationSkill1 = 0;
			unschedule("KeySkill1");
		}
	}, 0.1f, "KeySkill1");
}


// SKILL 2
void CoLong::createNgocNuKiemPhap(Point p_ptPoint) {
	AudioManager::playSound(SOUND_CLSKILL2);
	auto scale = this->getTrueRadiusOfHero() * 1.4f / 250;
	SkeletonAnimation * clone = new SkeletonAnimation("Animation/CoLong/skill2.json", "Animation/CoLong/skill2.atlas", scale);
	auto parentGameScene = (GameScene*)this->getParent();

	if (!m_lEnemiesSelectedBySkill2.empty()) {
		clone->setPosition(Point(p_ptPoint.x - m_lEnemiesSelectedBySkill2.front()->getContentSize().width * m_lEnemiesSelectedBySkill2.front()->getScaleX(),
			p_ptPoint.y));
		auto enemy = m_lEnemiesSelectedBySkill2.front();
		m_lEnemiesSelectedBySkill2.pop_front();
		enemy->die();

		auto hero = parentGameScene->getHero();
		hero->setScore(hero->getScore() + 15);
	}
	else {
		clone->setPosition(p_ptPoint);
	}

	parentGameScene->addChild(clone, ZORDER_SMT);
	clone->addAnimation(0, "skill2", false);
}

void CoLong::doCounterSkill2() {
	this->schedule([&](float dt) {
		checkDurationSkill2++;
		if (checkDurationSkill2 >= getDurationSkill2() * 10) {
			setIsDoneDuration2(true);
			checkDurationSkill2 = 0;
			unschedule("KeySkill2");
		}
	}, 0.1f, "KeySkill2");
}

// SKILL 3
void CoLong::doCounterSkill3() {
	keysoundSKill3 = AudioManager::playSoundForever(SOUND_CLSKILL3);
	changeBodyMaskBits(BITMASK_FLOOR | BITMASK_COIN | BITMASK_COIN_BULLION | BITMASK_ITEM);
	
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_SWORD);
	
	getSmokeRun()->setVisible(false);

	clearTracks();
	addAnimation(0, "skill3", true);
	setToSetupPose();

	this->schedule([&](float dt) {
		checkDurationSkill3++;

		if (checkDurationSkill3 >= getDurationSkill3() * 10) {
			changeBodyMaskBits(BITMASK_FLOOR | BITMASK_COIN | BITMASK_SLASH | BITMASK_BOSS | BITMASK_TOANCHAN1 | BITMASK_COIN_BULLION | BITMASK_ITEM);
			m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
			setIsDoneDuration3(true);
			checkDurationSkill3 = 0;
			AudioManager::stopSoundForever(keysoundSKill3);
			unschedule("KeySkill3");
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
			_pTempDPC->setVisible(false);
		}
		else {
			_pTempDPC->updateMe();
			m_lDocPhongCham.push_back(_pTempDPC);
		}
		m_lDocPhongCham.pop_front();
	}



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

	if (this->getPositionY() < 0) {
		return;
	}

	if (!getIsDoneDuration1()) {
		if (!this->m_lEnemiesSelectedBySkill1.empty()) {
			auto enemy = m_lEnemiesSelectedBySkill1.front();
			createDocPhongCham(this->getPosition(), enemy->getPosition() + enemy->getParent()->getPosition());
		}
	}
	if (!getIsDoneDuration2()) {
		if (!this->m_lEnemiesSelectedBySkill2.empty()) {
			auto enemy = m_lEnemiesSelectedBySkill2.front();
			createNgocNuKiemPhap(enemy->getPosition() + enemy->getParent()->getPosition());
		}
	}

	if(!isDriverEagle)
		getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));


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

void CoLong::updateAttackBossAsTarget(BaseEnemy *p_pBoss) {
	if (getB2Body() == nullptr) {
		return;
	}
	if (!getIsDoneDuration1()) {
		static float b = 1.0f;
		if (b < 1.0f) {
			b += 1.0f / 60.0f;
		}
		else {
			createDocPhongCham(this->getPosition(), p_pBoss->getPosition());
			b = 0.0f;
		}
	}

	if (!getIsDoneDuration2()) {
		static float c = 1.0f;
		if (c < 1.0f) {
			c += 1.0f / 60.0f;
		}
		else {
			createNgocNuKiemPhap(p_pBoss->getPosition());
			p_pBoss->die();
			c = 0.0f;
		}
	}
}

void CoLong::createRada(b2World *p_pWorld) {
	m_pRadaSkill1 = Rada::create("Animation/CoLong/blash.png");
	m_pRadaSkill1->setScale(SCREEN_SIZE.width / m_pRadaSkill1->getContentSize().width / 1.0f);
	m_pRadaSkill1->setVisible(false);
	m_pRadaSkill1->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill1->changeBodyCategoryBits(BITMASK_RADA_SKILL_1);
	m_pRadaSkill1->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2);


	m_pRadaSkill2 = Rada::create("Animation/CoLong/blash.png");
	m_pRadaSkill2->setScale(SCREEN_SIZE.width / m_pRadaSkill2->getContentSize().width / 1.5f);
	m_pRadaSkill2->setVisible(false);
	m_pRadaSkill2->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill2->changeBodyCategoryBits(BITMASK_RADA_SKILL_2);
	m_pRadaSkill2->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2);


	m_pRadaSkill3 = Rada::create("Animation/CoLong/blash.png");
	m_pRadaSkill3->setScale(SCREEN_SIZE.width / m_pRadaSkill3->getContentSize().width / 5.0f);
	m_pRadaSkill3->setVisible(false);
	m_pRadaSkill3->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
	m_pRadaSkill3->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG);

}

// SLASH
void CoLong::createSlash() {
	auto scale = this->getTrueRadiusOfHero() * 1.8f / 400;  // 400: hieght of spine
	slash = SkeletonAnimation::createWithFile("Animation/CoLong/slash4.json", "Animation/CoLong/slash4.atlas", scale);
	slash->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero(), this->getTrueRadiusOfHero() * 0.7f);
	slash->update(0.0f);
	slash->setVisible(false);
	this->addChild(slash);

	slashLand = SkeletonAnimation::createWithFile("Animation/CoLong/slash3.json", "Animation/CoLong/slash3.atlas", scale);
	slashLand->setPosition(this->getContentSize().width / 2 + this->getTrueRadiusOfHero() * 0.3f, this->getTrueRadiusOfHero() * 0.7f);
	slashLand->update(0.0f);
	slashLand->setVisible(false);
	this->addChild(slashLand);
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
	fixtureDef.filter.maskBits = BITMASK_FLOOR | BITMASK_COIN | BITMASK_ITEM |
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
	createSlash();

	BaseHero::addStuff();
}

void CoLong::createPool()
{
	poolSkill1 = CCArray::createWithCapacity(5);
	poolSkill1->retain();

	for (int i = 0; i < 5; ++i) {
		auto cham = TieuHonChuong::create("cham.png");
		cham->setScale(this->getTrueRadiusOfHero() * 1.5f / cham->getContentSize().width);
		poolSkill1->addObject(cham);
	}
}

void CoLong::listener() {
	
	this->setEndListener([&](int trackIndex) {

		if ((strcmp(getCurrent()->animation->name, "injured") == 0)) {

			this->getBloodScreen()->setVisible(false);

			setIsPriorInjured(false);

			if (getFSM()->globalState == MAttack) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MRun);
			}

			if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MInjured);
				getFSM()->setGlobalState(MLand);
			}

			if(getIsDoneDuration3())
				getFSM()->revertToGlobalState();

		}

		else if (strcmp(getCurrent()->animation->name, "revive") == 0) {
			getReviveMe()->setVisible(false);
			getFSM()->changeState(MLand);
			auto gameLayer = (GameScene*) this->getParent();
			initCirclePhysic(gameLayer->world, this->getPosition());
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

			if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();

		}

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			this->pause();
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

	if (!getIsDoneDuration3()) {

	}
	else {
		clearTracks();
		addAnimation(0, "run", true);
		setToSetupPose();

		if (getBloodScreen()->isVisible() && health > 1)
			getBloodScreen()->setVisible(false);

		if (!getSmokeRun()->isVisible()) {
			getSmokeRun()->setVisible(true);
		}
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
		BaseHero::normalJump();
		clearTracks();
		addAnimation(0, "jump", false);
		setToSetupPose();
		getSmokeRun()->setVisible(false);
	}
}

void CoLong::doubleJump() {
	if (!getIsDoneDuration3()) {
	}
	else {
		BaseHero::doubleJump();
		clearTracks();
		addAnimation(0, "jumpx2", false);
		setToSetupPose();

		getSmokeJumpX2()->setPosition(this->getPosition());
		getSmokeJumpX2()->setVisible(true);
		smokeJumpX2Ani();
	}
}

void CoLong::landing() {
	if (!getIsDoneDuration3()) {
	}
	else {
		clearTracks();
		addAnimation(0, "landing", true);
		setToSetupPose();

		getSmokeRun()->setVisible(false);
	}
}

void CoLong::idle() {
	clearTracks();
	addAnimation(0, "idle", false);
	setToSetupPose();
	getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	getSmokeRun()->setVisible(false);
}

void CoLong::die() {
	--dieHard;
	if (dieHard < 0) {
		//log("Die Hard");
		return;
	}

	if (!getIsDoneDuration1()) {
		setIsDoneDuration1(true);
		checkDurationSkill1 = 0;
		unschedule("KeySkill1");
	}

	if (!getIsDoneDuration2()) {
		setIsDoneDuration2(true);
		unschedule("KeySkill2");
		checkDurationSkill2 = 0;
	}

	if (!getIsDoneDuration3()) {
		setIsDoneDuration3(true);
		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}


	AudioManager::playSound(SOUND_CLDIE);
	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getB2Body()->SetLinearDamping(10);

	getSmokeRun()->setVisible(false);

	//log("die");
}

void CoLong::attackNormal() {
	if (!getIsDoneDuration3()) {		// is in Skill 3

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

void CoLong::attackLanding() {
	if (!getIsDoneDuration3()) {		// is in Skill 3

	}
	else {
		BaseHero::attackLanding();
		changeSwordCategoryBitmask(BITMASK_SWORD);
		setIsPriorAttack(true);
		runSlashLand();

		clearTracks();
		addAnimation(0, "attack3", false);
		setToSetupPose();

		getSlashBreak()->setVisible(false);
	}
}

void CoLong::injured() {
	if (!getIsDoneDuration3()) {		// is in Skill 3

	}
	else {
		AudioManager::playSound(SOUND_CLHIT);
		clearTracks();
		addAnimation(0, "injured", false);
		setToSetupPose();
		log("Injured");
	}
}

void CoLong::revive()
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

void CoLong::die(Point p_ptPositionOfCammera)
{

}
