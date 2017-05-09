#ifndef __ENEMY_DATNHIBA2_H__
#define __ENEMY_DATNHIBA2_H__

#include "EnemyDatNhiBa.h"




class EnemyDatNhiBa2 : public EnemyDatNhiBa
{

public:
	EnemyDatNhiBa2(spSkeletonData*data);
	EnemyDatNhiBa2(string jsonFile, string atlasFile, float scale);
	static EnemyDatNhiBa2* create(string jsonFile, string atlasFile, float scale);
	static EnemyDatNhiBa2* create(string filename, float scale);
	void initCirclePhysic(b2World * world, Point pos);
};

#endif // __ENEMY_H__
