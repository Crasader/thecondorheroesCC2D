#ifndef __ENEMY_HOACDO2_H__
#define __ENEMY_HOACDO2_H__

#include "EnemyToanChanStudent2.h"
#include "Slash.h"

USING_NS_CC;

using namespace spine;

class EnemyHoacDo2 : public EnemyToanChanStudent2
{
private:
	//Vec2 posHero;
public:

	EnemyHoacDo2(string jsonFile, string atlasFile, float scale);
	EnemyHoacDo2(spSkeletonData *data);
	static EnemyHoacDo2* create(string jsonFile, string atlasFile, float scale);
	static EnemyHoacDo2* create(string filename,float scale);
	void genSlash();
	void attack(Vec2 posHero);
	void updateMe(BaseHero * hero);
	void playsoundAt();
	void playsoundDie();

	//void removeFromParentAndCleanup(bool onExit);
};

#endif // __ENEMY_H__
