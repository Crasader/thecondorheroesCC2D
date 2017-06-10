#ifndef __HOANG_DUOC_SU_H__
#define __HOANG_DUOC_SU_H__

#include "BaseHero.h"
#include "colong/Rada.h"
#include "hoangdung/DaCauBongPhap.h"
#include "manager/AudioManager.h"

class HoangDuocSu : public BaseHero
{
public:

	HoangDuocSu(string jsonFile, string atlasFile, float scale);
	static HoangDuocSu* create(string jsonFile, string atlasFile, float scale);
	void initCirclePhysic(b2World *world, Point pos);
	void initSwordPhysic(b2World *world, Point position);

protected:

	SkeletonAnimation* effectSkill1;
	Rada *shield;
	void fastAndFurious();
	void doCounterSkill1();

	SkeletonAnimation *effectSkill2;
	CCArray* poolSkill2;
	int counterSkill2 = 0;
	int indexSkill2 = 0;
	int numberOfDeadDCTC;
	list<DaCauBongPhap*> listDCTC;
	void createDCTC(Point pos, float angle);
	void slashDCTC();
	void doCounterSkill2();

	//SkeletonAnimation *effectSkill3;
	int idSoundSkill3;
	Sprite* wave;
	void killAll();
	void doCounterSkill3();

	void runEffectSkill1();

	void createEffect();
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

#define SOUND_HDS_DIE				"sound/hdsdie.mp3"
#define SOUND_HDS_HIT				"sound/hdshit.mp3"
#define SOUND_HDS_SKILL1			"sound/hdsskill1.mp3"
#define SOUND_HDS_SKILL2			"sound/hdsskill2.mp3"
#define SOUND_HDS_SKILL3			"sound/hdsskill3.mp3"


#endif // __HOANG_DUOC_SU_H__