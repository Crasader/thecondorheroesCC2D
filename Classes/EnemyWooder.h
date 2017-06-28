#ifndef __ENEMY_WOODER_H__
#define __ENEMY_WOODER_H__

#include "BaseEnemy.h"
#include "manager/SkeletonManager.h"


USING_NS_CC;

using namespace spine;

class EnemyWooder : public BaseEnemy
{
public:
	EnemyWooder();
	~EnemyWooder();
	EnemyWooder(spSkeletonData *data);
	EnemyWooder(string jsonFile, string atlasFile, float scale);
	static EnemyWooder* create(string jsonFile, string atlasFile, float scale);
	static EnemyWooder* create(string filename,float scale);
	void run();
	void attack();
	void die();
	void updateMe(BaseHero* hero);
	void listener();
	void makeMask();
	void updatePos();
};

#endif // __ENEMY_H__
