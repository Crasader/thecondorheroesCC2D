#ifndef __ENEMY_TOANCHAN_STUDENT2_H__
#define __ENEMY_TOANCHAN_STUDENT2_H__

#include "BaseEnemy.h"

USING_NS_CC;

using namespace spine;

class EnemyToanChanStudent : public BaseEnemy
{
private:
	CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	CC_SYNTHESIZE(Sprite*, splash, Splash);
public:
	EnemyToanChanStudent(string jsonFile, string atlasFile, float scale);
	static EnemyToanChanStudent* create(string jsonFile, string atlasFile, float scale);

	void run();
	void attack();
	void die();
	//void updateMe(float dt);
	void genSplash();
	void listener();
};

#endif // __ENEMY_H__
