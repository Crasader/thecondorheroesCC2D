#ifndef __ENEMY_HOACDO2_H__
#define __ENEMY_HOACDO2_H__

#include "EnemyToanChanStudent2.h"
#include "Slash.h"

USING_NS_CC;

using namespace spine;

class EnemyHoacDo2 : public EnemyToanChanStudent2
{
private:
	
public:

	EnemyHoacDo2(string jsonFile, string atlasFile, float scale);
	EnemyHoacDo2(spSkeletonData *data);
	static EnemyHoacDo2* create(string jsonFile, string atlasFile, float scale);
	static EnemyHoacDo2* create(string filename,float scale);
	void genSlash();
	void attack();

	void playsoundAt();
	void playsoundDie();

	//void removeFromParentAndCleanup(bool onExit);
};

#endif // __ENEMY_H__
