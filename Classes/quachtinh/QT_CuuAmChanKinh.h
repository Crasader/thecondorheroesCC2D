#ifndef __QT_CUU_AM_CHAN_KINH_H__
#define __QT_CUU_AM_CHAN_KINH_H__

#include "B2Sprite.h"


class QT_CuuAmChanKinh : public B2Sprite
{

public:
	QT_CuuAmChanKinh();
	~QT_CuuAmChanKinh();

	static QT_CuuAmChanKinh* create();
	void initCirclePhysic(b2World *world, Point pos);

	void hitGround();
	void updateMe();

protected:
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	CC_SYNTHESIZE(float, angle, mAngle);
	
};

#define ChanKinh QT_CuuAmChanKinh

#endif // __QT_CUU_AM_CHAN_KINH_H__