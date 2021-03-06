#ifndef __MY_LAYER_H__
#define __MY_LAYER_H__

#include "cocos2d.h"
#include <string.h>
#include "MyPool.h"


USING_NS_CC;
using namespace std;

class MyLayer : public Layer
{
public:
	MyLayer();
	~MyLayer();
	MyPool* wooderPool;
	MyPool* toanchan1Pool;
	MyPool* toanchan2Pool;
	MyPool* toOngPool;
	MyPool* hongLangBa1Pool;
	MyPool* hongLangBa2Pool;
	MyPool* tnbPool;
	MyPool* hoacDo1Pool;
	MyPool* hoacDo2Pool;
	MyPool* datNhiBa1Pool;
	MyPool* datNhiBa2Pool;
	MyPool* chong1Pool;
	MyPool* chong2Pool;
	MyPool* chong3Pool;
	MyPool* linhcamgiao1Pool;
	MyPool* linhcamgiao2Pool;
	MyPool* linhtenthangPool;
	MyPool* linhtenxienPool;
	MyPool* linhcamroiPool;

	virtual bool init(TMXTiledMap * tmx_map);
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	

	void refresh();

	static MyLayer* create(TMXTiledMap* tmx_map);
};

#endif // __MY_LAYER_H__

