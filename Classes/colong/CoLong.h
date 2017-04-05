#pragma once
#ifndef __CO_LONG_H__
#define __CO_LONG_H__

#include "manager/EffectManager.h"
#include "manager/JSonHeroManager.h"
#include "BaseHero.h"
#include "Rada.h"
#include "duongqua/DQ_TieuHonChuong.h"
#include "layer/GameScene.h"

class CoLong : public BaseHero {
public:
	CoLong(string p_sJsonFile, string p_sAtlasFile, float p_fScale);
	static CoLong* create(string p_sJsonFile, string p_sAtlasFile, float p_fScale);

	void initCirclePhysic(b2World *world, Point pos);
	void addStuff();

	void idle();
	void run();
	void normalJump();
	void doubleJump();
	void landing();
	void die();
	void attackNormal();
	void attackLanding();
	void injured();
	void revive();
	void die(Point p_ptPositionOfCammera);
	void listener();

	void doDestroyBodies(b2World *world);

	void updateMe(float p_fDelta);


protected:
	Rada *m_pRadaSkill1;
	Rada *m_pRadaSkill2;
	Rada *m_pRadaSkill3;
	bool m_bEndSkill = false;
	void createRada(b2World *p_pWorld);

	// Skill 1
	list<TieuHonChuong *> m_lDocPhongCham;
	int checkDurationSkill1;
	void createDocPhongCham(Point p_ptStartPoint, Point p_ptEndPoint);
	void doCounterSkill1();

	// Skill 2
	int checkDurationSkill2;
	void createNgocNuKiemPhap(Point p_ptPoint);
	void doCounterSkill2();

	// Skill 3
	int checkDurationSkill3;
	void doCounterSkill3();

	//////////////////////////////////////////////////////////////////////////
	void createSlash();
	void runSlash();
	void runSlashLand();
	//////////////////////////////////////////////////////////////////////////
};
#endif // __CO_LONG_H__