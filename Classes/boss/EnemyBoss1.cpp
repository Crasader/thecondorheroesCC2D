#include "EnemyBoss1.h"
#include "BaseHero.h"
#include "AudioManager.h"

EnemyBoss1::EnemyBoss1(string jsonFile, string atlasFile, float scale) :BaseEnemy(jsonFile, atlasFile, scale)
{
	isNodie = false;
	isDie = false;
	control = 0;
	controlAttack = 2;
	controlState = 0;

	health = 15;
	exp = 50;
	baseVelocity = Vec2(SCREEN_SIZE.width / 2.3f, SCREEN_SIZE.height / 10);
	moveVelocity = Vec2(SCREEN_SIZE.height / 2, SCREEN_SIZE.height / 2);

	realtimeVec = Vec2(SCREEN_SIZE.width / 2.3f, SCREEN_SIZE.height / 10);
	realMoveVelocity = Vec2::ZERO;
	exxp = nullptr;
	randAt2 = 1;
	//lockState = false;
}

EnemyBoss1 * EnemyBoss1::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss1* boss = new EnemyBoss1(jsonFile, atlasFile, scale);
	boss->setAnimation(0, "idle", true);
	boss->update(0.0f);
	boss->state = new BossIdling();
	boss->setTag(TAG_BOSS);
	boss->scaleBoss = scale;
	boss->setLevelBoss(1);
	boss->autorelease();
	return boss;
}


void EnemyBoss1::idle()
{
	this->clearTracks();
	this->setAnimation(0, "idle", true);
	this->setToSetupPose();
}

void EnemyBoss1::attack()
{
	this->playSoundAttack1();
	this->isNodie = true;
	this->clearTracks();
	this->setAnimation(0, "attack", false);
	auto callfun = CallFunc::create([&] {
		this->creatHidenSlash((heroLocation - this->getPosition()).getAngle());
		this->setIsNodie(false);
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f),callfun));
}

void EnemyBoss1::attack2()
{
	this->playSoundAttack1();
	this->isNodie = true;
	this->clearTracks();
	this->setAnimation(0, "attack2", false);
}

void EnemyBoss1::fixStupid()
{
	float a = CCRANDOM_0_1();
	log("fixstupid %f", a);
	this->setRealMoveVelocity(Vec2(this->getmoveVelocity().x, this->getmoveVelocity().y*a));
	//log("fixstupid");
}



void EnemyBoss1::die()
{
	if (!isDie && !isNodie) {
		health--;
		if (health > 0) {
			this->playSoundHit();
			this->isNodie = true;
			this->clearTracks();
			this->setAnimation(0, "injured-red", false);
			this->setToSetupPose();
			this->scheduleOnce([&](float dt) {
				this->isNodie = false;
				this->clearTracks();
				this->setAnimation(0, "idle", false);
				this->setToSetupPose();
			}, 0.2f, "bossinjured");
		}
		else {
			this->playSoundDie();
			this->isNodie = true;
			this->clearTracks();
			this->setAnimation(0, "injured-red", false);
			this->setToSetupPose();
			this->changeState(new BossDie());
		}
		if (health <= 0) {
			spHp->setVisible(false);
			//isDie = true;
		}
		else {
			//float percent = spHp->getPercent();
			auto percent = spHp->getPercent();
			auto percent2 = ((float)this->health / (float)(this->health + 1))*percent;
			spHp->setPercent(percent2);

		}
	}
}

void EnemyBoss1::createPool()
{
	slashPool = CCArray::createWithCapacity(3);
	slashPool->retain();
	for (int i = 0; i < 3; i++) {
		auto scale = SCREEN_SIZE.height / 5 / (367 / 2);
		auto slash = SlashBoss::create("Animation/Enemy_Boss1/skill-boss.json", "Animation/Enemy_Boss1/skill-boss.atlas", scale);
		slash->setVisible(false);
		slash->setPosition(this->getPosition());
		//slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		this->getParent()->addChild(slash, ZORDER_ENEMY);
		auto tmpbody = slash->getB2Body();
		tmpbody = nullptr;
		slashPool->addObject(slash);
	}

	indexSlash = 0;
}

