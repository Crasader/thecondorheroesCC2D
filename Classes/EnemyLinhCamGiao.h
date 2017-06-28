#ifndef __ENEMY_LINH_CAM_GIAO_H__
#define __ENEMY_LINH_CAM_GIAO_H__

#include "EnemyHoacDo.h"

#define SOUND_GIAOAT		"sound/giaoat.mp3"
#define SOUND_GIAODIE		"sound/giaodie.mp3"
class EnemyLinhCamGiao : public EnemyHoacDo
{
public:
	EnemyLinhCamGiao(spSkeletonData*data);
	EnemyLinhCamGiao(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao* create(string jsonFile, string atlasFile, float scale);
	static EnemyLinhCamGiao* create(string filename, float scale);

	void run();
	virtual void updateMe(BaseHero* hero);
	virtual void initBoxPhysic(b2World * world, Point pos);
	virtual void playsoundAt();
	virtual void playsoundDie();
	virtual void addSquareFixture(Size size, Vec2 pos, unsigned int category, unsigned int mask);
	virtual void prepare();
	virtual void listener();
};

#endif // __ENEMY_H__
