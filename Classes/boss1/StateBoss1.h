#ifndef __STATE_BOSS1_H__
#define __STATE_BOSS1_H__

class EnemyBoss1;

class StateBoss1
{
public:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	StateBoss1();
	~StateBoss1();
	virtual void enter(EnemyBoss1 *boss);
	virtual void execute(EnemyBoss1 *boss) = 0;
	
};

class Boss1Idling : public StateBoss1 {
public:
	Boss1Idling();
	~Boss1Idling();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
};

class Boss1Attacking1 : public StateBoss1 {
public:
	Boss1Attacking1();
	~Boss1Attacking1();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss) ;
};

class Boss1Attacking2 : public StateBoss1 {
public:
	Boss1Attacking2();
	~Boss1Attacking2();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
};

class Boss1Stupiding : public StateBoss1 {
public:
	Boss1Stupiding();
	~Boss1Stupiding();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
};

class Boss1FixingStupid : public StateBoss1 {
public:
	Boss1FixingStupid();
	~Boss1FixingStupid();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	
};


#endif // __ENEMY_H__
