#ifndef __ENEMY_CHONG2_H__
#define __ENEMY_CHONG2_H__

#include "EnemyChong1.h"
#include "SkeletonManager.h"

USING_NS_CC;

using namespace spine;

class EnemyChong2 : public EnemyChong1
{
public:
	EnemyChong2();
	~EnemyChong2();
	EnemyChong2(spSkeletonData *data);
	EnemyChong2(string jsonFile, string atlasFile, float scale);
	static EnemyChong2* create(string jsonFile, string atlasFile, float scale);
	static EnemyChong2* create(string filename,float scale);
	void die();
	void updateMe(BaseHero* hero);
	void listener();
};

#endif // __ENEMY_TNB_H__
