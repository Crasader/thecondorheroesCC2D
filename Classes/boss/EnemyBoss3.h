#ifndef __ENEMY_BOSS3_H__
#define __ENEMY_BOSS3_H__

#include "EnemyBoss1.h"


class EnemyBoss3 : public EnemyBoss1
{
public:
	EnemyBoss3(string jsonFile, string atlasFile, float scale);
	static EnemyBoss3* create(string jsonFile, string atlasFile, float scale);

	void createPool();
	Vec2 getPosGenSlash();
	void creatSlash(float angel);
	void playSoundAttack1();
	void playSoundAttack2();
	void playSoundDie();
};

#endif // __ENEMY_H__
