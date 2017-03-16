#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

using namespace spine;
USING_NS_CC;

class MenuLayer : public Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MenuLayer);
	void gotoPlay(Ref* pSender);

};

#endif // __HUD_H__