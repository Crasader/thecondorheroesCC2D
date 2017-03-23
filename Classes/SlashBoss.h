#ifndef __SLASH_BOSS_H__
#define __SLASH_BOSS_H__

#include "B2Skeleton.h"



class SlashBoss : public B2Skeleton
{
public:
	SlashBoss(string json, string atlas, float scale);
	~SlashBoss();
	//Animate *animate;

	static SlashBoss* create(string json, string atlas, float scale);
	virtual void initCirclePhysic(b2World *world, Point pos);
	//void runAnimation();
	void die();
	void setAngel(float radian);
};
	

#endif // __B2_SPRITE_H__