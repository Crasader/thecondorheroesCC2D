#ifndef __DUONG_QUA_H__
#define __DUONG_QUA_H__

#include "DQ_TieuHonChuong.h"
#include "DQ_ToanChanKiemPhap.h"
#include "DQ_DocCoKiemPhap.h"
#include "BaseHero.h"
#include "manager/AudioManager.h"

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
	void createToanChanKiemPhap(Point posSword);
	void slashToanChanKiemPhap();
	void doCounterSkill1();

	void fastAndFurious();


	// Skill 2
	/*CCArray* poolSkill2;
	int indexSkill2 = 0;*/
	list<KiemPhap*> listKiemPhap;
	int numberOfDeadSword;
	int indexKiem = 0;
	void createKiemPhap(float posX);
	void landKiemPhap();
	void doCounterSkill2();

	// Skill 3
	CCArray *poolSkill3;
	int indexSkill3 = 0;
	SkeletonAnimation* spiritHole;
	int numberOfDeadTHC;
	list<TieuHonChuong*> listTieuHonChuong;
	void createSpiritHole();
	Point getLocalSpiritBonePos(string boneName);
	void createTieuHonChuong(Point posHand, int Zoder);
	void shootTieuHonChuong();
	void doCounterSkill3();
	

	//////////////////////////////////////////////////////////////////////////
	void createSlash();
	void runSlash();
	void runSlashLand();
	//////////////////////////////////////////////////////////////////////////

	void addStuff();
	void createPool();

	void run();
	void normalJump();
	void doubleJump();
	void landing();
	void die();
	void attackNormal();
	void attackLanding();
	void attackBySkill1();
	void injured();
	void listener();

	void stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3);
	void doDestroyBodies(b2World *world);

	void updateMe(float dt);

};
#endif // __DuongQua_H__