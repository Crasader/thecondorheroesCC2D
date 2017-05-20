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
	virtual void playsoundAt();
	virtual void playsoundDie();
};

#endif // __ENEMY_H__
