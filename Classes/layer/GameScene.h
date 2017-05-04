#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Global.h"
#include "colong/CoLong.h"
#include "duongqua/DuongQua.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"

#include "EnemyTNB.h"
#include "EnemyToOng.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"

#include "boss/EnemyBoss1.h"
#include "boss/EnemyBoss2.h"
#include "chimdieu/ChimDieu.h"
#include "coin/Coin.h"
#include "coin/CoinBag.h"
#include "coin/CoinBullion.h"
#include "CollisionListener.h"
#include "utils/InfiniteParallaxNode.h"
#include "utils/GLES-Render.h"
#include "layer/DialogPauseGame.h"
#include "layer/TutorialLayer.h"
#include "datastructures/MyData.h"
#include "datastructures/MyPool.h"
#include "layer/MyLayer.h"
#include "item/Item.h"
#include "layer/Hud.h"



class GameScene : public cocos2d::Layer
{
public:
	
	static Scene* createScene(int stage, int map, int haveboss, int charId);
	virtual bool init(int stage, int map, int haveboss, int charId);
	static GameScene* create(int stage, int map, int haveboss, int charId);

	BaseHero * getHero() { return hero; }
	void setLastScore(int lastScore) { m_lastScore = lastScore; }
	Hud* getHud();
	void enableCalling();

private:
	// props
	int stage;
	int map;
	int haveboss;
	float changebg;


	int charId;//Thinhnv Edited for select character
	int numberRevive = 0;

	bool isWinGame;

	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scaleOfMap;
	vector<MyData> listPosAndTag;

	int numberDeadItem = 0;
	list<Item*> listItem;

	MyLayer * preLayer;
	MyLayer * posLayer;

	TMXTiledMap* tmx_map;
	TMXTiledMap *tmx_mapboss[2];

	int indexOfNextMapBoss;// chi so cua map boss cuoi, khoi dau la -1, khi danh boss chuyen 1 va 0(0101010101)
	int currentButton = 0;
	float posXComingBoss = -1;
	
public:
	b2World *world;
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

private:
	Follow *camera;
	Node* follow;
	CCRect left_corner;
	LayerColor *blur;
	SpriteBatchNode* batchNode;

	BaseHero *hero;
	ChimDieu* _aEagle;

	int m_lastScore = 0;
	int m_nMultiKills = 0;
	float m_fMultiKillsCounterTime;


	int previousGold = 0;
	int previousScore = 0;

	InfiniteParallaxNode *background;
	InfiniteParallaxNode *background2;

	// skeleton cache
	spSkeletonData *sr_toanchan1;
	spSkeletonData *sr_wooder;
	//end skeleton cache


	// dialog here
	DialogPauseGame* dialogPause;

	// listener
	EventListenerTouchOneByOne* touch_listener;
	EventListenerKeyboard* key_listener;

	// Create Function
	void createDuongQua(string path_Json, string path_Atlas, Point position);
	void createCoLong(string path_Json, string path_Atlas, Point position);

	void createEagle(Point position);
	void heroGetOffEagle();


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
	void creatEnemyWooder(MyLayer* layer, Vec2 pos);
	void createEnemyToanChanStudent(MyLayer* layer, Vec2 pos);
	void createEnemyToanChanStudent2(MyLayer* layer, Vec2 pos);
	void createEnemyTNB(MyLayer* layer, Vec2 pos);
	void createEnemyHongLangBa(MyLayer* layer, Vec2 pos);
	void createEnemyHongLangBa2(MyLayer* layer, Vec2 pos);
	void createEnemyToOng(MyLayer* layer, Vec2 pos);
	void creatBoss();

	void createCoin();
	void createCointBag();
	void createCoinBullion();
	//void createFormCoin( MyLayer *layer,Vec2 pos, string objectMap, string objectInform, SpriteBatchNode* batchnode);
	void createFormCoin(string objectName, string objectMap, string objectInform);


	void createItem();

	//skeleton data
	//spSkeletonData* createSkeletonData(string atlasFile, string jsonFile);
	// sau va cham body cua cac quai khong con static nua
	// do do se bi roi xuong
	// bat su kien roi xuong qua man hinh de don dep map


	void danceWithCamera();
	float previousPercentPosition = 0.0f;
	void updateCharacterPoint();

	// touch listener
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:
	bool isModeDebug;
	void onBegin();
    
	// update functions
	void checkActiveButton();
	void listener();		// attack button listener | see update function
	void update(float dt);
	void updateEnemy();
	void updateBoss();

	void updateHUD(float dt);
	void updateMultiKills(); //DuongPM edited for multi kills
	void runnerItem(Item_type type, int counter);
	void updateBloodBar(int numberOfHealth, bool isVisible);
	void updateCamera();
	void updateChangeBg();

	void updateCoin();

	//void cleanMap();

	// cache function
	void cachePlist();
	void cacheSkeleton();


	// shaking
	void shakeTheScreen();
	void reviveHero();
	void callingBird();

	void blurScreen();
	void pauseGame();
	void dieGame();
	void overGame();
	void nextGame();
	void winGame();
	void resumeGame();
	void restartGame();

	// toi uu cho game
	void loadPosAndTag();// luu tru cac vi tri sinh agent
	void loadPosOfObjectInGroup(string nameOfGroup, float tag);// 
	void initLayerToAddAgent();
	void updateLayer();

	void createAgentOnLayer(MyLayer* layer);
	void creatAgentByMydata(MyLayer* layer, MyData data);

	// quan ly item
	//void createMapItem();// tao du lieu cho map item.

	

	// handle tuts

	// tut
private:
	TutorialLayer *tut;

	// tutorial
	bool isFirstPlay = false;

	float posXJump1Tut = -1;
	float posXJump2Tut = -1;
	float posXAttackTut = -1;
	float posXSkillTut = -1;
	float posXIntroBird = -1;

	void introJump();
	void introAttack();
	void introSkills();
	void introBird();

	void tutorial();

public:

	void jump();
	void resumeAfterTut(int caseTut);
};

#endif // __GAME_SCENE_H__
