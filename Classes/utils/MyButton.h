#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include <string>

USING_NS_CC;
using namespace std;
using namespace spine;

class MyButton : public Sprite
{
public:
	MyButton();
	static MyButton* create(string file_name_main, string file_name_CoolDown, Point pos);

private:
	EventListenerTouchOneByOne* listener;
	float timer;
	void checkInterval(float dt);
	void runTimer();
	void runSpecialTimer();

protected:
	CC_SYNTHESIZE(Label*, number, NumberCoolDown);
	CC_SYNTHESIZE(Label*, numberUseLb, NumberUseLb);
	CC_SYNTHESIZE(float, timeCoolDown, TimeCoolDown);
	CC_SYNTHESIZE(bool, isActive, IsActive);
	CC_SYNTHESIZE(bool, canTouch, CanTouch);
	CC_SYNTHESIZE(int, numberOfUse, NumberOfUse);
	CC_SYNTHESIZE(int, numberOfUseHasNotUsedYet, NumberOfUseHasNotUsedYet);
	CC_SYNTHESIZE(ProgressTimer*, main, Main);
	CC_SYNTHESIZE(Sprite*, blocked, BlockedImg);
	CC_SYNTHESIZE(bool, isBlocked, IsBlocked);

	void addNumberOfUse(int number);
	void addEvents();
	void pauseListener();
	void refresh();
	EventListenerTouchOneByOne* getListener() { return listener; }
};

#endif // __BUTTON_H__
