#pragma once
#ifndef __INTRO_INTRO_SCENE_H__
#define __INTRO_INTRO_SCENE_H__

#include "cocos2d.h"
#include "MenuScene.h"

USING_NS_CC;

class SceneIntro : public cocos2d::Layer {
public:
	Size m_szVisibleSize = Director::getInstance()->getVisibleSize(); // get visible sizes
    static Scene* createScene();
    virtual bool init();
	CREATE_FUNC(SceneIntro);
	
	void update(float p_fDelta);
private:
	void updateFeather(Sprite *p_pFeather);
	bool checkOutOfScreen(Sprite *p_pFeather);
	void goToMainMenuScene(Ref* p_pSender);
};

#endif // __INTRO_INTRO_SCENE_H__
