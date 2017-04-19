#ifndef __ENEMY_HONGLANGBA_H__
#define __ENEMY_HONGLANGBA_H__

#include "BaseEnemy.h"

USING_NS_CC;

using namespace spine;

class EnemyHongLangBa : public BaseEnemy
{
private:
	//CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	//CC_SYNTHESIZE(Sprite*, splash, Splash);
public:
	EnemyHongLangBa(spSkeletonData*data);
	EnemyHongLangBa(string jsonFile, string atlasFile, float scale);
	static EnemyHongLangBa* create(string jsonFile, string atlasFile, float scale);
	static EnemyHongLangBa* create(string filename, float scale);

	void run();
	virtual void attack();
	virtual void die();
	virtual void initCirclePhysic(b2World * world, Point pos);
	void updateMe(BaseHero* hero);
	//virtual void genSplash();
	virtual void listener();
};

#endif // __ENEMY_HONGLANGBA_H__
