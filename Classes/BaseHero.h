#ifndef __BASE_HERO_H_
#define __BASE_HERO_H_

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "State.h"


USING_NS_CC;

using namespace spine;

class BaseHero : public B2Skeleton
{
protected:
	State* stateMachine;

	CC_SYNTHESIZE(float, trueRadiusOfHero, TrueRadiusOfHero);
	CC_SYNTHESIZE(int, numberOfJump, NumberOfJump);
	CC_SYNTHESIZE(bool, onGround, OnGround);
	CC_SYNTHESIZE(float, jump_vel, JumpVel);

public:

	BaseHero(string jsonFile, string atlasFile, float scale);
	static BaseHero* create(string jsonFile, string atlasFile, float scale);

	virtual void run();
	virtual void normalJump();
	virtual void doubleJump();
	virtual void landing();
	virtual void die();
	virtual void attacknormal();
	virtual void attackBySkill1();
	virtual void attackBySkill2();
	virtual void attackBySkill3();
	virtual void injured();
	virtual void die(Point posOfCammera);
	virtual void listener();
	virtual void update(float dt);

	void changeState(State *newState);
	State* getCurrentState();
};
#endif // __SOLDIER_H__