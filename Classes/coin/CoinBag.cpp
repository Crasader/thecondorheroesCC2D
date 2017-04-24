
#include "CoinBag.h"
#include "manager\SkeletonManager.h"

CoinBag::CoinBag(spSkeletonData * data):B2Skeleton(data)
{
}

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

CoinBag * CoinBag::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto bag = new CoinBag(data);
	//enemy->initWithData(data);
	bag->update(0.0f);
	bag->setTag(TAG_COINBAG);
	bag->setAnimation(0, "Gold_bag", true);
	return bag;
}

void CoinBag::updateMe(BaseHero* hero)
{
	B2Skeleton::updateMe(hero);
}

void CoinBag::die()
{
	this->setVisible(false);

	//effect = SkeletonAnimation::createWithFile("Effect_getgolden.json", "Effect_getgolden.atlas", SCREEN_SIZE.height / 4.5f / 641);
	if (!SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden")) {
		SkeletonManager::getInstance()->cacheSkeleton("Effect_getgolden", SCREEN_SIZE.height / 3 / 291);
	}
	effect = SkeletonAnimation::createWithData(SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden"));

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


