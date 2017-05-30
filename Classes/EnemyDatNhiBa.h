#ifndef __ENEMY_DATNHIBA_H__
#define __ENEMY_DATNHIBA_H__

#include "EnemyToanChanStudent.h"




class EnemyDatNhiBa : public EnemyToanChanStudent
{
private:
	CC_SYNTHESIZE(bool, canRun, CanRun);
public:
	EnemyDatNhiBa(spSkeletonData*data);
	EnemyDatNhiBa(string jsonFile, string atlasFile, float scale);
	static EnemyDatNhiBa* create(string jsonFile, string atlasFile, float scale);
	static EnemyDatNhiBa* create(string filename, float scale);
	void hit();
	void updateMe(BaseHero* hero);
	void run();
	void playsoundAt();
	void playsoundDie();
	virtual void initCirclePhysic(b2World * world, Point pos);
	virtual void updatePos();
};

#endif // __ENEMY_H__
