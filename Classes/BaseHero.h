#ifndef __BASE_HERO_H__
#define __BASE_HERO_H__

#include "B2Skeleton.h"
#include "utils/GB2ShapeCache-x.h"
#include "state/StateMachine.h"
#include "BaseEnemy.h"


USING_NS_CC;

using namespace spine;
using namespace std;

class BaseHero : public B2Skeleton
{

protected:
	StateMachine* stateMachine;
	list<BaseEnemy *> m_lEnemiesSelectedBySkill1;		//DuongPM edited
	list<BaseEnemy *> m_lEnemiesSelectedBySkill2;		//DuongPM edited
	list<BaseEnemy *> m_lEnemiesSelectedBySkill3;

	CC_SYNTHESIZE(b2Body*, swordBody, SwordBody);
	CC_SYNTHESIZE(float, boxHeight, BoxHeight);
	CC_SYNTHESIZE(float, trueRadiusOfHero, TrueRadiusOfHero);

	CC_SYNTHESIZE(int, maxHealth, MaxHealth);
	// you can not be injured when no die 
	CC_SYNTHESIZE(bool, isNoDie, IsNoDie);

	CC_SYNTHESIZE(bool, onGround, OnGround);
	CC_SYNTHESIZE(int, numberOfJump, NumberOfJump);
	CC_SYNTHESIZE(float, jump_vel, JumpVel);
	CC_SYNTHESIZE(bool, noActive, NoActive);
	CC_SYNTHESIZE(bool, isKillAll, IsKillAll);


	CC_SYNTHESIZE(Sprite*, bloodScreen, BloodScreen);
	CC_SYNTHESIZE(Sprite*, blash, Blash);
	CC_SYNTHESIZE(SkeletonAnimation*, slash, SlashAni);
	CC_SYNTHESIZE(SkeletonAnimation*, slashLand, SlashLand);

	CC_SYNTHESIZE(SkeletonAnimation*, activeSkill, ActiveSkill);
	CC_SYNTHESIZE(SkeletonAnimation*, slashBreak, SlashBreak);

	CC_SYNTHESIZE(SkeletonAnimation*, smokeJumpX2, SmokeJumpX2);
	CC_SYNTHESIZE(SkeletonAnimation*, smokeLand, SmokeLand);

	CC_SYNTHESIZE(SkeletonAnimation*, smokeRun, SmokeRun);
	CC_SYNTHESIZE(SkeletonAnimation*, reviveMe, ReviveMe);

	CC_SYNTHESIZE(SkeletonAnimation*, suctionCoin, SuctionCoin);

	// stuff here
	CC_SYNTHESIZE(int, score, Score);
	CC_SYNTHESIZE(int, coinExplored, CoinExplored);
	CC_SYNTHESIZE(int, coinRatio, CoinRatio);

	// bool to make separate animation
	CC_SYNTHESIZE(bool, isPriorInjured, IsPriorInjured);
	CC_SYNTHESIZE(bool, isPriorAttack, IsPriorAttack);
	CC_SYNTHESIZE(bool, isPriorSkill1, IsPriorSkill1);
	CC_SYNTHESIZE(bool, isPriorSkill2, IsPriorSkill2);
	CC_SYNTHESIZE(bool, isPriorSkill3, IsPriorSkill3);
	CC_SYNTHESIZE(int, dieHard, DieHard);		// accept die only one time

	CC_SYNTHESIZE(float, durationSkill1, DurationSkill1);
	CC_SYNTHESIZE(float, durationSkill2, DurationSkill2);
	CC_SYNTHESIZE(float, durationSkill3, DurationSkill3);

	// bool to counter blocked button
	CC_SYNTHESIZE(bool, isDoneDuration1, IsDoneDuration1);
	CC_SYNTHESIZE(bool, isDoneDuration2, IsDoneDuration2);
	CC_SYNTHESIZE(bool, isDoneDuration3, IsDoneDuration3);

	CC_SYNTHESIZE(bool, isDriverEagle, IsDriverEagle);
	CC_SYNTHESIZE(float, currentRunDis, CurrentRunDis);
	CC_SYNTHESIZE(float, preRunDis, PreRunDis);


public:
	int checkItem[5];// key and state power, state 0 power off. key define in global

	BaseHero(string jsonFile, string atlasFile, float scale);
	static BaseHero* create(string jsonFile, string atlasFile, float scale);

	virtual void initSwordPhysic(b2World *world, Point position, float width);
	void changeSwordCategoryBitmask(uint16 bit);
	virtual void addStuff();
	virtual void createPool();


	virtual void idle();
	virtual void run();
	virtual void normalJump();
	virtual void doubleJump();
	virtual void landing();
	virtual void die();
	virtual void attackNormal();
	virtual void attackLanding();
	virtual void attackBySkill1();
	virtual void attackBySkill2();
	virtual void attackBySkill3();
	virtual void injured();
	virtual void revive();
	virtual void die(Point posOfCammera);
	virtual void listener();
	virtual void updateMe(float dt);

	virtual void doCounterSkill1();
	virtual void doCounterSkill2();
	virtual void doCounterSkill3();

	virtual void doDestroyBodies(b2World* world);
	virtual void updateAttackBossAsTarget(BaseEnemy *p_pBoss);

	// default methods
	void selectEnemyBySkill1(BaseEnemy *p_pEnemySelected); //DuongPM edited
	void deSelectEnemyBySkill1();							//DuongPM edited
	void selectEnemyBySkill2(BaseEnemy *p_pEnemySelected); //DuongPM edited
	void deSelectEnemyBySkill2();							//DuongPM edited
	void selectEnemyBySkill3(BaseEnemy *p_pEnemySelected); //DuongPM edited
	void deSelectEnemyBySkill3();							//DuongPM edited

	void killThemAll();

	
	void createMapItem();
	void updateMapItem();
	int getItemValue(int keyItem);
	void setItemValue(int keyItem, int value);
	

	// animation
	void activeSkillAni();
	void slashBreakAni();
	void smokeJumpX2Ani();
	void smokeLandingAni();
	void smokeRunAni();
	void reviveAni();
	void suctionAni();

	StateMachine* getFSM();
};
#endif // __SOLDIER_H__
