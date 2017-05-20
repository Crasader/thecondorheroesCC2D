#ifndef __SELECT_STAGE_SCENE_H__
#define __SELECT_STAGE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SelectStageLayer : public Layer
{
public:
	virtual bool init(int charId);
	static SelectStageLayer* create(int charId);
	void moveAva();

private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	int m_nLifeNumber;
	int m_nTimeAnchor;

	TMXTiledMap *tmxMap;
	ui::ScrollView* scrollView;
	Point nextMapPos;

	void gotoPlay(int id, int stage, int map, int charId);

	Sprite *character_point;
	Sprite* bossSprite(int order);
	void doNothing();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onExit();
	int convertId();
};

#endif // __SELECT_STAGE_SCENE_H__