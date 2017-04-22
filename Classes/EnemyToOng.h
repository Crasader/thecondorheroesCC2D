#ifndef __ENEMY_TOONG_H__
#define __ENEMY_TOONG_H__

#include "BaseEnemy.h"

USING_NS_CC;

using namespace spine;

class EnemyToOng : public BaseEnemy
{
private:
	//CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	//CC_SYNTHESIZE(Sprite*, splash, Splash);
public:
	EnemyToOng(spSkeletonData*data);
	EnemyToOng(string jsonFile, string atlasFile, float scale);
	static EnemyToOng* create(string jsonFile, string atlasFile, float scale);
	static EnemyToOng* create(string filename, float scale);

	void run();
	virtual void attack();
	virtual void die();
	virtual void initCirclePhysic(b2World * world, Point pos);
	void updateMe(BaseHero* hero);
	//virtual void genSplash();
	virtual void listener();
};

#endif // __ENEMY_TOONG_H__
