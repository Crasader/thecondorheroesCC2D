#ifndef __ENEMY_TNB_H__
#define __ENEMY_TNB_H__

#include "BaseEnemy.h"
#include "SkeletonManager.h"

USING_NS_CC;

using namespace spine;

class EnemyTNB : public BaseEnemy
{
public:
	EnemyTNB();
	~EnemyTNB();
	EnemyTNB(spSkeletonData *data);
	EnemyTNB(string jsonFile, string atlasFile, float scale);
	static EnemyTNB* create(string jsonFile, string atlasFile, float scale);
	static EnemyTNB* create(string filename,float scale);
	void run();
	void attack();
	void die();
	void updateMe(BaseHero* hero);
	void listener();

};

#endif // __ENEMY_TNB_H__
