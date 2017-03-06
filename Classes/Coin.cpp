#include "Coin.h"

Coin::Coin()
{
}

Coin::~Coin()
{
}

Coin * Coin::create()
{
	Coin* coin = new Coin();
	coin->initWithSpriteFrameName("coin_01.png");
	coin->setTag(TAG_COIN);
	return coin;
}

void Coin::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_staticBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
}

void Coin::runAnimation()
{
	Vector <SpriteFrame*> aniframes;
	for (int i = 1; i < 9; i++)
	{
		string frameName = StringUtils::format("coin_0%d.png", i);
		//log(frameName.c_str());
		aniframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}

	Animation* animation = Animation::createWithSpriteFrames(aniframes, 0.1f);
	animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
}
