#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "state/State.h"
#include "cocos2d.h"

USING_NS_CC;

class BaseHero;

/**
* Class StateMachine
*/
class StateMachine
{
public:
	StateMachine(BaseHero *owner);
	~StateMachine();

	BaseHero *owner;

	State* globalState;
	State* currentState;
	State* previousState;

	void setGlobalState(State *m_global);
	void setCurrentState(State *m_current);
	void setPreviousState(State *m_previous);

	void Update();
	void changeState(State* state);
	void revertToGlobalState();
};

#endif