#ifndef __LOADING_LAYER_H__
#define __LOADING_LAYER_H__

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

class LoadingLayer : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	
	CREATE_FUNC(LoadingLayer);

protected:
	CC_SYNTHESIZE(Sprite*, boardTime, BoardTime);

	CC_SYNTHESIZE(Sprite*, leftDoor, LeftDoor);
	CC_SYNTHESIZE(Sprite*, rightDoor, RightDoor);

	CC_SYNTHESIZE(LoadingBar*, loading, Loading);
	CC_SYNTHESIZE(Sprite*, lbLoading, LbLoading);

	CC_SYNTHESIZE(Sprite*, avatarHero, AvatarHero);
	CC_SYNTHESIZE(Label*, lbGuide, LbGuide);

private:
	float percent = 0.0f;
	void addStuff();
	void doLoading();
	void doOpen();

public:
	void addGuide(string guideWhat);
};

#endif // __LOADING_LAYER_H__