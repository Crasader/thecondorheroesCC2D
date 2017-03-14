#ifndef __BASE_ENEMY_H__
#define __BASE_ENEMY_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;

class BaseEnemy : public B2Skeleton
{
public:
	CC_SYNTHESIZE(bool, isDie, IsDie);
	BaseEnemy(spSkeletonData*data);
	BaseEnemy(string jsonFile, string atlasFile, float scale);
	static BaseEnemy* create(string jsonFile, string atlasFile, float scale);
	static BaseEnemy* create(spSkeletonData*data);
	virtual void run();
	virtual void attack();
	virtual void die();
	virtual void updateMe(float dt);
	virtual void initCirclePhysic(b2World *world, Point pos);
};

#endif // __ENEMY_H__
