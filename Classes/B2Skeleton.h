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
private:
	b2Body *body;
public:
	//	Sprite *boom;


	B2Skeleton(string jsonFile, string atlasFile, float scale);

	static B2Skeleton* create(string jsonFile, string atlasFile, float scale);

	// SCREEN_SIZE
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();


	//b2FixtureDef fixtureDef;

	//int health;
	//float move_vel;
	//bool facingRight;

	b2Body* getB2Body();

	virtual void initBoxPhysic(b2World *world, Point pos);
	virtual void initCirclePhysic(b2World *world, Point pos);
	virtual void initPhysicWithShapeCache(b2World *world, Point pos, string key);
	virtual void changeBodyCategoryBits(uint16 mask);
	virtual void changeBodyMaskBits(uint16 mask);
	//virtual void update();
	virtual void update(float dt);
};

#endif // __B2_SKELETON_H__