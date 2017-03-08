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
* Class Global                                                                     
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

//// make life easier
//#define MIdle Idling::getInstance()
//#define MRun Running::getInstance()
//#define MJump Jumping::getInstance()
//#define MLand Landing::getInstance()
//#define MAttack AttackNormal::getInstance()

#endif 
