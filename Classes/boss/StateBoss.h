#ifndef __STATE_BOSS_H__
#define __STATE_BOSS_H__

class EnemyBoss1;

enum IdStateBoss {
	Idling,
	Attack1,
	Attack2,
	Stupiding,
	FixStupiding,
	Die,
	Injure,

};

class StateBoss
{
public:
	IdStateBoss id;
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	StateBoss();
	~StateBoss();
	virtual void enter(EnemyBoss1 *boss);
	virtual void execute(EnemyBoss1 *boss) = 0;
	virtual void exit(EnemyBoss1 *boss)=0;
	
};

class BossIdling : public StateBoss {
public:
	BossIdling();
	~BossIdling();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);
};

class BossAttacking1 : public StateBoss {
public:
	BossAttacking1();
	~BossAttacking1();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss) ;
	void exit(EnemyBoss1 *boss);
};

class BossAttacking2 : public StateBoss {
public:
	BossAttacking2();
	~BossAttacking2();
	
	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);
};

class BossStupiding : public StateBoss {
public:
	BossStupiding();
	~BossStupiding();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);
};

class BossFixingStupid : public StateBoss {
public:
	BossFixingStupid();
	~BossFixingStupid();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);
	
};


class BossDie : public StateBoss {
public:
	BossDie();
	~BossDie();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);

};

class BossInjure : public StateBoss {
public:
	BossInjure();
	~BossInjure();

	void enter(EnemyBoss1 *boss);
	void execute(EnemyBoss1 *boss);
	void exit(EnemyBoss1 *boss);

};


#endif // __ENEMY_H__
