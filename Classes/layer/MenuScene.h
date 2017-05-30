#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include <vector>
#include <time.h>
#include "network/HttpClient.h"

#include "CustomLayerToToast.h"
#include "SelectStageScene.h"
#include "GoogleAnalysticHelper.h"

USING_NS_CC;
using namespace spine;
using namespace std;
using namespace ui;
using namespace network;

class MenuLayer : public Layer {
public:
	virtual bool init(bool p_bOnlySelectStage);
	void update(float p_fDelta);
	static MenuLayer* create(bool p_bOnlySelectStage);
private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	float m_fButtonStartPosition;							// make start and unlock button at same position
	int m_nMenuStatus = 0;

	// input value
	int m_nCurrentTimeFromGoogle = 0;						// time from google.com.vn (-7 hours from Viet Nam)

	int m_nIndexHeroSelected = 0;
	int m_nIndexHeroPicked = 0;
	int m_nLifeNumber;										// number of life
	int m_nCurrentGold;										// current gold
	int m_nCurrentDiamond;									// current diamond
	int m_arNumberItemOwning[5];							// number of item owning
	int m_arItemPrice[5];									// cost of items
	int m_nShopOption = 0;
	int m_nLanguage = 0;

	Label *m_pTimeCounter;									// time counter to increase life
	MenuItemSprite *m_arHeroButton[5];						// hero mini icon
	SkeletonAnimation *m_arPreviewHero[5];					// hero preview
	Label *m_arLabelNumberItemOwning[5];					// 
	MenuItemSprite *m_arBuyItemButton[5];
	Sprite *m_arSpriteItemMax[5];
	Sprite *m_arItemCoinSprite[5];
	Label *m_arItemLabelCost[5];

	SkeletonAnimation *m_pSpriteQuestAttention;						// 
	SkeletonAnimation *m_pSpriteFreeCoinAttention;						// 

	ListView *m_pPacksZone;
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

	SelectStageLayer *m_pSelectStageLayer = nullptr;
	Layer *m_pBlurScreen;
	LayerColor *m_pBuyPackConfirmBackground;

	// menus
	Menu *m_pTopMenu;
	Menu *m_pBottomMainMenu;
	Menu *m_pItemBoardMenu;
	Menu *m_pQuestBoardMenu;
	Menu *m_pSkillBoardMenu;
	Menu *m_pBottomHeroMenu;
	Menu *m_pShopMenu;
	RadioButtonGroup *m_pLanguageButtonGroup;

	//////////////////////////////////////////////////////////////////////////////////
	// INIT
	void initInputData();
	void initBackgroundLayer();
	void initSceneLayer();
	void initControlLayer();

	void initTopMainMenu();
	void initBottomMainMenu();
	void initItemBoard();
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
	void buttonSettingHandle();
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
	void buttonConfirmHandle(bool p_bConfirm, int p_nIndexPack);
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
	void scrollSlideHandle(Ref* sender, ScrollView::EventType type);
	void scrollShopHandle(Ref* sender, ScrollView::EventType type);
	void selectedItemEvent(Ref* sender, ListView::EventType type);
	void onChangedLanguage();
	void buttonSoundControlHandle(Ref* p_pSender);
	void buttonMusicControlHandle(Ref* p_pSender);
	// for google analytic
	void logButtonClickEvent(string button);
	void logBuyItemEvent(string item);
	void logUnlockHeroEvent(int indexhero);
	void logClickHeroEvent(int indexhero);
	void logTryHeroEvent(int indexhero);
	void logUpgradeHeroEvent(int indexhero, int level);
	void logUpgradeSkillEvent(int indexhero, int indexskill, int level);
	//void logUpgradeSkillEvent(int indexhero, int indexskill, int level);
private:
		string indexHeroToName(int indexHero);
};

#endif // __MENUSCENE_H__
