#ifndef __DQ_TIEU_HON_CHUONG_H__
#define __DQ_TIEU_HON_CHUONG_H__

#include "B2Sprite.h"


class DQ_TieuHonChuong : public B2Sprite
{

public:
	DQ_TieuHonChuong();
	~DQ_TieuHonChuong();

	static DQ_TieuHonChuong* create(string frameName);
	virtual void initCirclePhysic(b2World *world, Point pos);
	
	void updateMe();

protected:
	CC_SYNTHESIZE(bool, isCollide, IsCollide);
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	CC_SYNTHESIZE(b2Vec2, vel, Vel);
};

#define TieuHonChuong DQ_TieuHonChuong

#endif // __DQ_TIEU_HON_CHUONG_H__