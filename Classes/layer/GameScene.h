#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "colong/CoLong.h"
#include "duongqua/DuongQua.h"
#include "Global.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"
#include "boss1/EnemyBoss1.h"
#include "manager/EffectManager.h"
#include "manager/JSonHeroManager.h"
#include "chimdieu/ChimDieu.h"
#include "coin/Coin.h"
#include "coin/CoinBag.h"
#include "coin/CoinBullion.h"
#include "CollisionListener.h"
#include "utils/InfiniteParallaxNode.h"
#include "utils/GLES-Render.h"

#include "layer/DialogPauseGame.h"


USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene(int map, int haveboss, int charId);
	virtual bool init(int map, int haveboss, int charId);
	static GameScene* create(int map, int haveboss, int charId);

	BaseHero * getHero() { return hero; }

private:
	// props
	std::map <std::string, bool> checkGenEnemy;
	int map;
	int haveboss;
	int charId;//Thinhnv Edited for select character
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scaleOfMap;
	TMXTiledMap* tmx_map;
	TMXTiledMap *tmx_mapboss[2];
	TMXObjectGroup* groupGroundWooder;
	TMXObjectGroup* groupGroundToanchan1;
	TMXObjectGroup* groupGroundToanchan2;

	int indexOfNextMapBoss;// chi so cua map boss cuoi, khoi dau la -1, khi danh boss chuyen 1 va 0(0101010101)
	int currentButton = 0;

public:
	b2World *world;
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

private:
	Follow *camera;
	Node* follow;
	CCRect left_corner;
	b2Body* sensor;
	LayerColor *blur;


	BaseHero *hero;
	ChimDieu* _aEagle;
	int m_nMultiKills = 0;
	float m_fMultiKillsCounterTime;

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
	void loadBackground(int map);
	void createInfiniteNode();
	void createGroundBody();
	void createGroundForMapBoss();
	void creatEnemyWooder();
	void creatEnemyToanChanStudent();
	void creatEnemyToanChanStudent2();
	void creatBoss();

	/*void creatEnemyWooderRT();
	void creatEnemyToanChanStudentRT();
	void creatEnemyToanChanStudent2RT();*/

	void createCoint();
	void createCointBag();
	void createCoinBullion();
	/*void createTimCoin();
	void createParapolCoin();
	void createCircleCoin();
	void createSquareCoin();
	void createStraightCoin();
	void createZigzagCoin();
	void createZigzagCoin2();*/
	void createFormCoin(string objectName, string objectMap, string objectInform);

	//skeleton data
	//spSkeletonData* createSkeletonData(string atlasFile, string jsonFile);
	// sau va cham body cua cac quai khong con static nua
	// do do se bi roi xuong
	// bat su kien roi xuong qua man hinh de don dep map
	
	void createSensorToDetectEnemy();
	void updateSensor();

	void danceWithEffect();
	void danceWithCamera();
	float previousPercentPosition = 0.0f;
	void updateCharacterPoint();

	// touch listener
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:

	void onBegin();
    
	// update functions
	void checkActiveButton();
	void listener();		// attack button listener | see update function
	void update(float dt);
	void updateEnemy();
	void updateBoss();

	void updateMoney(int numberOfCoin);
	void updateScore(int score);
	void updateMultiKills(); //DuongPM edited for multi kills
	void updateBloodBar(int numberOfHealth, bool isVisible);
	void updateCamera();
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

    // implement the "static create()" method manually
   // CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__