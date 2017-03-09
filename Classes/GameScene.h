#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hud.h"
#include "DuongQua.h"
#include "Global.h"
#include "GLES-Render.h"
#include "EnemyWooder.h"
#include "EnemyToanChanStudent.h"
#include "Coin.h"
#include "CollisionListener.h"
#include "InfiniteParallaxNode.h"


USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();

private:

	// props
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scaleOfMap;
	TMXTiledMap* tmx_map;

	b2World *world;
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

	Follow *camera;
	Node* follow;
	CCRect left_corner;


	BaseHero *hero;
	InfiniteParallaxNode *background;

	// Create Function
	void createDuongQua(string path_Json, string path_Atlas, Point position);


	// function for process box2d
	void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags);
	void onDraw();
	void initB2World();
	void updateB2World(float dt);

	/**
	* Only create ground physic                                                                     
	*/
	void initGroundPhysic(b2World *world, Point pos, Size size);

	// function for process map
	void loadBackground();
	void createInfiniteNode();
	void createGroundBody();
	void creatEnemyWooder();
	void creatEnemyToanChanStudent();


	void createCoint();
	void createTimCoin();
	void createParapolCoin();
	void createCircleCoin();

	// sau va cham body cua cac quai khong con static nua
	// do do se bi roi xuong
	// bat su kien roi xuong qua man hinh de don dep map
	

	void danceWithCamera();

	// read file Json
	void readWriteJson();

	// touch listener
	bool onTouchBegan(Touch *touch, Event *unused_event);

public:
    
	// update functions
	void listener();		// attack button listener | see update function
	void update(float dt);
	void updateEnemy();
	//void cleanMap();
	// cache function
	void cachePlist();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
