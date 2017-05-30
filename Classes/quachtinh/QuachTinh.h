#ifndef __QUACH_TINH_H__
#define __QUACH_TINH_H__

#include "BaseHero.h"
#include "QT_CuuAmChanKinh.h"
#include "QT_ThanLongBatVi.h"
#include "QT_SongLongXuatThuy.h"
#include "AudioManager.h"

class QuachTinh : public BaseHero
{
public:

	QuachTinh(string jsonFile, string atlasFile, float scale);
	static QuachTinh* create(string jsonFile, string atlasFile, float scale);
	void initSwordPhysic(b2World *world, Point position);

	void pause();
	void resume();

protected:

	// Skill 1
	CCArray* poolSkill1;
	int indexSkill1 = 0;
	list<ChanKinh*> listRock;
	int numberOfDeadRock;
	int indexRock = 0;
	void createRock(float posX);
	void landRocks();
	void doCounterSkill1();

	CCArray* poolSkill2;
	int indexSkill2 = 0;
	list<ThanLong*> listThanLong;
	int numberOfCreateTL = 0;
	int numberOfDeadThanLong;
	float widthTL = -1;
	float originTL = 0;
	int indexThanLong = 0;
	void createTL(float posX);
	void landTLs();
	void doCounterSkill2();

	void doCounterSkill3();
	

	CC_SYNTHESIZE(int, checkHealth, CheckHealth);

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
	SongLong *songLong;

};

#define TIME_UP		15

#endif // __QUACH_TINH_H__
