#ifndef __ENEMY_TOANCHAN_STUDENT_H__
#define __ENEMY_TOANCHAN_STUDENT_H__

#include "BaseEnemy.h"

USING_NS_CC;

using namespace spine;

class EnemyToanChanStudent : public BaseEnemy
{
private:
	//CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	//CC_SYNTHESIZE(Sprite*, splash, Splash);
public:
	EnemyToanChanStudent(spSkeletonData*data);
	EnemyToanChanStudent(string jsonFile, string atlasFile, float scale);
	static EnemyToanChanStudent* create(string jsonFile, string atlasFile, float scale);
	static EnemyToanChanStudent* create(spSkeletonData*data);
	void run();
	virtual void attack();
	virtual void die();
	//void updateMe(float dt);
	//virtual void genSplash();
	virtual void listener();
};

#endif // __ENEMY_H__
