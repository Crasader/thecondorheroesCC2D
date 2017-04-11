#include "CoinBullion.h"



CoinBullion::CoinBullion(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile, atlasFile, scale)
{
}

CoinBullion * CoinBullion::create(string jsonFile, string atlasFile, float scale)
{
	auto coin = new CoinBullion(jsonFile, atlasFile, scale);
	coin->update(0.0f);
	coin->setTag(TAG_COINBULLION);
	coin->setAnimation(0,"Gold",true);
	return coin;
}

void CoinBullion::updateMe(BaseHero* hero)
{
	
}

void CoinBullion::picked()
{
	this->setVisible(false);
	effect = SkeletonAnimation::createWithFile("Effect_getgolden.json", "Effect_getgolden.atlas", SCREEN_SIZE.height / 3 / 290);
	effect->setAnimation(0, "Effect_gold", false);
	effect->setPosition(this->getPosition());
	this->getParent()->addChild(effect, ZORDER_ENEMY);
	CallFunc *call = CallFunc::create([&]() {
		effect->removeFromParentAndCleanup(true);
		this->removeFromParentAndCleanup(true);
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), call));
}



void CoinBullion::onExit()
{
	B2Skeleton::onExit();
}
