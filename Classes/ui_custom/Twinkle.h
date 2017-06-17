#ifndef __TWINKLE_H__
#define __TWINKLE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;
using namespace std;
using namespace ui;

#define TOAST_LONG			3.0f
#define TOAST_SHORT			1.0f

class Twinkle : public Layer {
private:
	Sprite *m_pTwinkle;
	float m_fMessageOpacity;
	float m_fBackgroundOpacity;
	float m_fTimeExist;
public:
	Twinkle();
	~Twinkle();
	void update(float p_fDelta);
	static Twinkle * create(float p_fDelay);
	void initTwinkle(float p_fDelay);

};

#endif // __TWINKLE_H__