#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include <vector>

USING_NS_CC;
using namespace spine;
using namespace std;
using namespace ui;

class MenuLayer : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(MenuLayer);
private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	float m_fButtonStartPosition;
	bool m_bIsHeroLayout = false; //
	int m_nGold = 0; //

	//
	int m_nIndexHeroSelected = 0;
	int m_nIndexHeroPicked = 0;

	// layers
	Layer *m_pGameBackground;
	Layer *m_pGameScene;
	Layer *m_pGameControl;
	Layer *m_pTopMainMenu;
	Layer *m_pBottomMainMenu;
	Layer *m_pUpgrateBoard;
	Layer *m_pItemBoard;
	Layer *m_pHeroInfoBoard;
	Layer *m_pBottomHeroMenu;

	// buttons
	MenuItemSprite *m_pHeroButton1;
	MenuItemSprite *m_pHeroButton2;
	MenuItemSprite *m_pHeroButton3;
	MenuItemSprite *m_pHeroButton4;
	MenuItemSprite *m_pHeroButton5;

	// preview
	SkeletonAnimation *m_pPreviewHero0;
	SkeletonAnimation *m_pPreviewHero1;
	SkeletonAnimation *m_pPreviewHero2;
	SkeletonAnimation *m_pPreviewHero3;
	SkeletonAnimation *m_pPreviewHero4;

	void initInputData();
	void initBackgroundLayer();
	void initSceneLayer();
	void initControlLayer();

	void initTopMainMenu();
	void initBottomMainMenu();
	void initItemBoard();
	void initUpgrateBoard();
	void initHeroInfoBoard();
	void initBottomHeroMenu();

	void backFunction();

	void buttonBackHanle();
	void buttonStartHandle();
	void buttonHeroesHandle();

	// hero menu
	void pickHeroFirst();
	void pickHeroSecond();
	void pickHeroThird();
	void pickHeroFourth();
	void pickHeroFifth();
	void tryButtonHandle();
	void unlockButtonHandle();
	void selectButtonHandle();

	//
	void showMainMenu();

	void createLayerViaInput(Layer *p_pLayer, Size p_v2Size, Vec2 p_v2Position);
	void moveLayerViaDirection(Layer *p_pLayer, int p_nDirection);
};

#endif // __HUD_H__
