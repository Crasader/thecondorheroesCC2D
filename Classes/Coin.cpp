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

	Animation* animation = Animation::createWithSpriteFrames(aniframes, 0.08f);
	animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
}

void Coin::picked()
{
	this->setVisible(false);
	effect = SkeletonAnimation::createWithFile("Effect_getgolden.json","Effect_getgolden.atlas",SCREEN_SIZE.height/3/291);
	effect->setAnimation(0,"Effect_coin", false);
	effect->setPosition(this->getPosition());
	this->getParent()->addChild(effect);
	CallFunc *call = CallFunc::create([&]() {
		effect->removeFromParentAndCleanup(true);
		this->removeFromParentAndCleanup(true);
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), call));
	//this->listener();
}

void Coin::listener()
{
	effect->setCompleteListener([&](int trackIndex, int loopCount) {
		if ((strcmp(effect->getCurrent()->animation->name, "Effect_coin") == 0) && loopCount == 1) {
			//effect->removeFromParentAndCleanup(true);
			//this->removeFromParentAndCleanup(true);
		}
	});
}

void Coin::onExit()
{
	B2Sprite::onExit();
	auto world = this->getB2Body()->GetWorld();
	world->DestroyBody(this->getB2Body());
	//this->removeFromParentAndCleanup(true);
	log("delete coin");
}
