#include "EnemyBoss1.h"
#include "StateBoss.h"

StateBoss::StateBoss()
{
}

StateBoss::~StateBoss()
{
}

void StateBoss::enter(EnemyBoss1 * boss)
{
	boss->setControlAttack(0);
	boss->setControlState(0);
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setControlState(0);
}


BossIdling::BossIdling()
{
}

BossIdling::~BossIdling()
{
}

void BossIdling::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	//log("idle");
}

void BossIdling::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() >= 180) {
		boss->changeState(new BossAttacking2());
		//delete this;
	}
}

BossAttacking1::BossAttacking1()
{
}

BossAttacking1::~BossAttacking1()
{
}

void BossAttacking1::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	srand(time(NULL));
	boss->setControlAttack(rand() % 3 + 1);
	//log("attack1");
}

void BossAttacking1::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() % 120 == 0) {
		if (boss->getControlAttack() == 0) {
			boss->changeState(new BossFixingStupid());
			//delete this;
		}
		boss->attack();
		boss->setControlAttack(boss->getControlAttack() - 1);
	}
}

BossAttacking2::BossAttacking2()
{
}

BossAttacking2::~BossAttacking2()
{
}

void BossAttacking2::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->setRandAt2(random() % (boss->getLevelBoss()));
	//log("attack2");
}

void BossAttacking2::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1); 
	if (boss->getControlState() == 1) {
		boss->attack2();
	}
	switch (boss->getRandAt2())
	{
	case 0: {
		if (boss->getControlState() == 16 || boss->getControlState() == 31 || boss->getControlState() == 46) {
			auto posHero = boss->heroLocation;
			auto posBoss = boss->getPosition();
			auto vecBossToHero = posHero - posBoss;
			boss->creatSlash(vecBossToHero.getAngle());
		}
		break;
	}
	case 1: {
		if (boss->getControlState() == 23 || boss->getControlState() == 46 ) {
			auto posHero = boss->heroLocation;
			auto posBoss = boss->getPosition();
			auto vecBossToHero = posHero - posBoss;
			boss->creatSlash(vecBossToHero.getAngle()-PI/24);
			boss->creatSlash(vecBossToHero.getAngle());
			boss->creatSlash(vecBossToHero.getAngle()+PI / 24);
		}
		break;
	}
	case 2: {
		break;
	}
	default:
		if (boss->getControlState() == 16 || boss->getControlState() == 31 || boss->getControlState() == 46) {
			auto posHero = boss->heroLocation;
			auto posBoss = boss->getPosition();
			auto vecBossToHero = posHero - posBoss;
			boss->creatSlash(vecBossToHero.getAngle());
		}
		break;
	}
	//if (boss->getLevelBoss() == 1) {
	
	if (boss->getControlState() >= 120) {
		boss->changeState(new BossStupiding());
		//delete this;
	}

}

BossStupiding::BossStupiding()
{
}

BossStupiding::~BossStupiding()
{
}

void BossStupiding::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->setRealMoveVelocity(-boss->getmoveVelocity());
	boss->setControlState(-1);
	//log("stupiding");
}

void BossStupiding::execute(EnemyBoss1 * boss)
{
	//log("stupiding ex");
	if (boss->getPositionY() < boss->heroLocation.y) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
	if (boss->getPositionX() < boss->heroLocation.x + boss->getBoundingBox().size.width / 3) {
		boss->setRealMoveVelocity(Vec2(0, -boss->getmoveVelocity().y));
	}
}

BossFixingStupid::BossFixingStupid()
{
}

BossFixingStupid::~BossFixingStupid()
{
}

void BossFixingStupid::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->setRealMoveVelocity(Vec2(boss->getmoveVelocity().x, boss->getmoveVelocity().y*CCRANDOM_0_1()));
	//log("fixstupid");
}

void BossFixingStupid::execute(EnemyBoss1 * boss)
{
	if (boss->checkStop()) {
		boss->changeState(new BossIdling());
	}

	if (boss->getPositionY() > SCREEN_SIZE.height *2.5f / 4) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
}

BossDie::BossDie()
{
}

BossDie::~BossDie()
{
}

void BossDie::enter(EnemyBoss1 * boss)
{
	//StateBoss1::enter(boss);
	boss->clearTracks();
	boss->setAnimation(0, "injured-red", false);
	boss->setRealMoveVelocity(Vec2(boss->getmoveVelocity().x, boss->getmoveVelocity().y));
}

void BossDie::execute(EnemyBoss1 * boss)
{
	if (boss->checkStop() && boss->getRealMoveVelocity().x != 0) {
		//boss->changeState(new Boss1Idling());
		boss->setRealMoveVelocity(Vec2(0, boss->getRealMoveVelocity().y));
		boss->boomboom();
	}

	if (boss->getPositionY() > SCREEN_SIZE.height *2.0f / 4 && boss->getRealMoveVelocity().y > 0) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}

}
