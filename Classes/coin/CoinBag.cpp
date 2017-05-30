
#include "CoinBag.h"
#include "SkeletonManager.h"
#include "BaseHero.h"
#include "AudioManager.h"

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
	coin->setAnimation(0, "Gold_bag", true);
	coin->effect = nullptr;
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
	bag->effect = nullptr;
	return bag;
}

void CoinBag::updateMe(BaseHero* hero)
{
	if (!this->isVisible() && this->getB2Body()->GetType() == b2_staticBody && this->effect == nullptr) {
		this->resume();
		this->setVisible(true);
		B2Skeleton::updateMe(hero);
		//this->update(0);
	}
	if (this->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width / 2) {
		if (hero->getItemValue(KEY_ITEM_MAGNET) > 0) {
			// neu hero co magnet
			if (this->getB2Body()->GetType() == b2_dynamicBody) {
				Vec2 coinToHero;
				B2Skeleton::updateMe(hero);
				coinToHero = Vec2(hero->getB2Body()->GetPosition().x*PTM_RATIO, hero->getB2Body()->GetPosition().y*PTM_RATIO) - this->getPosition();

				coinToHero = coinToHero*(SCREEN_SIZE.width / coinToHero.length());
				if (coinToHero.x < 0) {
					this->getB2Body()->SetType(b2_dynamicBody);
					this->getB2Body()->SetLinearVelocity(b2Vec2(coinToHero.x / PTM_RATIO - hero->getB2Body()->GetLinearVelocity().x, coinToHero.y / PTM_RATIO));
				}
				else if (coinToHero.x >= 0) {
					this->getB2Body()->SetType(b2_dynamicBody);
					this->getB2Body()->SetLinearVelocity(b2Vec2(coinToHero.x / PTM_RATIO + hero->getB2Body()->GetLinearVelocity().x, coinToHero.y / PTM_RATIO));

				}
			}

			else {
				if (hero->getItemValue(KEY_ITEM_MAGNET) > 30 && this->getB2Body()->GetType() == b2_staticBody) {
					this->getB2Body()->SetType(b2_dynamicBody);
					this->changeBodyCategoryBits(BITMASK_COIN_BULLION);
					this->changeBodyMaskBits(BITMASK_HERO);
				}
			}
		}

	}
}

void CoinBag::die()
{
	AudioManager::playSound(SOUND_COINBAG);
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


