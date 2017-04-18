#ifndef __MY_LAYER_H__
#define __MY_LAYER_H__

#include "cocos2d.h"
#include <string.h>
#include "datastructures\MyPool.h"

USING_NS_CC;
using namespace std;

class MyLayer : public Layer
{
public:
	MyPool* wooderPool;
	MyPool* toanchan1Pool;
	MyPool* toanchan2Pool;

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	

	void refresh();

	CREATE_FUNC(MyLayer);


};

#endif // __LOADING_LAYER_H__