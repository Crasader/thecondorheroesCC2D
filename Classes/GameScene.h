#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Global.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h" 

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
private:
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();
	float scale;
	TMXTiledMap* tmx_map;

	b2World *world;
	GLESDebugDraw *debugDraw;
	Mat4 _modelViewMV;
	CustomCommand _customCommand;

	Follow *camera;
	Node* follow;

    

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	// function for process box2d
	void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags);
	void onDraw();
	void initB2World();
	void updateB2World(float dt);
	void initPhysic(b2World *world, Point pos, Size size);

	// function for process map
	void loadBackground();
	void createGroundBody();

	


	void readWriteJson();

	void update(float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__
