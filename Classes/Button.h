#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;

class Button : public Sprite
{
public:
	Button();
	static Button* create(string file_name_main, string file_name_CoolDown, Point pos);

private:
	EventListenerTouchOneByOne* listener;
	int timer;
	void checkInterval(float dt);
	void runTimer();

protected:
	CC_SYNTHESIZE(Label*, number, NumberCoolDown);
	CC_SYNTHESIZE(float, timeCoolDown, TimeCoolDown);
	CC_SYNTHESIZE(bool, isActive, IsActive);
	CC_SYNTHESIZE(bool, canTouch, CanTouch);
	CC_SYNTHESIZE(Sprite*, coolDown, CoolDownSprite);
	CC_SYNTHESIZE(Sprite*, effectCoolDown, EffectCoolDown);
	CC_SYNTHESIZE(bool, isBlocked, IsBlocked);

	void addEvents();
	void pauseListener();
};

#endif // __BUTTON_H__
