#ifndef __STATE_H__
#define __STATE_H__


class BaseHero;

/**
* Class State
*/
class State
{
public:
	State();
	~State();
	virtual void execute(BaseHero* hero);

	static State* m_state;

	static State* getInstance();
};

/**
* Class Running
*/
class Running : public State
{
public:
	Running();
	~Running();

	void execute(BaseHero *hero);

	static Running* m_run;

	static Running* getInstance();
};

/**
* Class Idling
*/
class Idling : public State
{
public:
	Idling();
	~Idling();
	void execute(BaseHero *hero);
	static Idling *m_idle;

	static Idling* getInstance();
};

/**
* Class Jumping
*/
class Jumping : public State
{
public:
	Jumping();
	~Jumping();
	void execute(BaseHero *hero);
	static Jumping* m_jump;

	static Jumping* getInstance();
};

/**
* Double jump                                                                     
*/
class DoubleJumping : public State
{
public:
	DoubleJumping();
	~DoubleJumping();
	void execute(BaseHero *hero);
	static DoubleJumping* m_doubleJump;

	static DoubleJumping* getInstance();
};

/**
* Class Landing
*/
class Landing : public State
{
public:
	Landing();
	~Landing();

	void execute(BaseHero *hero);

	static Landing* m_land;

	static Landing* getInstance();
};

/**
* Class Landing Revive
*/
class LandingRevive : public State
{
public:
	LandingRevive();
	~LandingRevive();

	void execute(BaseHero *hero);

	static LandingRevive* m_landRevive;

	static LandingRevive* getInstance();
};


/**
* Class Injured                                                                     
*/
class Injuring : public State
{
public:
	Injuring();
	~Injuring();

	void execute(BaseHero *hero);
	static Injuring * m_injured;
	static Injuring* getInstance();
};


/**
* Class Dead
*/
class Dead : public State
{
public:
	Dead();
	~Dead();

	void execute(BaseHero *hero);
	static Dead * m_dead;
	static Dead* getInstance();
};

/**
* Class Attack                                                                    
*/
class AttackNormal : public State 
{
public:
	AttackNormal();
	~AttackNormal();

	void execute(BaseHero *hero);
	static AttackNormal * m_attack;
	static AttackNormal* getInstance();
};

/**
* Class Skill1
*/
class Skill1 : public State
{
public:
	Skill1();
	~Skill1();

	void execute(BaseHero *hero);
	static Skill1 * m_skill1;
	static Skill1* getInstance();
};

/**
* Class Skill2
*/
class Skill2 : public State
{
public:
	Skill2();
	~Skill2();

	void execute(BaseHero *hero);
	static Skill2 * m_skill2;
	static Skill2* getInstance();
};


/**
* Class Skill3
*/
class Skill3 : public State
{
public:
	Skill3();
	~Skill3();

	void execute(BaseHero *hero);
	static Skill3 * m_skill3;
	static Skill3* getInstance();
};



// make life easier
#define MIdle Idling::getInstance()
#define MRun Running::getInstance()
#define MJump Jumping::getInstance()
#define MLand Landing::getInstance()
#define MLandRevive LandingRevive::getInstance()
#define MAttack AttackNormal::getInstance()
#define MInjured Injuring::getInstance()
#define MDoubleJump DoubleJumping::getInstance()
#define MDie Dead::getInstance()
#define MSKill1 Skill1::getInstance()
#define MSKill2 Skill2::getInstance()
#define MSKill3 Skill3::getInstance()

#endif
