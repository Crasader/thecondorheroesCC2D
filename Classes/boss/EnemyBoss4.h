#ifndef __ENEMY_BOSS4_H__
#define __ENEMY_BOSS4_H__

#include "EnemyBoss3.h"


class EnemyBoss4 : public EnemyBoss3
{
public:
	EnemyBoss4(string jsonFile, string atlasFile, float scale);
	static EnemyBoss4* create(string jsonFile, string atlasFile, float scale);
	void attack4();
	void die();
	void doDefend();
	void doAttack2();
	void listener();
};

#endif // __ENEMY_H__
