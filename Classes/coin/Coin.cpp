#include "Coin.h"
#include "manager\SkeletonManager.h"
#include "BaseHero.h"

#include "manager\AudioManager.h"

Coin::Coin()
{
	body = nullptr;
	animate = nullptr;
	world = nullptr;
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

Coin * Coin::create(SpriteBatchNode * batchNode)
{
	Coin* coin = new Coin();
	coin->initWithTexture(batchNode->getTexture());
	coin->setTag(TAG_COIN);
	//coin->runAnimation();
	return coin;
}

void Coin::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->SetType(b2_staticBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	//this->chang
}

void Coin::runAnimation()
{
	if (!animate) {
		//SpriteBatchNode* node;
		Vector <SpriteFrame*> aniframes;
		for (int i = 1; i < 9; i++)
		{
			string frameName = StringUtils::format("coin_0%d.png", i);
			//log(frameName.c_str());
			aniframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
			//aniframes.pushBack(SpriteFrame::createWithTexture(node->getTexture(),));
		}

		Animation* animation = Animation::createWithSpriteFrames(aniframes, 0.08f);
		animate = Animate::create(animation);
		this->runAction(RepeatForever::create(animate));
	}
	else {
		this->resumeSchedulerAndActions();
	}
}

void Coin::picked()
{
	AudioManager::playSound(SOUND_COIN);
	this->setVisible(false);
	if (!SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden")) {
		SkeletonManager::getInstance()->cacheSkeleton("Effect_getgolden", SCREEN_SIZE.height / 3 / 291);
	}
	effect = SkeletonAnimation::createWithData(SkeletonManager::getInstance()->getSkeletonData("Effect_getgolden"));
	effect->setAnimation(0, "Effect_coin", false);
	effect->setPosition(this->getPosition());
	this->getParent()->addChild(effect, ZORDER_ENEMY);
	CallFunc *call = CallFunc::create([&]() {
		if (effect) {
			effect->removeFromParentAndCleanup(true);
			effect = nullptr;
			this->removeFromParentAndCleanup(true);
		}
	});
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), call));
	this->setTag(0);
	//this->listener();
}

void Coin::listener()
{
	effect->setCompleteListener([&](int trackIndex, int loopCount) {
		if ((strcmp(effect->getCurrent()->animation->name, "Effect_coin") == 0) && loopCount == 1) {
			/*effect->removeFromParentAndCleanup(true);
			this->removeFromParentAndCleanup(true);*/
		}
	});
}



void Coin::updateMe(BaseHero *hero)
{
	if (this->getB2Body()) {
		if (!this->isVisible() && this->getB2Body()->GetType() == b2_staticBody) {
			this->resumeSchedulerAndActions();
			this->setVisible(true);
			return;
		}
		if (this->isVisible() && hero->getB2Body()) {
			if (((this->getPosition() -
				(hero->getPosition() + Vec2(0, hero->getTrueRadiusOfHero()))).length()
				< hero->getTrueRadiusOfHero())) {
				this->picked();
				hero->setCoinExplored(hero->getCoinExplored() + hero->getCoinRatio());
			}
		}
		if (this->getB2Body()->GetType() == b2_dynamicBody) {
			B2Sprite::updateMe(hero);
		}
		if (this->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width / 2) {
			if (hero->getItemValue(KEY_ITEM_MAGNET) > 0) {
				// neu hero co magnet

				if (this->getB2Body()->GetType() == b2_dynamicBody) {
					Vec2 coinToHero;

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
					}
				}
			}

		}
	}
}

void Coin::setAngle(float radian)
{
	float vec = CCRANDOM_0_1();
	float vx = SCREEN_SIZE.width * vec / PTM_RATIO * cosf(radian);
	float vy = SCREEN_SIZE.width * vec / PTM_RATIO * sinf(radian);
	this->body->SetLinearVelocity(b2Vec2(vx, vy));
}
