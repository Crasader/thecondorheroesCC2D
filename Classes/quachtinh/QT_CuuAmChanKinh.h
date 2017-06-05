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
	void updateMe();
	void runAni();
	void explosion();

protected:
	CC_SYNTHESIZE(bool, isAdded, IsAdded);
	CC_SYNTHESIZE(bool, isCollide, IsCollide);
	CC_SYNTHESIZE(float, angle, mAngle);
	CC_SYNTHESIZE(ParticleSystemQuad*, particle, Particle);
	
};

#define ChanKinh QT_CuuAmChanKinh

#endif // __QT_CUU_AM_CHAN_KINH_H__