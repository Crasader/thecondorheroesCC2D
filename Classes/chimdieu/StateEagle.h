#ifndef __STATE_EAGLE_H__
#define __STATE_EAGLE_H__

#include "B2Skeleton.h"
#include "B2Sprite.h"

USING_NS_CC;

using namespace spine;

class ChimDieu;

class StateEagle {
public:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	StateEagle();
	~StateEagle();
	virtual void noCarry(ChimDieu *eagle);
	virtual void carry(ChimDieu *eagle);
};

class EagleFly : public StateEagle {
public:
	EagleFly();
	~EagleFly();
	void noCarry(ChimDieu *eagle);
	void carry(ChimDieu *eagle);
};

#endif // __STATE_EAGLE_H__
