#ifndef __HOANG_DUNG_H__
#define __HOANG_DUNG_H__

#include "BaseHero.h"
#include "manager/AudioManager.h"

class HoangDung : public BaseHero
{
public:

	HoangDung(string jsonFile, string atlasFile, float scale);
	static HoangDung* create(string jsonFile, string atlasFile, float scale);

protected:

	void doCounterSkill1();
	void doCounterSkill2();
	void doCounterSkill3();
	

	//////////////////////////////////////////////////////////////////////////
	void createSlash();
	void runSlash();
	void runSlashLand();
	//////////////////////////////////////////////////////////////////////////

	void initCirclePhysic(b2World *world, Point pos);
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
#endif // __HOANG_DUNG_H__