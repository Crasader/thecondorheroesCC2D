#ifndef __ENEMY_TOANCHAN_STUDENT2_H__
#define __ENEMY_TOANCHAN_STUDENT2_H__

#include "EnemyToanChanStudent.h"
#include "Slash.h"

USING_NS_CC;

using namespace spine;

class EnemyToanChanStudent2 : public EnemyToanChanStudent
{
private:
	
public:
	CC_SYNTHESIZE(float, scaleEnemy, ScaleEnemy);
	CC_SYNTHESIZE(Slash*, slash, Slash);
	CC_SYNTHESIZE(int, controlAttack, ControlAttack);
	EnemyToanChanStudent2(string jsonFile, string atlasFile, float scale);
	static EnemyToanChanStudent2* create(string jsonFile, string atlasFile, float scale);

	void attack();
	void die();
	void genSlash();
	void listener();
	void updateMe(float dt);
	void removeFromParentAndCleanup(bool cleanup);
	void onExit();
};

#endif // __ENEMY_H__
