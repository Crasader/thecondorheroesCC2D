#include "EnemyBoss1.h"
#include "StateBoss1.h"

StateBoss1::StateBoss1()
{
}

StateBoss1::~StateBoss1()
{
}

void StateBoss1::enter(EnemyBoss1 * boss)
{
	boss->setControlAttack(0);
	boss->setControlState(0);
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setControlState(0);
}


Boss1Idling::Boss1Idling()
{
}

Boss1Idling::~Boss1Idling()
{
}

void Boss1Idling::enter(EnemyBoss1 * boss)
{
	StateBoss1::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	//log("idle");
}

void Boss1Idling::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState()>= 180) {
		boss->changeState(new Boss1Attacking2());
		//delete this;
	}
}

Boss1Attacking1::Boss1Attacking1()
{
}

Boss1Attacking1::~Boss1Attacking1()
{
}

void Boss1Attacking1::enter(EnemyBoss1 * boss)
{
	StateBoss1::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	srand(time(NULL));
	boss->setControlAttack(rand() % 3 + 1);
	//log("attack1");
}

void Boss1Attacking1::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() % 120 == 0) {
		if (boss->getControlAttack() == 0) {
			boss->changeState(new Boss1FixingStupid());
			//delete this;
		}
		boss->attack();
		boss->setControlAttack(boss->getControlAttack() - 1);
	}
}

Boss1Attacking2::Boss1Attacking2()
{
}

Boss1Attacking2::~Boss1Attacking2()
{
}

void Boss1Attacking2::enter(EnemyBoss1 * boss)
{
	StateBoss1::enter(boss);
	boss->setRealMoveVelocity(Vec2::ZERO);
	//log("attack2");
}

void Boss1Attacking2::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() == 1) {
		boss->attack2();
	}
	if (boss->getControlState() == 16 || boss->getControlState() == 31 || boss->getControlState() == 46) {
		auto posHero = boss->heroLocation;
		auto posBoss = boss->getPosition();
		auto vecBossToHero = posHero - posBoss;
		boss->creatSlash(vecBossToHero.getAngle());
	}
	if (boss->getControlState() >= 120) {
		boss->changeState(new Boss1Stupiding());
		//delete this;
	}
}

Boss1Stupiding::Boss1Stupiding()
{
}

Boss1Stupiding::~Boss1Stupiding()
{
}

void Boss1Stupiding::enter(EnemyBoss1 * boss)
{
	StateBoss1::enter(boss);
	boss->setRealMoveVelocity(-boss->getmoveVelocity());
	boss->setControlState(-1);
	//log("stupiding");
}

void Boss1Stupiding::execute(EnemyBoss1 * boss)
{
	//log("stupiding ex");
	if (boss->getPositionY() < boss->heroLocation.y) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
	if (boss->getPositionX() < boss->heroLocation.x+boss->getBoundingBox().size.width/3) {
		boss->setRealMoveVelocity(Vec2(0, boss->getRealMoveVelocity().y));
	}
}

Boss1FixingStupid::Boss1FixingStupid()
{
}

Boss1FixingStupid::~Boss1FixingStupid()
{
}

void Boss1FixingStupid::enter(EnemyBoss1 * boss)
{
	StateBoss1::enter(boss);
	boss->setRealMoveVelocity(Vec2(boss->getmoveVelocity().x, boss->getmoveVelocity().y*CCRANDOM_0_1()));
	//log("fixstupid");
}

void Boss1FixingStupid::execute(EnemyBoss1 * boss)
{
	if (boss->checkStop()) {
		boss->changeState(new Boss1Idling());
	}

	if (boss->getPositionY() > SCREEN_SIZE.height *2.5f / 4) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
}

Boss1Die::Boss1Die()
{
}

Boss1Die::~Boss1Die()
{
}

void Boss1Die::enter(EnemyBoss1 * boss)
{
	//StateBoss1::enter(boss);
	boss->clearTracks();
	boss->setAnimation(0,"injured-red",false);
}

void Boss1Die::execute(EnemyBoss1 * boss)
{
	if (boss->control % 30 == 0) {
		//boss->createGold();
	}
}
