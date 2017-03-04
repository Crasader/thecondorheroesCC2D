#ifndef __BASE_HERO_H_
#define __BASE_HERO_H_

#include "B2Skeleton.h"
#include "GB2ShapeCache-x.h"
#include "State.h"


USING_NS_CC;

using namespace spine;

class BaseHero : public B2Skeleton
{
private:
	State* stateMachine;
public:

	BaseHero(string jsonFile, string atlasFile, float scale);
	static BaseHero* create(string jsonFile, string atlasFile, float scale);

	virtual void move();
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

	virtual void changeState(State *newState);
};
#endif // __SOLDIER_H__