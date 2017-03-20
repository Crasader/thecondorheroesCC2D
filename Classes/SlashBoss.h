#ifndef __SLASH_BOSS_H__
#define __SLASH_BOSS_H__

#include "B2Sprite.h"



class SlashBoss : public B2Sprite
{
public:
	SlashBoss();
	~SlashBoss();
	//Animate *animate;

	static SlashBoss* create(string file);
	virtual void initCirclePhysic(b2World *world, Point pos);
	//void runAnimation();
	void die();
	void setAngel(float radian);
};
	

#endif // __B2_SPRITE_H__