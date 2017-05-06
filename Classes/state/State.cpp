#include "State.h"
#include "BaseHero.h"

// define static state here
State* State::m_state;
Idling* Idling::m_idle;
Running* Running::m_run;
Landing* Landing::m_land;
Jumping* Jumping::m_jump;
DoubleJumping* DoubleJumping::m_doubleJump;
AttackNormal* AttackNormal::m_attack;
Revive* Revive::m_revive;
Injuring* Injuring::m_injured;
Dead* Dead::m_dead;
Skill1* Skill1::m_skill1;
Skill2* Skill2::m_skill2;
Skill3* Skill3::m_skill3;



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


Idling::Idling()
{
}

Idling::~Idling()
{
}

// IDLE
void Idling::execute(BaseHero * hero)
{
	hero->idle();
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
	hero->normalJump();
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


/**
*
*/
Revive::Revive()
{
}

Revive::~Revive()
{
}

void Revive::execute(BaseHero * hero)
{
	hero->revive();
}


Revive * Revive::getInstance()
{
	if (m_revive == nullptr)
		m_revive = new Revive();
	return m_revive;
}


// Injured
Injuring::Injuring()
{
}

Injuring::~Injuring()
{
}

void Injuring::execute(BaseHero * hero)
{
	hero->injured();
}

Injuring * Injuring::getInstance()
{
	if (m_injured == nullptr)
		m_injured = new Injuring();
	return m_injured;
}



// Double jump
DoubleJumping::DoubleJumping()
{
}

DoubleJumping::~DoubleJumping()
{
}

void DoubleJumping::execute(BaseHero * hero)
{
	hero->doubleJump();
}

DoubleJumping * DoubleJumping::getInstance()
{
	if (m_doubleJump == nullptr)
		m_doubleJump = new DoubleJumping();
	return m_doubleJump;
}



// DEAD
Dead::Dead()
{
}

Dead::~Dead()
{
}

void Dead::execute(BaseHero * hero)
{
	hero->die();
}

Dead * Dead::getInstance()
{
	if (m_dead == nullptr)
		m_dead = new Dead();
	return m_dead;
}


// SKILL 1
Skill1::Skill1()
{
}

Skill1::~Skill1()
{
}

void Skill1::execute(BaseHero * hero)
{
	hero->attackBySkill1();
}

Skill1 * Skill1::getInstance()
{
	if (m_skill1 == nullptr)
		m_skill1 = new Skill1();
	return m_skill1;
}



// SKILL 2
Skill2::Skill2()
{
}

Skill2::~Skill2()
{
}

void Skill2::execute(BaseHero * hero)
{
	hero->attackBySkill2();
}

Skill2 * Skill2::getInstance()
{
	if (m_skill2 == nullptr)
		m_skill2 = new Skill2();
	return m_skill2;
}



// SKILL 3
Skill3::Skill3()
{
}

Skill3::~Skill3()
{
}

void Skill3::execute(BaseHero * hero)
{
	hero->attackBySkill3();
}

Skill3 * Skill3::getInstance()
{
	if (m_skill3 == nullptr)
		m_skill3 = new Skill3();
	return m_skill3;
}
