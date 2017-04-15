#ifndef __COIN_BAG_H__
#define __COIN_BAG_H__

#include "B2Skeleton.h"



class CoinBag : public B2Skeleton
{

public:
	CoinBag(spSkeletonData *data);
	CoinBag(string jsonFile, string atlasFile,float scale);
	static CoinBag* create(string jsonFile, string atlasFile, float scale);
	static CoinBag* create(string filename, float scale);
	//static CoinBag* create(string jsonFile, string atlasFile, float scale);
	SkeletonAnimation *effect;
	void updateMe(BaseHero* hero);
	void die();
	//void listener();
	void onExit();
	
};

#endif // __B2_SPRITE_H__