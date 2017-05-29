#ifndef __ENEMY_LINH_TEN_THANG_H__
#define __ENEMY_LINH_TEN_THANG_H__

#include "EnemyHoacDo2.h"
#include "Slash.h"

USING_NS_CC;

using namespace spine;

class EnemyLinhTenThang : public EnemyHoacDo2
{
private:
	//Vec2 posHero;
public:

	EnemyLinhTenThang(spSkeletonData *data);
	static EnemyLinhTenThang* create(string filename,float scale);
	void genSlash();
	void attack(Vec2 posHero);
	virtual void updateMe(BaseHero *hero);
	void playsoundAt();
	void playsoundDie();

	//void removeFromParentAndCleanup(bool onExit);
};

#endif // __ENEMY_H__
