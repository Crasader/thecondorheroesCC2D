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
	bool accept = false;
protected:
	Sprite *main;
	//EventListenerTouchOneByOne * m_listener;

public:
	virtual bool init();
	static TutorialLayer* create();

	void showJump();
	void showAttack();
	void showSkills();
	void showBird();

private:
	void showTime(float dt);
};


#endif // __TUT_H__
