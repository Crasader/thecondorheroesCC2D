#ifndef __INFINITE_PARALLAX_NODE_H__
#define __INFINITE_PARALLAX_NODE_H__

#include "cocos2d.h"

USING_NS_CC;

//using namespace spine;



class InfiniteParallaxNode : public ParallaxNode
{
public:
	enum StateParalax {
		UP,
		DOWN,
		FREE
	};
	StateParalax state;
	static InfiniteParallaxNode* create();
	void updatePosition();
	void up(Vec2 posHero);
	void down(Vec2 posHero);
	void free();
};

#endif
