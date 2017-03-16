#ifndef __BASE_HERO_H__
#define __BASE_HERO_H__

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "StateMachine.h"
#include "BaseEnemy.h"


USING_NS_CC;

using namespace spine;

class BaseHero : public B2Skeleton
{
protected:
	StateMachine* stateMachine;


	CC_SYNTHESIZE(b2Body*, swordBody, SwordBody);
	CC_SYNTHESIZE(float, trueRadiusOfHero, TrueRadiusOfHero);

	CC_SYNTHESIZE(bool, onGround, OnGround);
	CC_SYNTHESIZE(int, numberOfJump, NumberOfJump);
	CC_SYNTHESIZE(float, jump_vel, JumpVel);

	CC_SYNTHESIZE(Sprite*, slash_1, Slash_1);
	CC_SYNTHESIZE(Sprite*, slash_2, Slash_2);

	// bool to make separate animation
	CC_SYNTHESIZE(bool, isPrior, IsPrior);
	CC_SYNTHESIZE(bool, isPriorSkill1, IsPriorSkill1);
	CC_SYNTHESIZE(bool, isPriorSkill2, IsPriorSkill2);
	CC_SYNTHESIZE(bool, isPriorSkill3, IsPriorSkill3);

	CC_SYNTHESIZE(float, durationSkill1, DurationSkill1);
	CC_SYNTHESIZE(float, durationSkill2, DurationSkill2);
	CC_SYNTHESIZE(float, durationSkill3, DurationSkill3);

	// bool to counter blocked button
	CC_SYNTHESIZE(bool, isDoneDuration1, IsDoneDuration1);
	CC_SYNTHESIZE(bool, isDoneDuration2, IsDoneDuration2);
	CC_SYNTHESIZE(bool, isDoneDuration3, IsDoneDuration3);

public:

	BaseHero(string jsonFile, string atlasFile, float scale);
	static BaseHero* create(string jsonFile, string atlasFile, float scale);

	virtual void initSwordPhysic(b2World *world, Point position, float width);
	void changeSwordCategoryBitmask(uint16 bit);

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
	virtual void die(Point posOfCammera);
	virtual void listener();
	virtual void updateMe(float dt);

	virtual void doCounterSkill1();

	StateMachine* getFSM();
};
#endif // __SOLDIER_H__