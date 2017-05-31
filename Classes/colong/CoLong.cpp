#include "CoLong.h"
#include "manager/RefManager.h"
#include "manager/AudioManager.h"

CoLong::CoLong(string p_sJsonFile, string p_sAtlasFile, float p_fScale) : BaseHero(p_sJsonFile, p_sAtlasFile, p_fScale) {

}

CoLong * CoLong::create(string p_sJsonFile, string p_sAtlasFile, float p_fScale) {
	CoLong *_pCoLong = new CoLong(p_sJsonFile, p_sAtlasFile, p_fScale);
	if (_pCoLong && _pCoLong->init())
	{
		_pCoLong->autorelease();
		_pCoLong->setTag(TAG_HERO);

		_pCoLong->update(0.0f);

		_pCoLong->stateMachine = new StateMachine(_pCoLong);
		_pCoLong->stateMachine->setCurrentState(MLand);

		_pCoLong->setBoxHeight(_pCoLong->getBoundingBox().size.height / 3.4f);

		_pCoLong->blash = Sprite::create("Animation/CoLong/blash.png");
		_pCoLong->blash->setScale(p_fScale / 2);
		_pCoLong->blash->setPosition(_pCoLong->getContentSize() / 2);
		_pCoLong->blash->setVisible(false);
		_pCoLong->addChild(_pCoLong->blash);

		return _pCoLong;
	}
	else
	{
		delete _pCoLong;
		_pCoLong = nullptr;
		return nullptr;
	}
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
	cham->changeBodyMaskBits(BITMASK_SLASH | BITMASK_BOSS | BITMASK_ENEMY);

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
	auto * clone = (SkeletonAnimation*) poolSkill2->getObjectAtIndex(indexSkill2++);

	clone->clearTracks();
	clone->addAnimation(0, "skill2", false);
	clone->setToSetupPose();

	auto parentGameScene = (GameScene*)this->getParent();

	if (!m_lEnemiesSelectedBySkill2.empty()) {
		clone->setPosition(Point(p_ptPoint.x - m_lEnemiesSelectedBySkill2.front()->getContentSize().width * m_lEnemiesSelectedBySkill2.front()->getScaleX(),
			p_ptPoint.y));
		auto enemy = m_lEnemiesSelectedBySkill2.front();
		m_lEnemiesSelectedBySkill2.pop_front();
		enemy->die();

		this->setScore(this->getScore() + 15);
	}
	else {
		clone->setPosition(p_ptPoint);
	}

	if (!isAddedAll) {
		/*clone->setEndListener([&](int trackIndex) {
			if ((strcmp(clone->getCurrent()->animation->name, "skill2") == 0)) {

			}
		});*/

		parentGameScene->addChild(clone, ZORDER_SMT);
	}

	if (indexSkill2 == 3) {
		indexSkill2 = 0;
		if (!isAddedAll) isAddedAll = true;
	}
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

	changeBodyMaskBits(BITMASK_FLOOR | BITMASK_COIN_BULLION | BITMASK_BOSS);
	
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_SWORD);
	
	getSmokeRun()->setVisible(false);

	clearTracks();
	addAnimation(0, "skill3", true);
	setToSetupPose();

	this->schedule([&](float dt) {
		checkDurationSkill3++;

		if (checkDurationSkill3 >= getDurationSkill3() * 10) {
			setIsDoneDuration3(true);
			if (getOnGround()) {
				getFSM()->changeState(MRun);
				run();
			}
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
			if (_pTempDPC->getB2Body() != nullptr) {
				gameLayer->world->DestroyBody(_pTempDPC->getB2Body());
				_pTempDPC->setB2Body(nullptr);
				_pTempDPC->setVisible(false);
			}		
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

	if (m_pRadaSkill1 != nullptr) {
		m_pRadaSkill1->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
		m_pRadaSkill2->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
		m_pRadaSkill3->getB2Body()->SetTransform(this->getB2Body()->GetPosition(), 0.0f);
	}

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
	m_pRadaSkill1->changeBodyMaskBits(BITMASK_ENEMY);


	m_pRadaSkill2 = Rada::create("Animation/CoLong/blash.png");
	m_pRadaSkill2->setScale(SCREEN_SIZE.width / m_pRadaSkill2->getContentSize().width / 1.5f);
	m_pRadaSkill2->setVisible(false);
	m_pRadaSkill2->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill2->changeBodyCategoryBits(BITMASK_RADA_SKILL_2);
	m_pRadaSkill2->changeBodyMaskBits(BITMASK_ENEMY);


	m_pRadaSkill3 = Rada::create("Animation/CoLong/blash.png");
	m_pRadaSkill3->setScale(SCREEN_SIZE.width / m_pRadaSkill3->getContentSize().width / 5.0f);
	m_pRadaSkill3->setVisible(false);
	m_pRadaSkill3->initCirclePhysic(p_pWorld, Vec2(this->getB2Body()->GetPosition().x, this->getB2Body()->GetPosition().y));
	m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
	m_pRadaSkill3->changeBodyMaskBits(BITMASK_ENEMY | BITMASK_SLASH | BITMASK_BOSS | BITMASK_WOODER | BITMASK_COIN_BAG);

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
	BaseHero::initCirclePhysic(world, pos);

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

	poolSkill2 = CCArray::createWithCapacity(3);
	poolSkill2->retain();

	auto scale = this->getTrueRadiusOfHero() * 1.4f / 250;
	for (int i = 0; i < 3; ++i) {
		SkeletonAnimation * clone = new SkeletonAnimation("Animation/CoLong/skill2.json", "Animation/CoLong/skill2.atlas", scale);
		poolSkill2->addObject(clone);
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

			if (getFSM()->globalState == MDoubleJump || getFSM()->globalState == MRevive) {
				getFSM()->setPreviousState(MAttack);
				getFSM()->setGlobalState(MLand);
			}

			getFSM()->revertToGlobalState();

		}

		else if (strcmp(getCurrent()->animation->name, "skill3") == 0) {
			changeBodyMaskBits(BITMASK_FLOOR | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BULLION | BITMASK_ENEMY);
			m_pRadaSkill3->changeBodyCategoryBits(BITMASK_WOODER);
		}

		else if (strcmp(getCurrent()->animation->name, "die") == 0) {
			this->pause();
			auto gamelayer = (GameScene*)this->getParent();
			gamelayer->dieGame();
		} 

	});
}

void CoLong::stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3)
{
	if (stopSkill1 && !getIsDoneDuration1()) {
		setIsDoneDuration1(true);
		checkDurationSkill1 = 0;
		unschedule("KeySkill1");
	}

	if (stopSkill2 && !getIsDoneDuration2()) {
		setIsDoneDuration2(true);
		unschedule("KeySkill2");
		checkDurationSkill2 = 0;
	}

	if (stopSkill3 && !getIsDoneDuration3()) {
		AudioManager::stopSoundForever(keysoundSKill3);
		setIsDoneDuration3(true);
		unschedule("KeySkill3");
		checkDurationSkill3 = 0;
	}
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

void CoLong::die() {
	BaseHero::die();
	AudioManager::playSound(SOUND_CLDIE);
	//log("die");
}

void CoLong::attackNormal() {
	if (!getIsDoneDuration3()) {		// is in Skill 3
		getFSM()->revertToGlobalState();
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
		getFSM()->revertToGlobalState();
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
		//log("Injured");
	}
}