void EnemyBoss1::creatSlash(float angel)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setIsDie(false);
	slash->setVisible(true);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world, this->getBoneLocation("bone65"));
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngle(angel);
	slash->setRotation(-angel / PI * 180 + 180);
	slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss1::creatHidenSlash(float angle)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setVisible(false);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world, this->getPosGenSlash());
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngle(angle);
	//slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss1::creatHpSprite()
{
	auto bloodbg = Sprite::create("UI/blood_boss_board.png");

	spHp = ui::LoadingBar::create("UI/blood_boss.png");
	auto tmp = SCREEN_SIZE.width / 6 / spHp->getContentSize().width;
	spHp->setScale(SCREEN_SIZE.width / 10 / spHp->getContentSize().width);
	//spHp->setScaleY(SCREEN_SIZE.height / 100 / spHp->getContentSize().height);
	spHp->setPosition(Vec2(0, 0));
	spHp->setPercent(100);
	bloodbg->setScale(spHp->getScaleX());
	//bloodbg->setScaleY(spHp->getScaleY());
	bloodbg->setPosition(0, 0);
	this->addChild(bloodbg);
	this->addChild(spHp);
	//spHp->update(0.0f);
}

void EnemyBoss1::boomboom()
{
	exxp = SkeletonAnimation::createWithFile("Effect/exxp.json", "Effect/exxp.atlas", scaleBoss);
	//exxp = Sprite::create("Effect/exxp.png");
	exxp->setScale(scaleBoss * 5);
	exxp->setPosition(this->getPosition());
	exxp->setAnimation(0, "exxp", true);
	exxp->setToSetupPose();
	exxp->update(0.0f);
	this->getParent()->addChild(exxp, 100);
	/*this->createGold();
	this->setRealMoveVelocity(Vec2(0, -this->getmoveVelocity().y/3));*/
	//auto vecy = (this->getPositionY() - heroLocation.y)/4;
	auto callBack = CCCallFunc::create([&]() {
		createGold();

		this->setRealMoveVelocity(Vec2(0, (this->getPositionY() - heroLocation.y) / 2));
	});

	auto callBack2 = CCCallFunc::create([&]() {
		createGold();

		this->setRealMoveVelocity(Vec2(0, -this->getmoveVelocity().y));
	});
	this->runAction(Sequence::create(DelayTime::create(1), callBack, DelayTime::create(1), callBack,
		DelayTime::create(1), callBack, DelayTime::create(1), callBack, callBack2, nullptr));
	/*this->schedule([&](float dt){
		this->setRealMoveVelocity(Vec2(0, (this->getPositionY() - heroLocation.y) / 2));
		if (createGold() > this->getCoinPool()->count()) {
			this->setRealMoveVelocity(Vec2(0, -this->getmoveVelocity().y));
			this->unschedule("bossboom");
		};

	}, 1, "bossboom");*/
}
//22/4 thuandv edited
int EnemyBoss1::createGold()
{
	int tmp = int(indexCoin + levelBoss * 24);
	if (tmp < coinPool->count()) {
		if (tmp > coinPool->count()) tmp = coinPool->count();
		for (; indexCoin < tmp; indexCoin++) {
			auto coin = (Coin*)coinPool->getObjectAtIndex(indexCoin);
			coin->setVisible(true);
			coin->setPosition(this->getPosition());
			this->getParent()->addChild(coin, ZORDER_ENEMY);
			coin->initCirclePhysic(this->getB2Body()->GetWorld(), this->getPosition());
			coin->getB2Body()->GetFixtureList()->SetSensor(false);
			coin->getB2Body()->SetType(b2_dynamicBody);
			coin->changeBodyCategoryBits(BITMASK_COIN);
			coin->changeBodyMaskBits(BITMASK_FLOOR);
			coin->setAngle(CCRANDOM_0_1()*PI);
			coin->getB2Body()->SetFixedRotation(true);
			coin->runAnimation();
		}
	}
	return tmp;
}

void EnemyBoss1::createCoinPool()
{
	int count = int(levelBoss * 50);
	coinPool = CCArray::createWithCapacity(count);
	coinPool->retain();
	for (int i = 0; i < count; i++) {
		auto coin = Coin::create();
		auto scale = SCREEN_SIZE.height * 0.075 / coin->getContentSize().height;
		coin->setScale(scale);
		coin->setVisible(false);
		//coin->setPosition(0, 0);
		//slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		/*this->getParent()->addChild(coin, ZORDER_ENEMY);
		auto tmpbody = coin->getB2Body();
		tmpbody = nullptr;*/
		coinPool->addObject(coin);
	}

	indexCoin = 0;
}

