#ifndef __ENEMY_LINH_CAM_ROI_H__
#define __ENEMY_LINH_CAM_ROI_H__

#include "EnemyLinhCamGiao.h"


class EnemyLinhCamRoi : public EnemyLinhCamGiao
{
public:
	EnemyLinhCamRoi(spSkeletonData*data);
	static EnemyLinhCamRoi* create(string filename, float scale);

	virtual void playsoundAt();
	virtual void playsoundDie();
	
};

#endif // __ENEMY_H__
