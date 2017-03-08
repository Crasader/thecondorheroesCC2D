#include "State.h"
#include "BaseHero.h"


State* State::m_state;
Idling* Idling::m_idle;
Running* Running::m_run;
Landing* Landing::m_land;
Jumping* Jumping::m_jump;
AttackNormal* AttackNormal::m_attack;

State::State()
{
	
}

State::~State()
{
}

void State::execute(BaseHero *hero)
{
	// do nothing
}


State * State::getInstance()
{
	if (m_state == nullptr)
		m_state = new State();
	return m_state;
}



// RUNNING
Running::Running()
{
}

Running::~Running()
{
}

void Running::execute(BaseHero * hero)
{
	hero->run();
}

Running * Running::getInstance()
{
	if (m_run == nullptr)
		m_run = new Running();
	return m_run;
}


// IDLE
Idling::Idling()
{
}

Idling::~Idling()
{
}

void Idling::execute(BaseHero * hero)
{
	hero->run();
	
}

Idling * Idling::getInstance()
{
	if (m_idle == nullptr)
		m_idle = new Idling();
	return m_idle;
}



// JUMP
Jumping::Jumping()
{
}

Jumping::~Jumping()
{
}

void Jumping::execute(BaseHero * hero)
{
	if (hero->getNumberOfJump() > 0) {
		hero->normalJump();
	}
	else {
		hero->doubleJump();
	}
}

Jumping * Jumping::getInstance()
{
	if (m_jump == nullptr)
		m_jump = new Jumping();
	return m_jump;
}



// LANDING
Landing::Landing()
{
}

Landing::~Landing()
{
}

void Landing::execute(BaseHero * hero)
{
	hero->landing();
}

Landing * Landing::getInstance()
{
	if (m_land == nullptr)
		m_land = new Landing();
	return m_land;
}

/**
*                                                                      
*/
AttackNormal::AttackNormal()
{
}

AttackNormal::~AttackNormal()
{
}

void AttackNormal::execute(BaseHero * hero)
{
	if (hero->getOnGround())
		hero->attackNormal();
	else
		hero->attackLanding();
}


AttackNormal * AttackNormal::getInstance()
{
	if (m_attack == nullptr)
		m_attack = new AttackNormal();
	return m_attack;
}