void EnemyBoss1::updateMe(BaseHero* hero)
{
	if (hero->getB2Body() != nullptr) {
		this->heroLocation = Vec2(hero->getB2Body()->GetPosition().x*PTM_RATIO, hero->getB2Body()->GetPosition().y*PTM_RATIO);
	}
	//log("ParentBoss: %f, %f, %s", this->getParent()->getPositionX(), this->getParent()->getPositionY(), this->getParent()->getName().c_str());
	//log();
	auto posHero = hero->getPosition();
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}

	if (exxp != nullptr) {
		exxp->setPosition(this->getPosition());
	}

	
	state->execute(this);
	
	if (hero->getHealth() <= 0 || hero->getB2Body() == nullptr) {
		this->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
	}
	else {

		this->getB2Body()->SetLinearVelocity(b2Vec2(this->realtimeVec.x / PTM_RATIO, this->realtimeVec.y*cosf(control / 120.0f * 2 * PI) / PTM_RATIO) +
			b2Vec2(realMoveVelocity.x / PTM_RATIO, realMoveVelocity.y / PTM_RATIO));
	}


	//else
	//	this->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
	//////////////
	control++;
	if (control == maxControl) {
		control = 1;
	}



	for (int i = 0; i < slashPool->count(); i++) {
		auto slash = (SlashBoss*)slashPool->getObjectAtIndex(i);
		slash->updateMe(0.0f);
		if (slash->getB2Body() != nullptr && slash->getPositionX() < (posHero.x-SCREEN_SIZE.width/4)) {
			slash->die();
		}
	}
	for (int i = 0; i < coinPool->count(); i++) {
		auto coin = (Coin*)coinPool->getObjectAtIndex(i);
		coin->updateMe(hero);
	}

	if (this->getPosition().y < -SCREEN_SIZE.height / 2) {
		this->setIsDie(true);
	}


}

void EnemyBoss1::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (getCurrent()) {
			if ((strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1)) {
				this->idle();
				setIsNodie(false);
			}
			else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
				this->idle();
				setIsNodie(false);
			}
			else if ((strcmp(getCurrent()->animation->name, "injured") == 0 && loopCount == 1)) {

			}
			else if ((strcmp(getCurrent()->animation->name, "injured-red") == 0 && loopCount == 1)) {
				if (this->getHealth() > 0) {
					setIsNodie(false);
					this->idle();
				}

			}
		}
	});
}

bool EnemyBoss1::checkStop()
{
	if (this->getPositionX() - heroLocation.x > SCREEN_SIZE.width*0.65) {
		return true;
	}
	return false;
}

void EnemyBoss1::changeState(StateBoss * state)
{
	auto tmp = this->state;
	this->state = state;
	state->enter(this);
	delete tmp;
}

void EnemyBoss1::doAttack1()
{
	this->schedule([&](float dt) {
		//log("doattack1");
		this->setControlState(this->getControlState() + 1);
		if (this->getControlState() % 20 == 0) {
			if (this->getControlAttack() == 0) {
				this->changeState(new BossFixingStupid());
				//delete this;
				this->unschedule("bossattack1");
			}
			this->attack();
			this->setControlAttack(this->getControlAttack() - 1);
		}
	}, 0.1f, "bossattack1");
}

void EnemyBoss1::doAttack2()
{
	this->schedule([&](float dt) {
		//log("do attack2");
		this->setControlState(this->getControlState() + 1);
		if (this->getControlState() == 1) {
			this->attack2();
		}
		auto posHero = this->heroLocation;
		auto posBoss = this->getPosGenSlash();
		switch (this->getRandAt2())
		{
		case 0: {
			if (this->getControlState() == 1 || this->getControlState() == 3 || this->getControlState() == 5) {
				auto vecBossToHero = posHero - posBoss;
				this->creatSlash(vecBossToHero.getAngle());
			}
			break;
		}
		case 1: {
			if (this->getControlState() == 2 || this->getControlState() == 4) {
				auto vecBossToHero = posHero - posBoss;
				this->creatSlash(vecBossToHero.getAngle() - PI / 24);
				this->creatSlash(vecBossToHero.getAngle());
				this->creatSlash(vecBossToHero.getAngle() + PI / 24);
			}
			break;
		}
		case 2: {
			break;
		}
		default:
			if (this->getControlState() == 1 || this->getControlState() == 3 || this->getControlState() == 5) {
				auto vecBossToHero = posHero - posBoss;
				this->creatSlash(vecBossToHero.getAngle());
			}
			break;
		}
		//if (boss->getLevelBoss() == 1) {

		if (this->getControlState() >= 50) {
			this->changeState(new BossStupiding());
			this->unschedule("bossattack2");
		}
	}, 0.1f, "bossattack2");
}

Vec2 EnemyBoss1::getPosGenSlash()
{
	return this->getBoneLocation("bone65");
}

void EnemyBoss1::playSoundAttack1()
{
	AudioManager::playSound(SOUND_BOSS1CHEM);
}

void EnemyBoss1::playSoundAttack2()
{
	AudioManager::playSound(SOUND_BOSS1SKILL);
}

void EnemyBoss1::playSoundHit()
{
	AudioManager::playSound(SOUND_ENEMYHIT);
}

void EnemyBoss1::playSoundDie()
{
	AudioManager::playSound(SOUND_BOSS1DIE);
}

