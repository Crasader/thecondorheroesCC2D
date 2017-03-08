#include "StateMachine.h"
#include "BaseHero.h"



StateMachine::StateMachine(BaseHero *owner) : owner(owner), currentState(nullptr), previousState(nullptr)
{
}

StateMachine::~StateMachine()
{
}


void StateMachine::setCurrentState(State * m_current)
{
	currentState = m_current;
}

void StateMachine::setPreviousState(State * m_previous)
{
	previousState = m_previous;
}

void StateMachine::Update()
{
	if (currentState != previousState) {
		currentState->execute(owner);

		previousState = currentState;
	}
	
}

void StateMachine::changeState(State * state)
{
	currentState = state;
}

void StateMachine::revertToPreviousState()
{
	changeState(previousState);
}
