#ifndef __B2_SKELETON_H__
#define __B2_SKELETON_H__

#include "Global.h"
#include <spine/spine-cocos2dx.h>
#include "GB2ShapeCache-x.h"

using namespace spine;
class BaseHero;
class B2Skeleton : public SkeletonAnimation
{

public:
	//static Map<std::string, SkeletonRenderer *> skeleRendererCache;
	B2Skeleton();
	~B2Skeleton();
	B2Skeleton(string jsonFile, string atlasFile, float scale);
	B2Skeleton(spSkeletonData *data);
	static B2Skeleton* create(string jsonFile, string atlasFile, float scale);
	static B2Skeleton* create(spSkeletonData *data);
	virtual void initBoxPhysic(b2World *world, Point pos);
	virtual void initCirclePhysic(b2World *world, Point pos);
	virtual void initPhysicWithShapeCache(b2World * world, Point pos, string key);
	virtual void die();
	virtual void changeBodyCategoryBits(uint16 mask);
	virtual void changeBodyMaskBits(uint16 mask);

	// return Pos bone in parent of this skeleton
	virtual Point getBoneLocation(string boneName);
	virtual void updateMe(BaseHero *hero);
	virtual void onExit();

protected:
	CC_SYNTHESIZE(b2Body*, body, B2Body);

	CC_SYNTHESIZE(int, health, Health);
	CC_SYNTHESIZE(float, move_vel, MoveVel);


	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
};

#endif // __B2_SKELETON_H__
