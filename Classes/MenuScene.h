#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include <vector>

using namespace spine;
USING_NS_CC;
using namespace std;

class MenuLayer : public Layer
{
public:
	vector <Sprite*> listMap;
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool onTouchBegan(Touch *touch, Event *event);
	CREATE_FUNC(MenuLayer);
	void gotoPlay(int map);

};

#endif // __HUD_H__