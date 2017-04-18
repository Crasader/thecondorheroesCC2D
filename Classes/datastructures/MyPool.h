#pragma once
#include "cocos2d.h"
#include <vector>
USING_NS_CC;

class MyPool
{
public:
	int indexOfPool;// current index of pool
	int maxPool;// max size of pool
	int tag;
	Ref**pool;
	MyPool();
	~MyPool();
	MyPool(int maxPool, int tag);
	static MyPool* create(int maxPool,int tag);
	Ref* getObject();
	
};

