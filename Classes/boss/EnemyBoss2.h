#ifndef __ENEMY_BOSS2_H__
#define __ENEMY_BOSS2_H__

#include "EnemyBoss1.h"


class EnemyBoss2 : public EnemyBoss1
{
public:
	EnemyBoss2(string jsonFile, string atlasFile, float scale);
	static EnemyBoss2* create(string jsonFile, string atlasFile, float scale);

	void createPool();
	void creatSlash(float angel);
	void playSoundAttack1();
	void playSoundAttack2();
	void playSoundDie();
};

#endif // __ENEMY_H__
