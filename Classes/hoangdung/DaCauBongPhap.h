#ifndef __DA_CAU_BONG_PHAP_H__
#define __DA_CAU_BONG_PHAP_H__

#include "B2Skeleton.h"

class DaCauBongPhap : public B2Skeleton
{
private:
	CC_SYNTHESIZE(bool, isDie, IsDie);
public:
	DaCauBongPhap(string jsonFile, string atlasFile, float scale);
	static DaCauBongPhap* create(string jsonFile, string atlasFile, float scale);
	void initCirclePhysic(b2World *world, Point pos);
	void updateMe();
protected:
	CC_SYNTHESIZE(bool, isCollide, IsCollide);
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	CC_SYNTHESIZE(b2Vec2, vel, Vel);
	
};

#endif // __DARTS_H__