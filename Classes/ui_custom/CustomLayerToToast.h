#ifndef __CUSTOM_LAYER_TO_TOAST_H__
#define __CUSTOM_LAYER_TO_TOAST_H__

#include "cocos2d.h"
#include <string>
USING_NS_CC;
using namespace std;

#define TOAST_LONG			3.0f
#define TOAST_SHORT			1.5f

class CustomLayerToToast : public Label {
private:
	Sprite *m_nBackground;
	void fading(float p_fTimeOption);

public:
	CustomLayerToToast();
	~CustomLayerToToast();
	static CustomLayerToToast * create(string p_sMessage, float p_fTimeOption);

};

#endif // __CUSTOM_LAYER_TO_TOAST_H__