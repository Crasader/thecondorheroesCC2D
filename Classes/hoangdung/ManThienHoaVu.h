#ifndef __MAN_THIEN_HOA_VU_H__
#define __MAN_THIEN_HOA_VU_H__

#include "B2Sprite.h"

class ManThienHoaVu : public B2Sprite
{

public:
	ManThienHoaVu();
	~ManThienHoaVu();

	static ManThienHoaVu* create(string frameName);
	virtual void initCirclePhysic(b2World *world, Point pos);
	
	void updateMe();

protected:
	CC_SYNTHESIZE(bool, isCollide, IsCollide);
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	CC_SYNTHESIZE(b2Vec2, vel, Vel);
};

#define InsManThienHoaVu ManThienHoaVu

#endif // __MAN_THIEN_HOA_VU_H__