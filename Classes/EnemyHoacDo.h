#ifndef __ENEMY_HOACDO_H__
#define __ENEMY_HOACDO_H__

#include "EnemyToanChanStudent.h"




class EnemyHoacDo : public EnemyToanChanStudent
{
private:
	//CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	//CC_SYNTHESIZE(Sprite*, splash, Splash);
public:
	EnemyHoacDo(spSkeletonData*data);
	EnemyHoacDo(string jsonFile, string atlasFile, float scale);
	static EnemyHoacDo* create(string jsonFile, string atlasFile, float scale);
	static EnemyHoacDo* create(string filename, float scale);
	virtual void playsoundAt();
	virtual void playsoundDie();
	virtual void initCirclePhysic(b2World * world, Point pos);
};

#endif // __ENEMY_H__
