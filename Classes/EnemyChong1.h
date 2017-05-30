#ifndef __ENEMY_CHONG1_H__
#define __ENEMY_CHONG1_H__

#include "EnemyTNB.h"

USING_NS_CC;

using namespace spine;

class EnemyChong1 : public EnemyTNB
{
public:
	EnemyChong1();
	~EnemyChong1();
	EnemyChong1(spSkeletonData *data);
	EnemyChong1(string jsonFile, string atlasFile, float scale);
	static EnemyChong1* create(string jsonFile, string atlasFile, float scale);
	static EnemyChong1* create(string filename,float scale);
	void die();
	void updateMe(BaseHero* hero);
	void listener();
	void initBoxPhysic(b2World *world, Point pos);

};

#endif // __ENEMY_TNB_H__
