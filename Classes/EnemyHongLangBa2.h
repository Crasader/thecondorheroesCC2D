#ifndef __ENEMY_HONGLANGBA2_H__
#define __ENEMY_HONGLANGBA2_H__

#include "EnemyHongLangBa.h"
#include "Darts.h"

USING_NS_CC;

using namespace spine;

class EnemyHongLangBa2 : public EnemyHongLangBa
{
private:
	
public:
	CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	CC_SYNTHESIZE(Darts*, enemyDarts, EnemyDarts);
	CC_SYNTHESIZE(int, controlAttack, ControlAttack);

	EnemyHongLangBa2(string jsonFile, string atlasFile, float scale);
	EnemyHongLangBa2(spSkeletonData *data);
	static EnemyHongLangBa2* create(string jsonFile, string atlasFile, float scale);
	static EnemyHongLangBa2* create(string filename,float scale);

	void attack();
	void die();
	void genDarts();
	void listener();
	void updateMe(BaseHero* hero);
	void initCirclePhysic(b2World * world, Point pos);

	//void removeFromParentAndCleanup(bool onExit);

	void onExit();
};

#endif // __ENEMY_HONGLANGBA2_H__
