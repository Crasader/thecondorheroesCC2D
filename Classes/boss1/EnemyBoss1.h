#ifndef __ENEMY_BOSS1_H__
#define __ENEMY_BOSS1_H__

#include "BaseEnemy.h"
#include "StateBoss1.h"
#include "SlashBoss.h"

class EnemyBoss1 : public BaseEnemy
{
private:
	CC_SYNTHESIZE(int, hp, HP);
	CC_SYNTHESIZE_READONLY(Vec2, baseVelocity, BaseVelocity);
	CC_SYNTHESIZE_READONLY(Vec2, moveVelocity, moveVelocity);
	CC_SYNTHESIZE(Vec2, realtimeVec, RealtimeVec);
	CC_SYNTHESIZE(Vec2, realMoveVelocity, RealMoveVelocity);
	CC_SYNTHESIZE(int, controlAttack, ControlAttack);
	const int maxControl = 960;
	int control;
public:
	Vec2 heroLocation;
	StateBoss1 *state;
	CCArray *slashPool;
	int indexSlash;
	EnemyBoss1(string jsonFile, string atlasFile, float scale);
	static EnemyBoss1* create(string jsonFile, string atlasFile, float scale);
	 void idle();
	 void attack();
	 void attack2();
	 void stupid();
	 void fixStupid();
	 void die();

	 void createPool();
	 void creatSlash();

	 void updateMe(Point posHero);

	 void listener();
	 bool checkStop();
	// void initCirclePhysic(b2World *world, Point pos);
};

#endif // __ENEMY_H__
