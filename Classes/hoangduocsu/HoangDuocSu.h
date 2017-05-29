#ifndef __HOANG_DUOC_SU_H__
#define __HOANG_DUOC_SU_H__

#include "BaseHero.h"
#include "manager/AudioManager.h"

class HoangDuocSu : public BaseHero
{
public:

	HoangDuocSu(string jsonFile, string atlasFile, float scale);
	static HoangDuocSu* create(string jsonFile, string atlasFile, float scale);
	void initCirclePhysic(b2World *world, Point pos);
	void initSwordPhysic(b2World *world, Point position);

protected:

	void fastAndFurious();
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

};

#endif // __HOANG_DUOC_SU_H__