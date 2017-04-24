#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include <vector>
#include <time.h>

USING_NS_CC;
using namespace spine;
using namespace std;
using namespace ui;

class MenuLayer : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	void update(float p_fDelta);
	CREATE_FUNC(MenuLayer);
private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	float m_fButtonStartPosition;
	bool m_bIsHeroLayout = false;

	//
	int m_nIndexHeroSelected = 0;
	int m_nIndexHeroPicked = 0;
	int m_arNumberItem[5];
	int m_nLifeNumber = 3;
	int m_nTimeAnchor = 1491987700;
	Label *m_pTimeCounter;

	//
	int m_nGold;
	int m_nDiamond;

	// layers
	Layer *m_pGameBackground;
	Layer *m_pGameScene;
	Layer *m_pGameControl;
	Layer *m_pTopMainMenu;
	Layer *m_pBottomMainMenu;
	Layer *m_pUpgradeBoard;
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
	SkeletonAnimation *m_arPreviewHero[5];

	// item number label
	Label *ar_pLabelNumberItem[5];
	// score bar
	Sprite *m_pLevelPoint;
	Label *m_pHealthPointLabel;
	Label *m_pBonusScoreLabel;
	Label *m_pBonusGoldLabel;


	void initInputData();
	void initBackgroundLayer();
	void initSceneLayer();
	void initControlLayer();

	void initTopMainMenu();
	void initBottomMainMenu();
	void initItemBoard();
	void initUpgradeBoard();
	void initHeroInfoBoard();
	void initBottomHeroMenu();

	void backFunction();

	// handle button
	void buttonBackHanle();
	void buttonStartHandle();
	void buttonAddLifeHandle();
	void buttonAddGoldHandle();
	void buttonAddDiamondHandle();
	void buttonHeroesHandle();

	// upgrade skill handle
	void upgradeSkill1();
	void upgradeSkill2();
	void upgradeSkill3();

	// hero menu
	void pickHeroFirst();
	void pickHeroSecond();
	void pickHeroThird();
	void pickHeroFourth();
	void pickHeroFifth();
	void tryButtonHandle();
	void unlockButtonHandle();
	void selectButtonHandle();

	// buy items
	void buyItem1();
	void buyItem2();
	void buyItem3();
	void buyItem4();
	void buyItem5();

	//
	void showMainMenu();

	void createLayerViaInput(Layer *p_pLayer, Size p_v2Size, Vec2 p_v2Position);
	void moveLayerViaDirection(Layer *p_pLayer, int p_nDirection);
};

#endif // __HUD_H__
