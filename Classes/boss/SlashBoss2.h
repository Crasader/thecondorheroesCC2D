#ifndef __SLASH_BOSS2_H__
#define __SLASH_BOSS2_H__

#include "B2Sprite.h"



class SlashBoss2 : public B2Sprite
{

public:
	SlashBoss2();
	~SlashBoss2();

	static SlashBoss2* create(string filename);
	//static SlashBoss2* create(SpriteBatchNode * batchNode);
	void initCirclePhysic(b2World *world, Point pos);
	void updateMe(float dt);
	void die();
	void setAngle(float radian);
	
};

#endif // __B2_SPRITE_H__