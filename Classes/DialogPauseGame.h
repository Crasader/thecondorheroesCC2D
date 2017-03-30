#ifndef __DIALOG_PAUSE_GAME_H__
#define __DIALOG_PAUSE_GAME_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class DialogPauseGame : public Layer
{
public:
	virtual bool init(int type);
	static DialogPauseGame* create(int type); // -1 is pause, 0 is die, 1 is win

private:
	void initForPause();
	void initForDie();
	void initForWin();

	void backToPlay(Ref* pSender);
	void exitToStage(Ref* pSender);
	void replay(Ref* pSender);
	void nextState(Ref* pSender);

};

#endif // __DIALOG_GAME_OVER_H__