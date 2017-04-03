#ifndef __DIALOG_PAUSE_GAME_H__
#define __DIALOG_PAUSE_GAME_H__

#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;


class DialogPauseGame : public Layer
{
public:
	ui::LoadingBar *loading;
	int countDown = 50;
	virtual bool init(int type);
	static DialogPauseGame* create(int type); // -1 is pause, 0 is die, 1 is win

public:
	void initForPause();
	void initForDie();
	void initForGameover();
	void initForWin();

	void resumeGame(Ref* pSender);
	void backHome(Ref* pSender);
	void replayGame(Ref* pSender);
	void nextState(Ref* pSender);

};

#endif // __DIALOG_GAME_OVER_H__