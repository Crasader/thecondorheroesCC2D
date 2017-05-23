#ifndef __ENEMY_LINH_CAM_GIAO_H__
#define __ENEMY_LINH_CAM_GIAO_H__

#include "EnemyToanChanStudent.h"


class EnemyLinhCamGiao : public EnemyToanChanStudent
{
public:
	EnemyLinhCamGiao(spSkeletonData*data);
	EnemyLinhCamGiao(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao* create(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao* create(string filename, float scale);

	void run();
	virtual void updateMe(BaseHero* hero);
	void initBoxPhysic(b2World * world, Point pos);
	virtual void playsoundAt();
	virtual void playsoundDie();
	virtual void addSquareFixture(Size size, Vec2 pos, unsigned int category, unsigned int mask);
};

#endif // __ENEMY_H__
