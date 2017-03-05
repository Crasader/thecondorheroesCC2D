#ifndef __B2_SKELETON_H__
#define __B2_SKELETON_H__

#include "Global.h"
#include <spine/spine-cocos2dx.h>
#include <string>
#include "GB2ShapeCache-x.h"

using namespace std;
using namespace spine;

class B2Skeleton : public SkeletonAnimation
{

public:

	B2Skeleton(string jsonFile, string atlasFile, float scale);

	static B2Skeleton* create(string jsonFile, string atlasFile, float scale);

	virtual void initBoxPhysic(b2World *world, Point pos);
	virtual void initCirclePhysic(b2World *world, Point pos);
	virtual void initPhysicWithShapeCache(b2World * world, Point pos, string key);
	virtual void die();
	virtual void changeBodyCategoryBits(uint16 mask);
	virtual void changeBodyMaskBits(uint16 mask);

	virtual void update(float dt);

protected:
	CC_SYNTHESIZE(b2Body*, body, Body);

	CC_SYNTHESIZE(int, health, Health);
	CC_SYNTHESIZE(float, move_vel, MoveVel);
	CC_SYNTHESIZE(bool, facingRight, FacingRight);


	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
};

#endif // __B2_SKELETON_H__