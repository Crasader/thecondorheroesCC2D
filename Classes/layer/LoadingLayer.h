#ifndef __LOADING_LAYER_H__
#define __LOADING_LAYER_H__

#include "cocos2d.h"
#include <string.h>
#include "ui/CocosGUI.h"
#include "GameScene.h"
#include <time.h>

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

class LoadingLayer : public Layer
{
public:

	static Scene* createScene(int stage, int map, int haveboss, int charId);
	
	virtual bool init(int stage, int map, int haveboss, int charId);
	static LoadingLayer* create(int stage, int map, int haveboss, int charId);


	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

protected:
	CC_SYNTHESIZE(GameScene*, mainScene, MainScene);
	CC_SYNTHESIZE(Hud*, hud, HUD);
	CC_SYNTHESIZE(Sprite*, boardTime, BoardTime);

	CC_SYNTHESIZE(Sprite*, leftDoor, LeftDoor);
	CC_SYNTHESIZE(Sprite*, rightDoor, RightDoor);

	CC_SYNTHESIZE(LoadingBar*, loading, Loading);
	CC_SYNTHESIZE(Sprite*, lbLoading, LbLoading);

	CC_SYNTHESIZE(Sprite*, avatarHero, AvatarHero);
	CC_SYNTHESIZE(Label*, lbGuide, LbGuide);

private:

	// props
	int stage;
	int map;
	int haveboss;
	float charId;
	float percent = 0.0f;
	chrono::time_point<std::chrono::system_clock> start, end;
	int timer = 0;
	void addStuff();
	void doLoading();
	void doOpen();

public:
	void doProcess();
	void addGuide(string guideWhat);
};

#endif // __LOADING_LAYER_H__