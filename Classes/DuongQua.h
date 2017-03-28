#ifndef __DUONG_QUA_H__
#define __DUONG_QUA_H__

#include "EffectManager.h"
#include "DQ_TieuHonChuong.h"
#include "DQ_ToanChanKiemPhap.h"
#include "DQ_DocCoKiemPhap.h"
#include "BaseHero.h"

class DuongQua : public BaseHero
{
public:

	DuongQua(string jsonFile, string atlasFile, float scale);
	static DuongQua* create(string jsonFile, string atlasFile, float scale);

protected:

	// Skill 1
	list<ToanChanKiemPhap*> listToanChanKiemPhap;
	int checkDurationSkill1;
	void createToanChanKiemPhap(Point posSword);
	void slashToanChanKiemPhap();
	void doCounterSkill1();


	// Skill 2
	list<KiemPhap*> listKiemPhap;
	int checkDurationSkill2;
	int numberOfDeadSword;
	void createKiemPhap(float posX);
	void landKiemPhap();
	void doCounterSkill2();

	// Skill 3
	SkeletonAnimation* spiritHole;
	int checkDurationSkill3;
	int numberOfDeadTHC;
	list<TieuHonChuong*> listTieuHonChuong;
	void createSpiritHole();
	void runSpiritHole();
	Point getLocalSpiritBonePos(string boneName);
	void createTieuHonChuong(Point posHand, int Zoder);
	void shootTieuHonChuong();
	void doCounterSkill3();
	

	//////////////////////////////////////////////////////////////////////////
	void createSlash();
	void runSlash();
	void runSlashLand();
	//////////////////////////////////////////////////////////////////////////

	void initCirclePhysic(b2World *world, Point pos);

	void idle();
	void run();
	void normalJump();
	void doubleJump();
	void landing();
	void die();
	void attackNormal();
	void attackLanding();
	void attackBySkill1();
	void attackBySkill2();
	void attackBySkill3();
	void injured();
	void die(Point posOfCammera);
	void listener();

	void updateMe(float dt);

};
#endif // __DuongQua_H__