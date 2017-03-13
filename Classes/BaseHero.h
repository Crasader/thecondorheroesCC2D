#ifndef __BASE_HERO_H_
#define __BASE_HERO_H_

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

	CC_SYNTHESIZE(Sprite*, slash, Slash);

	CC_SYNTHESIZE(bool, isPrior, IsPrior);
	CC_SYNTHESIZE(bool, isPriorSkill1, IsPriorSkill1);
	CC_SYNTHESIZE(bool, isPriorSkill2, IsPriorSkill2);
	CC_SYNTHESIZE(bool, isPriorSkill3, IsPriorSkill3);

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


	StateMachine* getFSM();
};
#endif // __SOLDIER_H__