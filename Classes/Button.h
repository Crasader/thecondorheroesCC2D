#ifndef __BUTTON_LEFT_H__
#define __BUTTON_LEFT_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;

class Button : public Sprite
{
public:
	Button();
	static Button* create(string file_name, Point pos);

private:
	void addEvents();
	void checkInterval(float dt);
	bool canTouch;

protected:
	CC_SYNTHESIZE(float, timeCoolDown, TimeCoolDown);
	CC_SYNTHESIZE(bool, isActive, IsActive);
};

#endif // __BUTTON_LEFT_H__
