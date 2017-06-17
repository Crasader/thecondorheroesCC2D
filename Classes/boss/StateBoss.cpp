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
	if (boss->crazyState) {
		boss->crazyState->exit(boss);
		boss->crazyState = nullptr;
	}
	//boss->setControlState(0);
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
	boss->unImmortal();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->idle();
	log("idle");
}

void BossIdling::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() >= 60) {
		this->exit(boss);
		//delete this;
	}
}

void BossIdling::exit(EnemyBoss1 * boss)
{
	float a = CCRANDOM_0_1();
	if (a < 1.0f/boss->getBalanceAt1andAT2()) {
		boss->changeState(new BossAttacking2());
		boss->setBalanceAt1andAT2(boss->getBalanceAt1andAT2() + 2);
	}
	else {
		boss->changeState(new BossStupiding());
		if (boss->getBalanceAt1andAT2() - 2 > 1) {
			boss->setBalanceAt1andAT2(boss->getBalanceAt1andAT2() - 2);
		}
		else {
			boss->setBalanceAt1andAT2(1);
		}
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
	boss->doAttack1();
	boss->setRealMoveVelocity(Vec2::ZERO);
	srand(time(NULL));
	//boss->setControlAttack(rand() % 3 + 1);
	boss->setControlAttack(0);
	
}

void BossAttacking1::execute(EnemyBoss1 * boss)
{
	
}

void BossAttacking1::exit(EnemyBoss1 * boss)
{
	boss->changeState(new BossFixingStupid());
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
	boss->doAttack2();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->setRandAt2(cocos2d::random() % (boss->getLevelBoss()));
	log("attack2");
	
}

void BossAttacking2::execute(EnemyBoss1 * boss)
{
	

}

void BossAttacking2::exit(EnemyBoss1 * boss)
{
	boss->changeState(new BossIdling());
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
	boss->idle();
	boss->setRealMoveVelocity(-boss->getmoveVelocity());
	boss->setControlState(-1);
	log("stupiding");
}

void BossStupiding::execute(EnemyBoss1 * boss)
{
	////log("stupiding ex");
	if (boss->getPositionY() < SCREEN_SIZE.height/5) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
	else {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, -boss->getmoveVelocity().y));
	}
	if (boss->getPositionX() < boss->heroLocation.x + boss->getB2Body()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO) {
		boss->setRealMoveVelocity(Vec2(0, boss->getRealMoveVelocity().y));
	}
	else {
		boss->setRealMoveVelocity(Vec2(-boss->getmoveVelocity().x, boss->getRealMoveVelocity().y));
	}
}

void BossStupiding::exit(EnemyBoss1 * boss)
{
	boss->changeState(new BossAttacking1());
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
	boss->fixStupid();
}

void BossFixingStupid::execute(EnemyBoss1 * boss)
{
	if (boss->checkStop()) {
		this->exit(boss);
	}

	if (boss->getPositionY() > SCREEN_SIZE.height *2.5f / 4) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
	if (boss->getPositionY() < SCREEN_SIZE.height / 6 && boss->getRealMoveVelocity().y < 0) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x, 0));
	}
}

void BossFixingStupid::exit(EnemyBoss1 * boss)
{
	boss->changeState(new BossIdling());
}

BossDie::BossDie()
{
}

BossDie::~BossDie()
{
}

void BossDie::enter(EnemyBoss1 * boss)
{
	StateBoss::enter(boss);
	boss->clearTracks();
	boss->setAnimation(0, "injured-red", false);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2(boss->getmoveVelocity().x, boss->getmoveVelocity().y));
	/*boss->unschedule("bossattack1");
	boss->unschedule("bossattack2");*/
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
	boss->immortal();
}

void BossDie::exit(EnemyBoss1 * boss)
{
}

BossInjure::BossInjure()
{
}

BossInjure::~BossInjure()
{
}

void BossInjure::enter(EnemyBoss1 * boss)
{
	boss->immortal();
	boss->clearTracks();
	boss->setAnimation(0, "injured-red", false);
	boss->setToSetupPose();
	boss->setControlState(boss->getControlState() - boss->getControlState() % 20);
}

void BossInjure::execute(EnemyBoss1 * boss)
{
	boss->setControlState(boss->getControlState() + 1);
	if (boss->getControlState() % 20 == 0) {
		this->exit(boss);
	}
}

void BossInjure::exit(EnemyBoss1 * boss)
{
	boss->unImmortal();
	boss->crazyState = nullptr;
	boss->idle();
	delete this;
}
