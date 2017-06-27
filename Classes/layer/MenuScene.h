#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <spine/spine-cocos2dx.h>
#include <vector>
#include <time.h>
#include "network/HttpClient.h"

#include "ui_custom\CustomLayerToToast.h"
#include "SelectStageScene.h"

#include "thirdsdkhelper\GoogleAnalysticHelper.h"
#include "thirdsdkhelper\GoogleAnalysticHelper.h"
#include "thirdsdkhelper\IAPHelper.h"
#include "thirdsdkhelper\VungleHelper.h"
#include "thirdsdkhelper\SdkboxPlay.h"

USING_NS_CC;
using namespace spine;
using namespace std;
using namespace ui;
using namespace network;

//#ifndef __MENUSCENE_H__
//#define __MENUSCENE_H__
//
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include <spine/spine-cocos2dx.h>
//#include <vector>
//#include <time.h>
//#include "network/HttpClient.h"
//
//#include "CustomLayerToToast.h"
//#include "SelectStageScene.h"
//
//#include "GoogleAnalysticHelper.h"
//#include "GoogleAnalysticHelper.h"
//#include "IAPHelper.h"
//#include "VungleHelper.h"
//#include "SdkboxPlay.h"
//
//
//USING_NS_CC;
//using namespace spine;
//using namespace std;
//using namespace ui;
//using namespace network;

#ifdef SDKBOX_ENABLED
class MenuLayer : public cocos2d::Layer, public sdkbox::IAPListener, public sdkbox::VungleListener
#else
class MenuLayer : public cocos2d::Layer
#endif
{
public:
	virtual bool init(bool p_bOnlySelectStage, bool p_bGoToHeroesMenu = false);
	void update(float p_fDelta);
	static MenuLayer* create(bool p_bOnlySelectStage, bool p_bGoToHeroesMenu = false);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	bool downLife();
	void disableListener();

private:
	const Size m_szVisibleSize = Director::getInstance()->getVisibleSize();
	float m_fButtonStartPosition;							// make start and unlock button at same position
	int m_nMenuStatus = 0;
	EventListenerKeyboard* key_listener;


	// input value
	int m_nCurrentTimeFromGoogle = 0;						// time from google.com.vn (-7 hours from Viet Nam)
	int m_nTimeForLife = 3;

	int m_nIndexHeroSelected = 0;
	int m_nIndexHeroPicked = 0;
	int m_nLifeNumber;										// number of life
	int m_nCurrentGold;										// current gold
	int m_nCurrentDiamond;									// current diamond
	int m_arNumberItemOwning[5];							// number of item owning
	int m_arItemPrice[5];									// cost of items
	int m_nShopOption = 0;
	int m_nLanguage = 0;
	int backNumber = 0;
	//float m_nCakeScale;

	Label *m_pTimeCounter;									// time counter to increase life
	MenuItemSprite *m_arHeroButton[5];						// hero mini icon
	SkeletonAnimation *m_arPreviewHero[5];					// hero preview
	Label *m_arLabelNumberItemOwning[5];					// 
	Button *m_arBuyItemButton[5];
	Sprite *m_arSpriteItemMax[5];
	Sprite *m_arItemCoinSprite[5];
	Label *m_arItemLabelCost[5];
	Sprite *m_pReceviveDailyRewardSprite;
	Sprite *m_iconLife;

	SkeletonAnimation *m_pSpriteQuestAttention;						// 
	SkeletonAnimation *m_pSpriteBuyHeroAttention;						// 
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
	LayerColor *m_pShopBlurBackground;

	// menus
	Menu *m_pTopMenu;
	Menu *m_pBottomMainMenu;
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
	void buttonLeaderBoardHandle();
	void buttonFreeCoinHandle();
	void buttonSettingHandle();
	void buttonMoreGameHandle();

	void buttonStartHandle();

	void buttonDailyRewardHandle();
	void buttonConfirmDailyRewardHandle();

	// upgrade skill handle
	void buttonUpgradeSkillHandle(int p_nIndexSkill, int p_nCost);

	// hero menu
	void buttonPickHeroHandle(int p_nIndexHero);
	void buttonTryHeroHandle();
	void buttonUnlockHeroHandle();
	void buttonUpgradeHeroHandle();

	// buy items
	void buttonBuyItemHandle(int p_nIndexItem);
	void receiveButtonPressEvent(Ref *pSender, Widget::TouchEventType type, int p_nIndexItem);

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
	void loadTwinkle(TMXTiledMap *p_pSprite, float p_fMinScaleViaHeight, float p_fMaxScaleViaHeight);
	void buttonSpringy(MenuItemSprite *p_pButton);
	void showPopupInfoDialog(string p_sMessage, bool p_bGoToHeroesMenu = false);
	void GoToHeroesMenu();

	Sprite * createSpriteOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sPath, float p_fScaleX, float p_fScaleY, bool p_bScaleByWidth, Vec2 p_v2Anchor, Vec2 p_v2Position);
	MenuItemSprite * createButtonOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, string p_sPath, ccMenuCallback p_pCallback, float p_fScaleX, float p_fScaleY, bool p_bScaleByWidth, Vec2 p_v2Anchor, Vec2 p_v2Position);
	Label * createLabelBMOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sFontName, string p_sText, float p_fFontSize, TextHAlignment p_pHAlignment, TextVAlignment p_pVAlignment, Vec2 p_v2Anchor, Vec2 p_v2Position);
	Label * createLabelTTFOnParent(Layer *p_pLayerParent, Sprite *p_pSpriteParent, int p_nLayer, string p_sFontName, string p_sText, float p_fScale, bool p_bScaleByWidth, TextHAlignment p_pHAlignment, TextVAlignment p_pVAlignment, Vec2 p_v2Anchor, Vec2 p_v2Position, Color3B p_b3Color);

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
	void logShowMoreCoin();
	void logShowMoreDiamond();
	void logShowMoreLife();
	void logBuyCoin(int dexOfPack);
	void logBuyLife(int dexOfPack);
	void logBuyDiamond(int dexOfPack, float money);


	//void logUpgradeSkillEvent(int indexhero, int indexskill, int level);
#ifdef SDKBOX_ENABLED
	virtual void onInitialized(bool ok) override;
	virtual void onSuccess(sdkbox::Product const& p) override;
	virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
	virtual void onCanceled(sdkbox::Product const& p) override;
	virtual void onRestored(sdkbox::Product const& p) override;
	virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
	virtual void onProductRequestFailure(const std::string &msg) override;
	void onRestoreComplete(bool ok, const std::string &msg) override;

	void onVungleAdViewed(bool isComplete);
	void onVungleCacheAvailable();
	void onVungleStarted();
	void onVungleFinished();
	void onVungleAdReward(const std::string& name);

#endif // DEBUG

private:
	string indexHeroToName(int indexHero);
	void singlePress(float dt);
};

#endif // __MENUSCENE_H__

