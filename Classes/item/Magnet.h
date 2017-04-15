#ifndef __MAGNET_H__
#define __MAGNET_H__

#include "B2Sprite.h"

class Magnet : public B2Sprite
{

public:
	Magnet();
	~Magnet();
	b2World* world;

	static Magnet* create();
	virtual void initCirclePhysic(b2World *world, Point pos);
	void picked();
	void updateMe(BaseHero *hero);
	
};

#endif // __B2_SPRITE_H__