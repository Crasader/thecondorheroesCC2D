#ifndef __CUSTOM_SRPITE_TO_BUY_PACK_H__
#define __CUSTOM_SRPITE_TO_BUY_PACK_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;
using namespace std;
using namespace ui;

class CustomSpriteToBuyPack : public Sprite{
private:
	bool m_bTouchMoved;
	float m_fBigScale;
	float m_fSmallScale;
	ScrollView *m_pParent;
	ccMenuCallback m_pCallBack;
public:
	CustomSpriteToBuyPack();
	~CustomSpriteToBuyPack();
	static CustomSpriteToBuyPack * create(string p_sFilePath, ccMenuCallback p_pCallBack, ScrollView *p_pParent);

	void initOptions(ScrollView *p_pParent, ccMenuCallback p_pCallBack);
	void addEvents();
	bool touchBegan(Touch *p_pTouch, Event *p_pEvent);
	void touchCancelled(Touch *p_pTouch, Event *p_pEvent);
	void touchEnded(Touch *p_pTouch, Event *p_pEvent);
};

#endif // __CUSTOM_SRPITE_TO_BUY_PACK_H__