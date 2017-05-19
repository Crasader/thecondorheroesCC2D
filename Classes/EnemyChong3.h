#ifndef __ENEMY_CHONG3_H__
#define __ENEMY_CHONG3_H__

#include "EnemyChong2.h"
#include "manager/SkeletonManager.h"

USING_NS_CC;

using namespace spine;

class EnemyChong3 : public EnemyChong2
{
public:
	EnemyChong3();
	~EnemyChong3();
	EnemyChong3(spSkeletonData *data);
	EnemyChong3(string jsonFile, string atlasFile, float scale);
	static EnemyChong3* create(string jsonFile, string atlasFile, float scale);
	static EnemyChong3* create(string filename, float scale);
};

#endif // __ENEMY_TNB_H__
