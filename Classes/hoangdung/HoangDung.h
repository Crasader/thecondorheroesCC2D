#ifndef __HOANG_DUNG_H__
#define __HOANG_DUNG_H__

#include "BaseHero.h"
#include "AudioManager.h"
#include "Rada.h"
#include "DaCauBongPhap.h"
#include "ManThienHoaVu.h"

class HoangDung : public BaseHero
{
public:

	HoangDung(string jsonFile, string atlasFile, float scale);
	static HoangDung* create(string jsonFile, string atlasFile, float scale);
	void initSwordPhysic(b2World *world, Point position);

	void initCirclePhysic(b2World *world, Point pos);

protected:

	Rada *m_pRadaShield;
	SkeletonAnimation* thunderShield;

	SkeletonAnimation* Skill2Effect1;
	SkeletonAnimation* Skill2Effect2;
	SkeletonAnimation* Skill2Effect3;

	void createThunderShield();
	void createSkill2Effect();

	bool isReviveAfterDead = false;

	CCArray* poolSkill2;
	int indexSkill2 = 0;
	int numberOfDeadMTHV;
	void createManThienHoaVu(Point posHand, int Zoder, float angle);
	void shootManThienHoaVu();
	list<ManThienHoaVu *> listManThienHoaVu;

	CCArray* poolSkill3;
	int indexSkill3 = 0;
	int numberOfDeadDCBP;
	list<DaCauBongPhap*> listDaCauBongPhap;
	void createDaCauBongPhap(Point posSword);
	void slashDaCauBongPhap();

	void doCounterSkill1();
	void doCounterSkill2();
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
	void injured();
	void listener();

	void stopSkillAction(bool stopSkill1, bool stopSkill2, bool stopSkill3);
	void doDestroyBodies(b2World *world);

	void updateMe(float dt);

private:

};
#endif // __HOANG_DUNG_H__
