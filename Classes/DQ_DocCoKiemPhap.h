#ifndef __DQ_DOC_CO_KIEM_PHAP_H__
#define __DQ_DOC_CO_KIEM_PHAP_H__

#include "B2Sprite.h"


class DQ_DocCoKiemPhap : public B2Sprite
{

public:
	DQ_DocCoKiemPhap();
	~DQ_DocCoKiemPhap();

	CC_SYNTHESIZE(Sprite*, hitGroundEffect, HitGround);
	CC_SYNTHESIZE(ParticleSystemQuad*, particle, Particle);

	static DQ_DocCoKiemPhap* create(string file);
	virtual void initBoxPhysic(b2World *world, Point pos);


	void hitGround();
	void updateMe(float dt);

protected:
	CC_SYNTHESIZE(bool, isCollide, IsCollide);
	
};

#define KiemPhap DQ_DocCoKiemPhap

#endif // __DQ_DOC_CO_KIEM_PHAP_H__