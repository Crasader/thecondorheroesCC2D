#ifndef __BATCHNODE_MANAGER_H__
#define __BATCHNODE_MANAGER_H__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class BatchNodeManager
{
public:

	BatchNodeManager();
	~BatchNodeManager();

	static BatchNodeManager* instance;
	static SpriteBatchNode* getSkeletonData(string filename);
	static BatchNodeManager* getInstance();
	static void cacheSkeleton(string filename, float scale);
	static Map<std::string, BatchNodeManager *> batchCache;

};



#endif // __JSON_MANAGER_H__