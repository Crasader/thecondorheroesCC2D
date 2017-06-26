#include "EnemyBoss4.h"
#include "manager\AudioManager.h"
#include "layer\GameScene.h"
#include "BaseHero.h"

EnemyBoss4::EnemyBoss4(string jsonFile, string atlasFile, float scale) :EnemyBoss3(jsonFile, atlasFile, scale)
{
	health = 60;
}

EnemyBoss4 * EnemyBoss4::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss4* boss = new EnemyBoss4(jsonFile, atlasFile, scale);
	boss->setAnimation(0, "idle", true);
	boss->update(0.0f);
	boss->normalState = new BossIdling();
	boss->crazyState = nullptr;
	boss->setTag(TAG_BOSS);
	boss->scaleBoss = scale;
	boss->setLevelBoss(3);
	boss->autorelease();
	return boss;
}

void EnemyBoss4::attack4()
{
	this->immortal();
	this->clearTracks();
	this->setAnimation(0,"attack4",false);
	this->setToSetupPose();
	this->scheduleOnce([&](float dt) {
		auto hero = ((GameScene*)this->getParent())->getHero();
		hero->silence();
	}, 0.1f,"bossattack4");
}

void EnemyBoss4::die()
{
	float  magicNumber = CCRANDOM_0_1();
	
	if (!isDie && !isImmortal) {
		if (magicNumber < 0.6f) {
			health--;
			if (health > 0) {
				this->playSoundHit();
				this->immortal();
				this->clearTracks();
				this->setAnimation(0, "injured-red", false);
				this->setToSetupPose();
				/*this->scheduleOnce([&](float dt) {
					this->unImmortal();
					this->clearTracks();
					this->setAnimation(0, "idle", false);
					this->setToSetupPose();
				}, 0.3f, "bossinjured");*/
			}
			else {
				this->unschedule("bossinjured");
				this->playSoundDie();
				this->immortal();
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
		else {
			this->doDefend();
		}
	}
}

void EnemyBoss4::doDefend()
{
	this->addAnimation(1, "defend", false);
	//this->isNodie = true;
}

void EnemyBoss4::doAttack2()
{
	this->setControlState(0);
	this->unschedule("bossinjured");
	if (this->getPositionY() > SCREEN_SIZE.height / 5) {
		this->attack2();
		auto type1 = CallFunc::create([&]() {
			auto vecBossToHero = this->heroLocation -this->getPosGenSlash();
			this->creatSlash(vecBossToHero.getAngle());

		});

		auto type2 = CallFunc::create([&]() {
			auto vecBossToHero = this->heroLocation -this->getPosGenSlash();
			this->creatSlash(vecBossToHero.getAngle() - PI / 24);
			this->creatSlash(vecBossToHero.getAngle());
			this->creatSlash(vecBossToHero.getAngle() + PI / 24);
		});
		switch (this->getRandAt2())
		{
		case 0: {
			this->runAction(Sequence::create(type1, DelayTime::create(0.3f), type1, DelayTime::create(0.3f), type1, nullptr));
			break;
		}
		case 1: {
			this->runAction(type2);
			break;
		}
		default:
			this->runAction(type2);
			break;
		}
	}
	else {
		float magicnumber = CCRANDOM_0_1();
		
		if (magicnumber < 0.5f) {
			this->attack3();
			auto type3 = CallFunc::create([&]() {
				this->creatHidenSlash(PI);
			});
			this->runAction(Sequence::create(DelayTime::create(0.7f), type3, DelayTime::create(0.5f), type3, DelayTime::create(0.5f), type3, nullptr));
		}
		else {
			this->attack4();
			////log("at4");
		}
	}
}

void EnemyBoss4::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (getCurrent()) {
			if ((strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
				//this->unschedule("bossattack2");
			}
			else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
			}
			else if ((strcmp(getCurrent()->animation->name, "attack3") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
				//this->unschedule("bossattack2");
				this->setTimeScale(1);
			}
			else if ((strcmp(getCurrent()->animation->name, "attack4") == 0 && loopCount == 1)) {
				this->normalState->exit(this);
				//this->unschedule("bossattack2");
			}

			/*else if ((strcmp(getCurrent()->animation->name, "injured-red") == 0 && loopCount == 1)) {
				if (this->getHealth() > 0) {
					this->unImmortal();
					this->idle();
				}

			}*/
		}
	});
}

