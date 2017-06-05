#ifndef __ENEMY_LINH_CAM_GIAO2_H__
#define __ENEMY_LINH_CAM_GIAO2_H__

#include "EnemyLinhCamGiao.h"


class EnemyLinhCamGiao2 : public EnemyLinhCamGiao
{
public:
	EnemyLinhCamGiao2(spSkeletonData*data);
	EnemyLinhCamGiao2(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao2* create(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao2* create(string filename, float scale);
	void updateMe(BaseHero * hero);
	void run();
	void initBoxPhysic(b2World * world, Point pos);
	virtual void updatePos();
	void makeMask();
	virtual void addSquareFixture(Size size, Vec2 pos, unsigned int category, unsigned int mask);
	virtual void listener();
};

#endif // __ENEMY_H__
