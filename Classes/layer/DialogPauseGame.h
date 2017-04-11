#ifndef __DIALOG_PAUSE_GAME_H__
#define __DIALOG_PAUSE_GAME_H__

#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;


class DialogPauseGame : public Layer
{
protected:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();


public:
	virtual bool init();
	static DialogPauseGame* create();

public:

	void resumeGame(Ref* pSender);
	void backHome(Ref* pSender);
	void replayGame(Ref* pSender);
	void nextState(Ref* pSender);
	void restartGame(Ref* pSender);
	void upgrade(Ref* pSender);

	virtual void effect();

};

class DialogPause : public DialogPauseGame
{

public:
	bool init();
	static DialogPause* create();
};


class DialogRevive : public DialogPauseGame
{
protected:
	ui::LoadingBar *loading;
	int countDown = 50;

	Label *goldReviveLb;
	CC_SYNTHESIZE(int, gold, GoldToShow);
	CC_SYNTHESIZE(int, numberOfRevive, NumberOfRevive);

public:
	bool init(int numberOfRevive);
	static DialogRevive* create(int numberOfRevive);

};

class DialogStageClear : public DialogPauseGame
{
protected:
	Label *scoreLb;
	Label *goldLb;

	CC_SYNTHESIZE(int, score, ScoreToShow);
	CC_SYNTHESIZE(int, gold, GoldToShow);

public:
	bool init(int score, int gold);
	static DialogStageClear* create(int score, int gold); // -1 is pause, 0 is die, 1 is win
};

class DialogOverGame : public DialogStageClear
{

	CC_SYNTHESIZE(int, score, ScoreToShow);
	CC_SYNTHESIZE(int, gold, GoldToShow);

public:
	bool init(int score, int gold);
	static DialogOverGame* create(int score, int gold);

};


#endif // __DIALOG_H__