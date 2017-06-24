#ifndef __ENEMY_BOSS1_H__
#define __ENEMY_BOSS1_H__

#include "BaseEnemy.h"
#include "StateBoss.h"
#include "SlashBoss.h"

#include "coin/Coin.h"
#include "ui\CocosGUI.h"


class EnemyBoss1 : public BaseEnemy
{
private:
	CC_SYNTHESIZE(int, balanceAt1andAT2, BalanceAt1andAT2);
	CC_SYNTHESIZE(int, levelBoss, LevelBoss);
	CC_SYNTHESIZE(int, randAt2, RandAt2);
	CC_SYNTHESIZE(bool, isImmortal, IsImmortal);
	CC_SYNTHESIZE_READONLY(Vec2, baseVelocity, BaseVelocity);
	CC_SYNTHESIZE_READONLY(Vec2, moveVelocity, moveVelocity);
	CC_SYNTHESIZE(Vec2, realtimeVec, RealtimeVec);
	CC_SYNTHESIZE(Vec2, realMoveVelocity, RealMoveVelocity);
	CC_SYNTHESIZE(int, controlAttack, ControlAttack);
	CC_SYNTHESIZE(int, controlState, ControlState);
	CC_SYNTHESIZE(SkeletonAnimation*,exxp, Exxp);
	CC_SYNTHESIZE(CCArray *, coinPool, CoinPool);
	//CC_SYNTHESIZE(Sprite*, exxp, Exxp);
	const int maxControl = 960;
	int control;
public:
	//bool lockState;
	b2Vec2 dsBoss_Follow;
	Vec2 heroLocation;
	StateBoss *normalState;
	StateBoss *crazyState;
	CCArray *slashPool;
	ui::LoadingBar *spHp;
	int indexSlash;
	int indexCoin;
	float scaleBoss;
	EnemyBoss1(string jsonFile, string atlasFile, float scale);
	static EnemyBoss1* create(string jsonFile, string atlasFile, float scale);

	void idle();
	void attack();
	void attack2();
	virtual void fixStupid();
	virtual void die();
	virtual void createPool();
	virtual void creatSlash(float angel);
	virtual void creatHidenSlash(float angel); // tao check chem
	virtual void creatHpSprite();
	void boomboom();
	int createGold();
	void createCoinPool();
	void updateMe(BaseHero* hero);

	virtual void listener();
	virtual bool checkStop();
	void changeState(StateBoss *state);
	void doAttack1();
	virtual void doAttack2();
	virtual Vec2 getPosGenSlash();
	virtual void playSoundAttack1();
	virtual void playSoundAttack2();
	virtual void playSoundHit();
	virtual void playSoundDie();
	void immortal();
	void unImmortal();
	virtual void completeQuest();
	// void initCirclePhysic(b2World *world, Point pos);
};

#endif // __ENEMY_H__
