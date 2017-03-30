#ifndef __DIALOG_PAUSE_GAME_H__
#define __DIALOG_PAUSE_GAME_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class DialogPauseGame : public Layer
{
public:
	virtual bool init();
	static DialogPauseGame* create();
	
private:
	Sprite* background;

protected:
	CC_SYNTHESIZE(MenuItemImage*, backBtn, BackBtn);
	CC_SYNTHESIZE(MenuItemImage*, exitBtn, ExitBtn);

private:
	void backToPlay(Ref* pSender);
	void exitToStage(Ref* pSender);

};

#endif // __DIALOG_GAME_OVER_H__