#include "EnemyBoss1.h"
#include "StateBoss1.h"

StateBoss1::StateBoss1()
{
}

StateBoss1::~StateBoss1()
{
}

void StateBoss1::idle(EnemyBoss1 * boss)
{
}

void StateBoss1::attack1(EnemyBoss1 * boss)
{
}

void StateBoss1::attack2(EnemyBoss1 * boss)
{
}

void StateBoss1::stupid(EnemyBoss1 * boss)
{
}

void StateBoss1::fixStupid(EnemyBoss1 * boss)
{
}

void StateBoss1::updateVec(EnemyBoss1 * boss)
{
}



Boss1Attacking1::Boss1Attacking1()
{
}

Boss1Attacking1::~Boss1Attacking1()
{
}

void Boss1Attacking1::attack1(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "attack", false);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Attacking1();
	delete this;
}

void Boss1Attacking1::idle(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	//boss->setRealtimeVec(boss->getBaseVelocity());
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Idling();
	delete this;
}

void Boss1Attacking1::fixStupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(boss->getmoveVelocity());
	boss->state = new Boss1FixingStupid();
	delete this;
}

void Boss1Attacking1::updateVec(EnemyBoss1 * boss)
{
	boss->setRealMoveVelocity(Vec2::ZERO);
}

Boss1Attacking2::Boss1Attacking2()
{
}

Boss1Attacking2::~Boss1Attacking2()
{
}

void Boss1Attacking2::idle(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Idling();
	delete this;
}

void Boss1Attacking2::updateVec(EnemyBoss1 * boss)
{
	boss->setRealMoveVelocity(Vec2::ZERO);
}

Boss1Stupiding::Boss1Stupiding()
{
}

Boss1Stupiding::~Boss1Stupiding()
{
}

void Boss1Stupiding::attack1(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "attack", false);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Attacking1();
	delete this;
}

void Boss1Stupiding::updateVec(EnemyBoss1 * boss)
{
	if (boss->getPositionY() < SCREEN_SIZE.height / 4) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x,0));
	}
}

Boss1FixingStupid::Boss1FixingStupid()
{
}

Boss1FixingStupid::~Boss1FixingStupid()
{
}

void Boss1FixingStupid::idle(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Idling();
	delete this;
}

void Boss1FixingStupid::updateVec(EnemyBoss1 * boss)
{
	if (boss->getPositionY() > SCREEN_SIZE.height *2.5f / 4) {
		boss->setRealMoveVelocity(Vec2(boss->getRealMoveVelocity().x,0));
	}

	if (boss->checkStop()) {
		boss->clearTracks();
		boss->setAnimation(0, "idle", true);
		boss->setToSetupPose();
		boss->setRealMoveVelocity(Vec2::ZERO);
		boss->state = new Boss1Idling();
		delete this;
	}
}

Boss1Idling::Boss1Idling()
{
}

Boss1Idling::~Boss1Idling()
{
}


void Boss1Idling::attack2(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "attack2",false);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(Vec2::ZERO);
	boss->state = new Boss1Attacking2();
	delete this;
}

void Boss1Idling::stupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealMoveVelocity(-boss->getmoveVelocity());
	boss->state = new Boss1Stupiding();
	delete this;
}

void Boss1Idling::fixStupid(EnemyBoss1 * boss)
{
	//boss->clearTracks();
	//boss->setAnimation(0, "idle", true);
	//boss->setToSetupPose();
	////boss->setRealtimeVec(boss->getBaseVelocity() + Vec2(-SCREEN_SIZE.width / 6, SCREEN_SIZE.height / 10));
	//boss->setRealMoveVelocity(boss->getmoveVelocity());
	//boss->state = new Boss1FixingStupid();
	//delete this;
}

void Boss1Idling::updateVec(EnemyBoss1 * boss)
{
	boss->setRealMoveVelocity(Vec2::ZERO);
}

