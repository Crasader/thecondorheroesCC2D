#ifndef __DIALOG_PAUSE_GAME_H__
#define __DIALOG_PAUSE_GAME_H__

#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include "thirdsdkhelper\AdmobHelper.h"
#include "thirdsdkhelper\VungleHelper.h"
#define KEY_PRE_STAGE_STATUS "keyPreStageStatus"// true is win, false is else
USING_NS_CC;
using namespace std;
using namespace spine;


class DialogPauseGame : public Layer
{
protected:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	Menu* menu;

public:
	virtual bool init();
	static DialogPauseGame* create();
	void onExit();

public:

	void resumeGame();
	void backHome();
	void overGame();
	void replayGame(int goldRevive, bool isWatchVideo);
	void nextStage();
	void restartGame();

	virtual void effect();

};

class DialogPause : public DialogPauseGame
{
private:
	void selectedEventMusic(Ref *pSender, ui::CheckBox::EventType type);
	void selectedEventSound(Ref *pSender, ui::CheckBox::EventType type);

public:
	bool init();
	static DialogPause* create();
};


class DialogRevive : public DialogPauseGame
{
private:
	int countDown = 50;
	ui::LoadingBar *loading;

public:
	bool init(int numberOfRevive, bool isWatchedVid);
	static DialogRevive* create(int numberOfRevive, bool isWatchedVid);

private:
	int calGoldRevive(int number);
};

class DialogStageClear : public DialogPauseGame
{
private:
	Label *bonusGoldLb;
public:
	bool init(int score, int gold);
	static DialogStageClear* create(int score, int gold);
	void effect();
	void onExit();

private:
	void shareFB();
};

class DialogOverGame : public DialogStageClear
{

public:
	bool init(int score, int gold);
	static DialogOverGame* create(int score, int gold);
	void effect();
	void onExit();
};


#endif // __DIALOG_H__
