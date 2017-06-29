#ifndef __TUTORIAL_LAYER_H__
#define __TUTORIAL_LAYER_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class TutorialLayer : public Layer
{
public: 
	int type = 1;
protected:
	Sprite *main;
	//EventListenerTouchOneByOne * m_listener;

public:
	virtual bool init();
	static TutorialLayer* create();
	//virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	void showJump();
	void showAttack();
	void showSkills();
	void showBird();
};


#endif // __TUT_H__
