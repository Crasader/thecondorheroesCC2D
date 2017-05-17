#ifndef __SELECT_STAGE_SCENE_H__
#define __SELECT_STAGE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class SelectStageLayer : public Layer
{
public:

	static Scene* createScene(int charId);
	virtual bool init(int charId);
	static SelectStageLayer* create(int charId);

private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	int m_nLifeNumber;
	int m_nTimeAnchor;

	void initData();
	void gotoPlay(int id, int stage, int map, int haveBoss, int charId);

	void doNothing();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif // __SELECT_STAGE_SCENE_H__