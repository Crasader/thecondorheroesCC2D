#ifndef __SKELETON_MANAGER_H__
#define __SKELETON_MANAGER_H__

#include "cocos2d.h"
#include "spine\spine-cocos2dx.h"


using namespace std;
USING_NS_CC;
using namespace spine;

class SkeletonManager
{
public:

	SkeletonManager();
	~SkeletonManager();

	static SkeletonManager* instance;
	static spSkeletonData* getSkeletonData(string filename);
	static SkeletonManager* getInstance();
	static void cacheSkeleton(string filename, float scale);
	static Map<std::string, SkeletonRenderer *> skeleRendererCache;

};


#endif // __JSON_MANAGER_H__

