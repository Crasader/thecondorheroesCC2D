#ifndef __TWINKLE_H__
#define __TWINKLE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class Twinkle : public Sprite {
public:
	Twinkle();
	~Twinkle();
	static Twinkle * create();
	void initTwinkle();

};

#endif // __TWINKLE_H__