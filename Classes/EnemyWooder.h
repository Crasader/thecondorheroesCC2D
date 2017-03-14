#ifndef __ENEMY_WOODER_H__
#define __ENEMY_WOODER_H__

#include "BaseEnemy.h"

USING_NS_CC;

using namespace spine;

class EnemyWooder : public BaseEnemy
{
public:
	EnemyWooder(spSkeletonData *data);
	EnemyWooder(string jsonFile, string atlasFile, float scale);
	static EnemyWooder* create(string jsonFile, string atlasFile, float scale);
	static EnemyWooder* create(spSkeletonData*data);
	void run();
	void attack();
	void die();
	void updateMe(float dt);
	void listener();

};

#endif // __ENEMY_H__
