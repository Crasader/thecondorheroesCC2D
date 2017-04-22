#ifndef __DARTS_H__
#define __DARTS_H__

#include "B2Skeleton.h"



class Darts : public B2Skeleton
{
private:
	CC_SYNTHESIZE(bool, isDie, IsDie);
public:
	Darts(string jsonFile, string atlasFile, float scale);
	static Darts* create(string jsonFile, string atlasFile, float scale);
	void initCirclePhysic(b2World *world, Point pos);
	void updateMe(BaseHero* hero);
	//void runAnimation();
	//void die();
	
};

#endif // __DARTS_H__