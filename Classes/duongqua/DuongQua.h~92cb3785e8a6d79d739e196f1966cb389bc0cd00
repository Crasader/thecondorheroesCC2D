#ifndef __DUONG_QUA_H__
#define __DUONG_QUA_H__

#include "duongqua/DQ_TieuHonChuong.h"
#include "duongqua/DQ_ToanChanKiemPhap.h"
#include "duongqua/DQ_DocCoKiemPhap.h"
#include "BaseHero.h"

class DuongQua : public BaseHero
{
public:

	DuongQua(string jsonFile, string atlasFile, float scale);
	static DuongQua* create(string jsonFile, string atlasFile, float scale);

protected:

	// Skill 1
	CCArray* poolSkill1;
	int indexSkill1 = 0;
	list<ToanChanKiemPhap*> listToanChanKiemPhap;
	int checkDurationSkill1;
	void createToanChanKiemPhap(Point posSword);
	void slashToanChanKiemPhap();
	void doCounterSkill1();


	// Skill 2
	list<KiemPhap*> listKiemPhap;
	int checkDurationSkill2;
	int numberOfDeadSword;
	int indexKiem = 0;
	void createKiemPhap(float posX);
	void landKiemPhap();
	void doCounterSkill2();

	// Skill 3
	CCArray *poolSkill3;
	int indexSkill3 = 0;
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
	void addStuff();
	void createPool();

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
	void revive();
	void die(Point posOfCammera);
	void listener();

	void doDestroyBodies(b2World *world);

	void updateMe(float dt);

};
#endif // __DuongQua_H__