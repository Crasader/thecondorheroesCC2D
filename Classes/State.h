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
	virtual void run(BaseHero* hero);
	virtual void jump(BaseHero *hero);
	virtual void land(BaseHero *hero);
	virtual void attack(BaseHero *hero);
};

/**
* Class Running
*/
class Running : public State
{
public:
	Running();
	~Running();

	void jump(BaseHero *hero);
	void land(BaseHero* hero);
	void attack(BaseHero *hero);

};

/**
* Class Idling
*/
class Idling : public State
{
public:
	Idling();
	~Idling();
	void run(BaseHero *hero);


};

/**
* Class Jumping
*/
class Jumping : public State
{
public:
	Jumping();
	~Jumping();
	void jump(BaseHero *hero);
	void land(BaseHero* hero);
	void attack(BaseHero *hero);
};

/**
* Class DoupleJumping
*/
class DoupleJumping : public State
{
public:
	DoupleJumping();
	~DoupleJumping();

	void land(BaseHero* hero);
};

/**
* Class Landing
*/
class Landing : public State
{
public:
	Landing();
	~Landing();

	void jump(BaseHero* hero);
	void run(BaseHero* hero);
	void attack(BaseHero *hero);
};

/**
* Class Landing2
*/
class Landing2 : public State
{
public:
	Landing2();
	~Landing2();

	void run(BaseHero *hero);
	void attack(BaseHero *hero);

};


/**
* Class Attack
*/
class Attack : public State
{
public:
	Attack();
	~Attack();

	void run(BaseHero *hero);
	void jump(BaseHero *hero);
	void land(BaseHero *hero);

};


#endif 
