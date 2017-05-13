#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include <vector>
#include <time.h>
#include "network/HttpClient.h"

#include "ui_custom/CustomSpriteToBuyPack.h"
#include "ui_custom/CustomLayerToToast.h"

USING_NS_CC;
using namespace spine;
using namespace std;
using namespace ui;
using namespace network;

class MenuLayer : public Layer {
public:
	static Scene * createScene();
	virtual bool init();
	void update(float p_fDelta);
	CREATE_FUNC(MenuLayer);
private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	float m_fButtonStartPosition;							// make start and unlock button at same position
	int m_nMenuStatus = 0;

	// input value
	int m_nCurrentTimeFromGoogle = 0;						// time from google.com.vn (-7 hours from Viet Nam)

	int m_nIndexHeroSelected = 0;
	int m_nIndexHeroPicked = 0;
	int m_nLifeNumber;										// number of life
	int m_nAnchorTime;										// time number of life was changed by time
	int m_nCurrentGold;										// current gold
	int m_nCurrentDiamond;									// current diamond
	int m_arNumberItemOwning[5];							// number of item owning
	int m_arItemPrice[5];									// cost of items
	int m_nShopOption = 0;

	Label *m_pTimeCounter;									// time counter to increase life
	MenuItemSprite *m_arHeroButton[5];						// hero mini icon
	SkeletonAnimation *m_arPreviewHero[5];					// hero preview
	Label *m_arLabelNumberItemOwning[5];					// 
	MenuItemSprite *m_arBuyItemButton[5];
	Sprite *m_arSpriteItemMax[5];

	SkeletonAnimation *m_pSpriteQuestAttention;						// 
	SkeletonAnimation *m_pSpriteFreeCoinAttention;						// 

	Label *m_pLabelNumberGoldOnBuy;
	Label *m_pLabelNumberDiamondOnBuy;
	ScrollView *m_pPacksZone;
	ScrollView *m_pItemScrollView;
	// score bar
	/*Sprite *m_pLevelPoint;
	Label *m_pHealthPointLabel;
	Label *m_pBonusScoreLabel;
	Label *m_pBonusGoldLabel;*/

	// layers
	Layer *m_pGameBackground;
	Layer *m_pGameScene;
	Layer *m_pGameControl;
	Layer *m_pTopMainMenu;
	Layer *m_pBottomMainLayer;
	Layer *m_pUpgradeBoard;
	Layer *m_pQuestBoard;
	Layer *m_pItemBoard;
	Layer *m_pHeroInfoBoard;
	Layer *m_pBottomHeroLayer;
	Layer *m_pShopBoardLayer;

	Layer *m_pBlurScreen;

	// menus
	Menu *m_pTopMenu;
	Menu *m_pBottomMainMenu;
	Menu *m_pItemBoardMenu;
	Menu *m_pQuestBoardMenu;
	Menu *m_pSkillBoardMenu;
	Menu *m_pBottomHeroMenu;

	//////////////////////////////////////////////////////////////////////////////////
	// INIT
	void initInputData();
	void initBackgroundLayer();
	void initSceneLayer();
	void initControlLayer();

	void initTopMainMenu();
	void initBottomMainMenu();
	void initItemBoard(float p_fPercent);
	void initUpgradeBoard();
	void initQuestBoard(int p_nFocus);
	void initHeroInfoBoard();
	void initBottomHeroMenu();
	void initDailyRewardBoard();

	void initShopBoard(int p_nOption);

	void backFunction();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// HANDLE
	// handle button
	void buttonBackHandle();
	void buttonAddLifeHandle();
	void buttonAddGoldHandle();
	void buttonAddDiamondHandle();

	void buttonQuestHandle();
	void buttonHeroesHandle();
	void buttonShopHandle();
	void buttonFreeCoinHandle();
	void buttonMoreGameHandle();

	void buttonStartHandle();

	void buttonDailyRewardHandle();

	// upgrade skill handle
	void buttonUpgradeSkillHandle(int p_nIndexSkill);

	// hero menu
	void buttonPickHeroHandle(int p_nIndexHero);
	void buttonTryHeroHandle();
	void buttonUnlockHeroHandle();
	void buttonUpgradeHeroHandle();

	// buy items
	void buttonBuyItemHandle(int p_nIndexItem);

	// reward quest
	void buttonRewardQuest(int p_nQuestIndex);

	// daily reward
	bool createRequestToGoogle();														// send a request to google.com.vn to get data
	void onHttpRequestCompleted(HttpClient *p_pSender, HttpResponse *p_pResponse);		// handle response an get realtime from google.com.vm

																						// buy coin
	void buttonBuyLifeHandle(int p_nIndexEnergyPack);
	void buttonBuyCoinHandle(int p_nIndexCoinPack);
	void buttonBuyDiamondHandle(int p_nIndexDiamondPack);
	void buttonCloseShopHandle();

	// TODO: check quest reward;

	///////////////////////////////////////////////////////////////////////////////////////
	// 
	// create layer and move them
	void showMainMenu();
	void hideMainMenu();
	void showBlurScreen();
	void hideBlurScreen();
	void createLayerViaInput(Layer *p_pLayer, Size p_v2Size, Vec2 p_v2Position);
	void moveLayerViaDirection(Layer *p_pLayer, int p_nDirection);

	// supporter
	int calTimeFromString(string p_sInputString);
};

#endif // __MENUSCENE_H__