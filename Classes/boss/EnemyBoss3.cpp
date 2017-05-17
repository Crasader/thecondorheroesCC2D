#include "EnemyBoss3.h"
#include "manager\AudioManager.h"

EnemyBoss3::EnemyBoss3(string jsonFile, string atlasFile, float scale) :EnemyBoss1(jsonFile, atlasFile, scale)
{
	health = 30;
}

EnemyBoss3 * EnemyBoss3::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss3* boss = new EnemyBoss3(jsonFile, atlasFile, scale);
	boss->setAnimation(0, "idle", true);
	boss->update(0.0f);
	boss->state = new BossIdling();
	boss->setTag(TAG_BOSS);
	boss->scaleBoss = scale;
	boss->setLevelBoss(3);
	return boss;
}

void EnemyBoss3::createPool()
{
	slashPool = CCArray::createWithCapacity(3);
	slashPool->retain();
	for (int i = 0; i < 3; i++) {
		auto scale = SCREEN_SIZE.height / 20 / 96;
		auto slash = SlashBoss::create("Animation/Enemy_Boss3/skill-boss-3.json", "Animation/Enemy_Boss3/skill-boss-3.atlas", scale);
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

void EnemyBoss3::attack3()
{
	AudioManager::playSound(SOUND_BOSS3SKILL2);
	this->isNodie = true;
	this->clearTracks();
	this->setAnimation(0, "attack3", false);
	this->setTimeScale(0.4f);
}

void EnemyBoss3::fixStupid()
{
	int randnumber = RandomHelper::random_int(0,1);
	if (randnumber) {
		EnemyBoss1::fixStupid();
	}
		
	else {
		this->setRealMoveVelocity(Vec2(this->getmoveVelocity().x, this->getmoveVelocity().y*CCRANDOM_0_1()*-1));
	}
}

Vec2 EnemyBoss3::getPosGenSlash()
{
	return this->getBoneLocation("vukhi2");
}

void EnemyBoss3::creatSlash(float angel)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
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
	slash->initCirclePhysic(world, this->getBoneLocation("vukhi2"));
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngle(angel);
	slash->setRotation(-angel / PI * 180 + 180);
	slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss3::playSoundAttack1()
{
	AudioManager::playSound(SOUND_BOSS3CHEM);
}

void EnemyBoss3::playSoundAttack2()
{
	AudioManager::playSound(SOUND_BOSS3SKILL1);
}


void EnemyBoss3::playSoundDie()
{
	AudioManager::playSound(SOUND_BOSS3DIE);
}

void EnemyBoss3::doAttack2()
{
	if (this->getPositionY() > SCREEN_SIZE.height / 5) {
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


			if (this->getControlState() >= 50) {
				this->changeState(new BossStupiding());
				this->unschedule("bossattack2");
			}

		}, 0.1f, "bossattack2");
	}
	else {
		this->schedule([&](float dt) {
			//log("do attack2");
			this->setControlState(this->getControlState() + 1);
			if (this->getControlState() == 1) {
				this->attack3();


			}
			if (this->getControlState() == 7 ||this->getControlState() == 12 || this->getControlState() == 17) {
				this->creatHidenSlash(PI);
			}
			//if (boss->getLevelBoss() == 1) {
			if (this->getControlState() == 25) {
				this->clearTracks();
				this->setAnimation(0, "idle", true);
				this->setTimeScale(1);
				this->setToSetupPose();
			}

			if (this->getControlState() >= 50) {
				this->changeState(new BossStupiding());
				this->unschedule("bossattack2");
			}

		}, 0.1f, "bossattack2");
	}

}
