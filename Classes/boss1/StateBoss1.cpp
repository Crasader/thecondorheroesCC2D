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



Boss1Attacking1::Boss1Attacking1()
{
}

Boss1Attacking1::~Boss1Attacking1()
{
}

void Boss1Attacking1::idle(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealtimeVec(boss->getBaseVelocity());
	boss->state = new Boss1Idling();
	delete this;
}

void Boss1Attacking1::fixStupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealtimeVec(boss->getBaseVelocity()+Vec2(SCREEN_SIZE.width/6,0));
	boss->state = new Boss1FixingStupid();
	delete this;
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
	boss->setRealtimeVec(boss->getBaseVelocity());
	boss->state = new Boss1Idling();
	delete this;
}

Boss1Stupiding::Boss1Stupiding()
{
}

Boss1Stupiding::~Boss1Stupiding()
{
}

void Boss1Stupiding::fixStupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealtimeVec(boss->getBaseVelocity() + Vec2(SCREEN_SIZE.width / 6, 0));
	boss->state = new Boss1FixingStupid();
	delete this;
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
	boss->setRealtimeVec(boss->getBaseVelocity());
	boss->state = new Boss1Idling();
	delete this;
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
	boss->setRealtimeVec(boss->getBaseVelocity());
	boss->state = new Boss1Attacking2();
	delete this;
}

void Boss1Idling::stupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealtimeVec(boss->getBaseVelocity() + Vec2(SCREEN_SIZE.width/6, -SCREEN_SIZE.height/10));
	boss->state = new Boss1Stupiding();
	delete this;
}

void Boss1Idling::fixStupid(EnemyBoss1 * boss)
{
	boss->clearTracks();
	boss->setAnimation(0, "idle", true);
	boss->setToSetupPose();
	boss->setRealtimeVec(boss->getBaseVelocity() + Vec2(-SCREEN_SIZE.width / 6, SCREEN_SIZE.height / 10));
	boss->state = new Boss1FixingStupid();
	delete this;
}
