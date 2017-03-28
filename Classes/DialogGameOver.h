#ifndef __DIALOG_GAME_OVER_H__
#define __DIALOG_GAME_OVER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class DialogGameOver : public Layer
{
public:
	virtual bool init();
	static DialogGameOver* create(int coin, int score);

	
private:
	int coin;
	int score;
	Sprite* background;

protected:
	CC_SYNTHESIZE(MenuItemImage*, backBtn, BackBtn);
	CC_SYNTHESIZE(MenuItemImage*, restartBtn, RestartBtn);
	CC_SYNTHESIZE(MenuItemImage*, shareMe, ShareMe);
	CC_SYNTHESIZE(Label*, myScore, MyScore);
	CC_SYNTHESIZE(Label*, myCoin, MyCoin);
	CC_SYNTHESIZE(Label*, bonus, Bonus);

private:
	void backToStage(Ref* pSender);
	void restartMe(Ref* pSender);
	void sharing(Ref* pSender);
	void showResult();

};

#endif // __DIALOG_GAME_OVER_H__