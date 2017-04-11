
#include "CoinBag.h"

CoinBag::CoinBag(string jsonFile, string atlasFile, float scale) :B2Skeleton( jsonFile,  atlasFile,  scale)
{
}

CoinBag * CoinBag::create(string jsonFile, string atlasFile, float scale)
{
	auto coin = new CoinBag(jsonFile, atlasFile, scale);
	coin->update(0.0f);
	coin->setTag(TAG_COINBAG);
	coin->setAnimation(0,"Gold_bag",true);
	return coin;
}

void CoinBag::updateMe(BaseHero* hero)
{
	
}

void CoinBag::die()
{
	this->setVisible(false);
	effect = SkeletonAnimation::createWithFile("Effect_getgolden.json", "Effect_getgolden.atlas", SCREEN_SIZE.height / 4.5f / 641);
	effect->setAnimation(0, "Effect_goldbag", false);
	effect->setPosition(this->getPosition());
	this->getParent()->addChild(effect, ZORDER_ENEMY);
	CallFunc *call = CallFunc::create([&]() {
		effect->removeFromParentAndCleanup(true);
		this->removeFromParentAndCleanup(true);
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), call));
}

void CoinBag::onExit()
{
	B2Skeleton::onExit();
}


