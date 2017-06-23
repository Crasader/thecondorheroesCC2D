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
	EnemyToanChanStudent2(spSkeletonData *data);
	static EnemyToanChanStudent2* create(string jsonFile, string atlasFile, float scale);
	static EnemyToanChanStudent2* create(string filename,float scale);

	virtual void attack();
	virtual void die();
	virtual void genSlash();
	virtual void listener();
	virtual void updateMe(BaseHero* hero);
	virtual void initCirclePhysic(b2World * world, Point pos);
	virtual void playsoundAt();
	virtual void playsoundDie();
	//void removeFromParentAndCleanup(bool onExit);
	virtual void updatePos();
	virtual void onExit();
	virtual void prepare();
};

#endif // __ENEMY_H__
