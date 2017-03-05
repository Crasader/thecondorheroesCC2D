#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hud.h"
#include "DuongQua.h"
#include "Global.h"
#include "GLES-Render.h"
#include "EnemyWooder.h"
#include "CollisionListener.h"

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
	void createGroundBody();
	void creatEnemyWooder();

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

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
