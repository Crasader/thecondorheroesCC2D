#ifndef __BASE_ENEMY_H__
#define __BASE_ENEMY_H__

#include "B2Skeleton.h"
//#include "Agent.h"

USING_NS_CC;

using namespace spine;

class BaseEnemy : public B2Skeleton//, public Agent
{
public:
	CC_SYNTHESIZE(bool, isDie, IsDie);
	CC_SYNTHESIZE(bool, isOccur, IsOccur);
	CC_SYNTHESIZE(bool, isEndOfScreen, IsEndOfScreen);
	CC_SYNTHESIZE(int, damage, Damage);
	CC_SYNTHESIZE(int, exp, Exp);

	BaseEnemy();
	~BaseEnemy();
	BaseEnemy(spSkeletonData*data);
	BaseEnemy(string jsonFile, string atlasFile, float scale);
	static BaseEnemy* create(string jsonFile, string atlasFile, float scale);
	static BaseEnemy* create(spSkeletonData*data);
	virtual void hit();
	virtual void run();
	virtual void attack();
	virtual void die();
	virtual void updateMe(BaseHero* hero);
	virtual void initCirclePhysic(b2World *world, Point pos);
	virtual void initBoxPhysic(b2World *world, Point pos);
	virtual void makeMask();
};

#endif // __ENEMY_H__
