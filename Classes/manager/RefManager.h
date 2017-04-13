#pragma once
#ifndef __REF_MANAGER_H__
#define __REF_MANAGER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class RefManager
{
public:

	RefManager();
	~RefManager();

protected:
	CC_SYNTHESIZE_READONLY(int, selectedHero, SelectedHero);
	CC_SYNTHESIZE_READONLY(bool, isLockedHero, IsLockedHero);	// to try or smt

	CC_SYNTHESIZE_READONLY(int, currentLevel, CurrentLevel);
	CC_SYNTHESIZE_READONLY(int, currentScore, CurrentScore);

	CC_SYNTHESIZE_READONLY(int, currentHealth, CurrentHealth);

	// life of game
	CC_SYNTHESIZE_READONLY(int, numberOfLife, NumberOfLife);   //
	CC_SYNTHESIZE_READONLY(int, goldExplored, GoldExplored);
	CC_SYNTHESIZE_READONLY(int, diamondBuy, DiamondBuy);


	CC_SYNTHESIZE_READONLY(float, durationSkill_1, DurationSkill_1);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_1, CoolDownSkill_1);

	CC_SYNTHESIZE_READONLY(float, durationSkill_2, DurationSkill_2);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_2, CoolDownSkill_2);

	CC_SYNTHESIZE_READONLY(float, durationSkill_3, DurationSkill_3);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill_3, CoolDownSkill_3);

	CC_SYNTHESIZE_READONLY(int, levelSkill_1, LevelSkill_1);
	CC_SYNTHESIZE_READONLY(int, levelSkill_2, LevelSkill_2);
	CC_SYNTHESIZE_READONLY(int, levelSkill_3, LevelSkill_3);

	CC_SYNTHESIZE_READONLY(int, bonusScore, BonusScore);
	CC_SYNTHESIZE_READONLY(int, bonusGold, BonusGold);

	CC_SYNTHESIZE_READONLY(int, numberItemHealth, NumberItemHealth);
	CC_SYNTHESIZE_READONLY(int, numberItemBird, NumberItemBird);
	CC_SYNTHESIZE_READONLY(int, numberItemMagnet, NumberItemMagnet);
	CC_SYNTHESIZE_READONLY(int, numberItemDoubleGold, NumberItemDoubleGold);
	CC_SYNTHESIZE_READONLY(int, numberItemCoolDown, NumberItemCoolDown);

private:
	UserDefault* ref;
	bool isFirstTime = true;
	static RefManager *refManager;

public:
	static RefManager* getInstance();

	void pointToCurrentHero(int index);		// set props to get
	void unLockHero(int index);
	void increaseLevel();
	
	// baseHP
	void setUpHealth(int health);
	void setDownHealth(int health);

	// set current score after increase level
	void setCurrentScoreAfterIncrease(int score);

	// set increase score after done one game
	void setUpScore(int score);
	
	// void decrease time duration and cooldown
	void decreaseDurationSkill_X(int skill_What, int percent);
	void decreaseCoolDownSkill_X(int skill_What, int percent);

	void increaseBonusScore(int value);
	void increaseBonusGold(int value);

	// up and down

	// eat to play
	void setUpLife(int life);   
	bool setDownLife(int life);
	
	void setUpGoldExplored(int gold);
	bool setDownGold(int gold);

	void setUpDiamondBuy(int diamond);
	bool setDownDiamond(int diamond);


	void increaseLevelSkill_1();
	void increaseLevelSkill_2();
	void increaseLevelSkill_3();

	void increaseNumberItemHealth();				void decreaseNumberItemHealth();
	void increaseNumberItemBird();					void decreaseNumberItemBird();
	void increaseNumberItemMagnet();				void decreaseNumberItemMagnet();
	void increaseNumberItemDoubleGold();			void decreaseNumberItemDoubleGold();
	void increaseNumberItemCoolDown();				void decreaseNumberItemCoolDown();

};

#define REF RefManager::getInstance()

#endif // __REF_MANAGER_H__