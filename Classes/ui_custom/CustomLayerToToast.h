#ifndef __CUSTOM_LAYER_TO_TOAST_H__
#define __CUSTOM_LAYER_TO_TOAST_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;
using namespace std;
using namespace ui;

#define TOAST_LONG			3.0f
#define TOAST_SHORT			1.0f

class CustomLayerToToast : public Layer {
private:
	Label *m_pLabelMessage;
	Sprite *m_pToastBackground;
	float m_fMessageOpacity;
	float m_fBackgroundOpacity;
	float m_fTimeExist;
public:
	CustomLayerToToast();
	~CustomLayerToToast();
	void update(float p_fDelta);
	static CustomLayerToToast * create(string p_sMessage, float p_fTimeOption);
	void setMessage(string p_sMessage, float p_fTimeOption);

};

#endif // __CUSTOM_LAYER_TO_TOAST_H__