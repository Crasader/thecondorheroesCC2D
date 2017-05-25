#ifndef __ENEMY_LINH_TEN_XIEN_H__
#define __ENEMY_LINH_TEN_XIEN_H__

#include "EnemyLinhTenThang.h"
#include "Slash.h"

USING_NS_CC;

using namespace spine;

class EnemyLinhTenXien : public EnemyLinhTenThang
{
private:
	//Vec2 posHero;
public:

	EnemyLinhTenXien(spSkeletonData *data);
	static EnemyLinhTenXien* create(string filename,float scale);
	void attack(Vec2 posHero);
	//void updateMe(BaseHero *hero);
	//void removeFromParentAndCleanup(bool onExit);
};

#endif // __ENEMY_H__
