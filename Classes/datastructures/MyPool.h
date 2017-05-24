#pragma once
#include "cocos2d.h"
#include <vector>
#include "EnemyToanChanStudent.h"
#include "EnemyToanChanStudent2.h"
#include "EnemyWooder.h"
#include "EnemyTNB.h"
#include "EnemyToOng.h"
#include "EnemyHongLangBa.h"
#include "EnemyHongLangBa2.h"
#include "EnemyHoacDo.h"
#include "EnemyHoacDo2.h"
#include "EnemyDatNhiBa.h"
#include "EnemyDatNhiBa2.h"
#include "EnemyChong1.h"
#include "EnemyChong2.h"
#include "EnemyChong3.h"
#include "EnemyLinhCamGiao.h"
#include "EnemyLinhCamGiao2.h"
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

