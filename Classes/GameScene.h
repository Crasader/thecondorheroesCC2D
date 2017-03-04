#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hud.h"
#include "DuongQua.h"


USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

private:

	// Hero
	BaseHero *hero;



	// Properties
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();

	// Create Function
	void createDuongQua(string path_Json, string path_Atlas, Point position);

	// Update Function
	void listener();
	void update(float dt);





public:
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
