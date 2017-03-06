#ifndef __COIN_H__
#define __COIN_H__

#include "B2Sprite.h"



class Coin : public B2Sprite
{

public:
	Coin();
	~Coin();
	Animate *animate;

	static Coin* create();
	virtual void initCirclePhysic(b2World *world, Point pos);
	void runAnimation();
	void picked();
	
};

#endif // __B2_SPRITE_H__