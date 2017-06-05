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

	static Scene* createScene(int stage, int map, int charId);
	
	virtual bool init(int stage, int map, int charId);
	static LoadingLayer* create(int stage, int map, int charId);


	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

protected:
	CC_SYNTHESIZE(GameScene*, mainScene, MainScene);
	CC_SYNTHESIZE(Hud*, hud, HUD);

	CC_SYNTHESIZE(LoadingBar*, loading, Loading);

private:

	// props
	int stage;
	int map;
	float charId;
	float percent = 0.0f;
	chrono::time_point<std::chrono::system_clock> start, end;
	int timer = 0;
	void addStuff();
	void doLoading();

public:
	void doProcess();
};

#endif // __LOADING_LAYER_H__