#include "CoinBullion.h"
#include "SkeletonManager.h"



CoinBullion::CoinBullion(spSkeletonData * data) :B2Skeleton(data)
{
}

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

CoinBullion * CoinBullion::create(string filename, float scale)
{

	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto bag = new CoinBullion(data);
	//enemy->initWithData(data);
	bag->update(0.0f);
	bag->setTag(TAG_COINBAG);
	bag->setAnimation(0, "Gold", true);
	return bag;
}

void CoinBullion::updateMe(BaseHero* hero)
{
	B2Skeleton::updateMe(hero);
}

void CoinBullion::picked()
{
	this->setVisible(false);
	//effect = SkeletonAnimation::createWithFile("Effect_getgolden.json", "Effect_getgolden.atlas", SCREEN_SIZE.height / 3 / 290);
	if (!SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden")) {
		SkeletonManager::getInstance()->cacheSkeleton("Effect_getgolden", SCREEN_SIZE.height / 3 / 291);
	}
	effect = SkeletonAnimation::createWithData(SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden"));
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
