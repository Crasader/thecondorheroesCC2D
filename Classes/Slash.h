#ifndef __SLASH_H__
#define __SLASH_H__

#include "B2Sprite.h"



class Slash : public B2Sprite
{

public:
	Slash();
	~Slash();
	//Animate *animate;

	static Slash* create(string file);
	virtual void initCirclePhysic(b2World *world, Point pos);
	//void runAnimation();
	void die();
	
};

#endif // __B2_SPRITE_H__