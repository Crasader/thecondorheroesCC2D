#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hud.h"
#include "DuongQua.h"
#include "Global.h"
#include "GLES-Render.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();

private:

	// props
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scale;
	TMXTiledMap* tmx_map;

	b2World *world;
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

	Follow *camera;
	Node* follow;

	BaseHero *hero;

	// Create Function
	void createDuongQua(string path_Json, string path_Atlas, Point position);

	// Update Function
	void listener();
	void update(float dt);


	// function for process box2d
	void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags);
	void onDraw();
	void initB2World();
	void updateB2World(float dt);
	void initPhysic(b2World *world, Point pos, Size size);

	// function for process map
	void loadBackground();
	void createGroundBody();


	// read file Json
	void readWriteJson();




public:
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
