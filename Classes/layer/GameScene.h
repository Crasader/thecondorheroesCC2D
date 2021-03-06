#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Global.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"

#include "EnemyTNB.h"
#include "EnemyToOng.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"
#include "EnemyHoacDo.h"
#include "EnemyHoacDo2.h"
#include "EnemyDatNhiBa.h"
#include "EnemyDatNhiBa2.h"
#include "EnemyBoss1.h"
#include "EnemyBoss2.h"
#include "EnemyBoss3.h"
#include "ChimDieu.h"
#include "Coin.h"
#include "CoinBag.h"
#include "CoinBullion.h"
#include "CollisionListener.h"
#include "InfiniteParallaxNode.h"
#include "GLES-Render.h"
#include "DialogPauseGame.h"
#include "TutorialLayer.h"
#include "MyData.h"
#include "MyPool.h"
#include "MyLayer.h"
#include "Item.h"
#include "Hud.h"
#include "EnemyLinhTenThang.h"
#include "EnemyLinhTenXien.h"
#include "VungleHelper.h"


#ifdef SDKBOX_ENABLED
class GameScene : public cocos2d::Layer, public sdkbox::VungleListener
#else
class GameScene : public cocos2d::Layer
#endif
{
public:
	
	static Scene* createScene(GameScene* gameLayer, Hud *m_hud);
	virtual bool init(int stage, int map, int charId);
	static GameScene* create(int stage, int map, int charId);

	BaseHero * getHero() { return hero; }
	Node* getFollow() { return follow; }
	void setLastScore(int lastScore) { m_lastScore = lastScore; }
	CC_SYNTHESIZE(Hud*, hud, Hud);
	void enableCalling();

private:

	bool isModeDebug;
	
	// props
	int stage;
	int map;
	int haveboss;
	float changebg;
	Point heroStartPosition;


	int charId;	//Thinhnv Edited for select character
	int numberRevive = 0;

	bool isWinGame;

	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scaleOfMap;

	int numberDeadItem = 0;

	int indexOfNextMapBoss;// chi so cua map boss cuoi, khoi dau la -1, khi danh boss chuyen 1 va 0(0101010101)
	int currentButton = 0;
	float posXComingBoss = -1;

	int m_lastScore = 0;
	int m_nMultiKills = 0;
	float m_fMultiKillsCounterTime;


	int previousGold = 0;
	int previousScore = 0;

	float previousPercentPosition = 0.0f;
	
public:
	b2World *world;


private:
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

	Follow *camera;
	Node* follow;
	CCRect left_corner;
	SpriteBatchNode* batchNode;

	BaseHero *hero;
	ChimDieu* _aEagle;

	InfiniteParallaxNode *background;
	InfiniteParallaxNode *background2;

	list<Item*> listItem;

	MyLayer * preLayer;
	MyLayer * posLayer;

	TMXTiledMap* tmx_map;
	TMXTiledMap *tmx_mapboss[2];
	vector<MyData> listPosAndTag;

	// dialog here
	DialogPauseGame* dialogPause;

	// listener
	EventListenerTouchOneByOne* touch_listener;

	void selectHero();

	// Create For Hero
	void createDuongQua(string path_Json, string path_Atlas);
	void createCoLong(string path_Json, string path_Atlas);
	void createHoangDung(string path_Json, string path_Atlas);
	void createQuachTinh(string path_Json, string path_Atlas);
	void createEagle(Point position);
	void heroGetOffEagle();


	// Create Fro Enemies
	
	void createAgentOnLayer(MyLayer* layer);
	void creatAgentByMydata(MyLayer* layer, MyData data);

	void creatEnemyWooder(MyLayer* layer, Vec2 pos);
	void createEnemyToanChanStudent(MyLayer* layer, Vec2 pos);
	void createEnemyToanChanStudent2(MyLayer* layer, Vec2 pos);
	void createEnemyTNB(MyLayer* layer, Vec2 pos);
	void createEnemyHongLangBa(MyLayer* layer, Vec2 pos);
	void createEnemyHongLangBa2(MyLayer* layer, Vec2 pos);
	void createEnemyToOng(MyLayer* layer, Vec2 pos);
	void createEnemyHoacDo(MyLayer* layer, Vec2 pos);
	void createEnemyHoacDo2(MyLayer* layer, Vec2 pos);
	void createEnemyDatNhiBa(MyLayer* layer, Vec2 pos);
	void createEnemyDatNhiBa2(MyLayer* layer, Vec2 pos);
	void createEnemyChong1(MyLayer * layer, Vec2 pos);
	void createEnemyChong2(MyLayer * layer, Vec2 pos);
	void createEnemyChong3(MyLayer * layer, Vec2 pos);
	void createEnemyLinhCamGiao(MyLayer * layer, Vec2 pos);
	void createEnemyLinhCamGiao2(MyLayer * layer, Vec2 pos);
	void createEnemyLinhTenThang(MyLayer * layer, Vec2 pos);
	void createEnemyLinhTenXien(MyLayer * layer, Vec2 pos);
	void createEnemyLinhCamRoi(MyLayer * layer, Vec2 pos);
	void creatBoss();


	// function for process box2d
	void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags);
	void onDraw();
	void initB2World();
	void updateB2World(float dt);

	/**
	* Only create ground physic                                                                     
	*/
	void initGroundPhysic(b2World *world, Point pos, Size size);
	void initUnderGroundPhysic(b2World *world, Point pos, Size size);

	// function for process map
	void loadBackground();
	void createInfiniteNode();
	void createGroundBody();
	void createGroundForMapBoss();

	// toi uu cho game
	void loadPosAndTag();	// luu tru cac vi tri sinh agent
	void loadPosOfObjectInGroup(string nameOfGroup, float tag);// 
	void initLayerToAddAgent();
	

	// Coin and item
	void createCoin();
	void createCointBag();
	void createCoinBullion();
	void createFormCoin(string objectName, string objectMap, string objectInform);
	void createItem();
	

	// update private
	void updateQuest();
	void checkActiveButton();
	void listener();
	void update(float dt);
	void updateEnemy();
	void updateHUD(float dt);
	void updateCamera();
	void updateChangeBg();
	void updateCoin();
	void updateCharacterPoint();
	void updateLayer();

	// Utils
	void reachNewMap();
	void danceWithCamera();


	// touch listener
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:
	void onBegin();
	
	// update public
	void updateMultiKills();		//DuongPM edited for multi kills
	void runnerItem(Item_type type, int counter);
	void updateBloodBar(int numberOfHealth, bool isVisible);

	// shaking
	void shakeTheScreen();
	void reviveHero();
	void callingBird();

	void blurScreen();
	void pauseGame();
	void dieGame();
	void overGame();
	void winGame();
	void resumeGame();
	void restartGame();
	


	// TUTORIAL
private:
	TutorialLayer *tut;
	bool isFirstPlay = false;

	float posXJump1Tut = -1;
	float posXJump2Tut = -1;
	float posXAttackTut = -1;
	float posXSkillTut = -1;
	float posXIntroBird = -1;

	void introJump(int type);
	void introAttack();
	void introSkills();
	void introBird();

	void tutorial();

public:

	void jump();
	void resumeAfterTut(int caseTut);

#ifdef SDKBOX_ENABLED

	 void onVungleAdViewed(bool isComplete);
	 void onVungleCacheAvailable();
	 void onVungleStarted();
	 void onVungleFinished();
	 void onVungleAdReward(const std::string& name);
#endif 
};

#endif // __GAME_SCENE_H__
