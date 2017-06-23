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

protected:
	CC_SYNTHESIZE(ui::ScrollView*, scrollView, mScrollView);
	CC_SYNTHESIZE(Menu*, menu, mMenu);

private:

	TMXTiledMap *tmxMap;
	Point nextMapPos, mapPlayPos;

	int stage;
	int map;
	int charId;

	void gotoPlay(int id, int stage, int map, Point point);

	Sprite *character_point;
	Sprite* bossSprite(int order);
	void doNothing();
	//int convertId();

	void createCloud();
};

#endif // __SELECT_STAGE_SCENE_H__