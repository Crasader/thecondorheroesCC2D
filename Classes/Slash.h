#ifndef __SLASH_H__
#define __SLASH_H__

#include "B2Skeleton.h"



class Slash : public B2Skeleton
{
private:
	CC_SYNTHESIZE(bool, isDie, IsDie);
public:
	Slash(string jsonFile, string atlasFile, float scale);
	static Slash* create(string jsonFile, string atlasFile, float scale);
	void initCirclePhysic(b2World *world, Point pos);
	void updateMe(BaseHero* hero);
	//void runAnimation();
	//void die();
	
};

#endif // __B2_SPRITE_H__