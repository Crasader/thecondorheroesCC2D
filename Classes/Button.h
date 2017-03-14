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
	
	void addEvents();
	void checkInterval(float dt);

protected:
	CC_SYNTHESIZE(float, timeCoolDown, TimeCoolDown);
	CC_SYNTHESIZE(bool, isActive, IsActive);
	CC_SYNTHESIZE(bool, canTouch, CanTouch);
	CC_SYNTHESIZE(Sprite*, coolDown, CoolDownSprite);
	CC_SYNTHESIZE(Sprite*, block, BlockSprite);
	CC_SYNTHESIZE(bool, isBlocked, IsBlocked);
};

#endif // __BUTTON_LEFT_H__
