#ifndef __STATE_H__
#define __STATE_H__

class BaseHero;

class State
{
public:
	State();
	~State();
	virtual void run(BaseHero* hero);
	virtual void jump(BaseHero *hero);
	virtual void land(BaseHero *hero);
};

class Running: public State
{
public:
	Running();
	~Running();

	void jump(BaseHero *hero);
	void land(BaseHero* hero);
	

};

class Idling:  public State
{
public:
	Idling();
	~Idling();
	void run(BaseHero *hero);

private:

};

class Jumping : public State
{
public:
	Jumping();
	~Jumping();
	void jump(BaseHero *hero);
	void land(BaseHero* hero);

};

class DoupleJumping : public State
{
public:
	DoupleJumping();
	~DoupleJumping();
	
	void land(BaseHero* hero);

};

class Landing : public State
{
public:
	Landing();
	~Landing();

	void jump(BaseHero* hero);
	void run(BaseHero* hero);

};

class Landing2 : public State
{
public:
	Landing2();
	~Landing2();

	void run(BaseHero*hero);

private:

};

#endif 
