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
	boss->state = new BossIdling();
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
	
	if (!isDie) {
		if (magicNumber < 0.6f) {
			health--;
			if (health > 0) {
				this->playSoundHit();
				this->immortal();
				this->clearTracks();
				this->setAnimation(0, "injured-red", false);
				this->setToSetupPose();
				this->scheduleOnce([&](float dt) {
					this->unImmortal();
					this->clearTracks();
					this->setAnimation(0, "idle", false);
					this->setToSetupPose();
				}, 0.2f, "bossinjured");
			}
			else {
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
	this->unschedule("bossinjured");
	if (this->getPositionY() > SCREEN_SIZE.height / 5) {
		this->schedule([&](float dt) {
			////log("do attack2");
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
				if (this->getControlState() == 3) {
					auto vecBossToHero = posHero - posBoss;
					this->creatSlash(vecBossToHero.getAngle() - PI / 24);
					this->creatSlash(vecBossToHero.getAngle());
					this->creatSlash(vecBossToHero.getAngle() + PI / 24);
				}
				break;
			}
			case 2: {
				if (this->getControlState() == 3) {
					auto vecBossToHero = posHero - posBoss;
					this->creatSlash(vecBossToHero.getAngle() - PI / 24);
					this->creatSlash(vecBossToHero.getAngle());
					this->creatSlash(vecBossToHero.getAngle() + PI / 24);
				}
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


			if (this->getControlState() >= 30) {
				this->changeState(new BossStupiding());
				this->unschedule("bossattack2");
			}

		}, 0.1f, "bossattack2");
	}
	else {
		float magicnumber = CCRANDOM_0_1();
		if (magicnumber < 0.5f) {
			this->schedule([&](float dt) {
				////log("do attack2");

				this->setControlState(this->getControlState() + 1);
				if (this->getControlState() == 1) {
					this->attack3();
					////log("at3");

				}
				if (this->getControlState() == 7 || this->getControlState() == 12 || this->getControlState() == 17) {
					this->creatHidenSlash(PI);
				}
				//if (boss->getLevelBoss() == 1) {
				if (this->getControlState() == 25) {
					this->clearTracks();
					this->setAnimation(0, "idle", true);
					this->setTimeScale(1);
					this->setToSetupPose();
					this->unImmortal();
				}

				if (this->getControlState() >= 30) {
					this->changeState(new BossStupiding());
					this->unschedule("bossattack2");
				}

			}, 0.1f, "bossattack2");
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
				this->idle();
				this->unImmortal();
			}
			else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
				this->idle();
				this->unImmortal();
			}
			else if ((strcmp(getCurrent()->animation->name, "attack3") == 0 && loopCount == 1)) {
				this->idle();
				this->unImmortal();
			}
			else if ((strcmp(getCurrent()->animation->name, "attack4") == 0 && loopCount == 1)) {
				this->idle();
				this->unImmortal();
				this->changeState(new BossStupiding());
				this->unschedule("bossattack2");
			}

			else if ((strcmp(getCurrent()->animation->name, "injured-red") == 0 && loopCount == 1)) {
				if (this->getHealth() > 0) {
					this->unImmortal();
					this->idle();
				}

			}
		}
	});
}

