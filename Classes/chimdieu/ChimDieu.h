#pragma once
#ifndef __CHIM_DIEU_H__
#define __CHIM_DIEU_H__

#include "B2Skeleton.h"

USING_NS_CC;

using namespace spine;

class ChimDieu : public B2Skeleton {
private:
	CC_SYNTHESIZE(bool, isCarry, IsCarry);
	CC_SYNTHESIZE(bool, isUp, IsUp);
	CC_SYNTHESIZE(bool, isDown, IsDown);
	CC_SYNTHESIZE(float, sequenceCloud, SequenceCloud);
	CC_SYNTHESIZE(string, stringHero, StringHero);

public:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	ChimDieu(spSkeletonData *data);
	ChimDieu(string jsonFile, string atlasFile, float scale);
	static ChimDieu * create(string jsonFile, string atlasFile, float scale);
	static ChimDieu * create(spSkeletonData*data);

	void flyUp(b2Vec2 p_b2v2Velocity);
	void flyDown(b2Vec2 p_b2v2Velocity);
	void flyAway();
	virtual void updateMe(float dt);
	virtual void initCirclePhysic(b2World *world, Point pos);
};
#endif // __CHIM_DIEU_H__