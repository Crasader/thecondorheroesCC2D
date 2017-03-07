#ifndef __BASE_HERO_H_
#define __BASE_HERO_H_

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "State.h"
#include "BaseEnemy.h"


USING_NS_CC;

using namespace spine;

class BaseHero : public B2Skeleton
{
protected:
	State* stateMachine;

	CC_SYNTHESIZE(b2Body*, swordBody, SwordBody);
	CC_SYNTHESIZE(State*, previous_state, PreviousState);
	CC_SYNTHESIZE(float, trueRadiusOfHero, TrueRadiusOfHero);
	CC_SYNTHESIZE(int, numberOfJump, NumberOfJump);
	CC_SYNTHESIZE(float, jump_vel, JumpVel);

	CC_SYNTHESIZE(Sprite*, slash, Slash);

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
	virtual void update(float dt);

	// check attack near by
	virtual void checkNearBy(BaseEnemy *enemy);

	void changeState(State *newState);
	State* getCurrentState();
};
#endif // __SOLDIER_H__