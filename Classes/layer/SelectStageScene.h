#ifndef __SELECT_STAGE_SCENE_H__
#define __SELECT_STAGE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SelectStageLayer : public Layer
{
public:

	static Scene* createScene(int charId);
	virtual bool init(int charId);	
	static SelectStageLayer* create(int charId);
	void moveAva();

private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	int m_nLifeNumber;
	int m_nTimeAnchor;
	Label *m_pTimeCounter;
	Layer *m_pTopMainMenu;
	TMXTiledMap *tmxMap;
	ui::ScrollView* scrollView;
	Point nextMapPos;

	void initData();
	void initTopMainMenu();
	void gotoPlay(int id, int stage, int map, int charId);

	Sprite *character_point;
	Sprite* bossSprite(int order);
	void goBack();
	void doNothing();
	void buttonAddLifeHandle();
	void buttonAddGoldHandle();
	void buttonAddDiamondHandle();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onExit();
};

#endif // __SELECT_STAGE_SCENE_H__