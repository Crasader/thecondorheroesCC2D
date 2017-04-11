#include "Coin.h"

Coin::Coin()
{
	body = nullptr;
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
	//this->chang
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
	effect = SkeletonAnimation::createWithFile("Effect_getgolden.json", "Effect_getgolden.atlas", SCREEN_SIZE.height / 3 / 291);
	effect->setAnimation(0, "Effect_coin", false);
	effect->setPosition(this->getPosition());
	this->getParent()->addChild(effect, ZORDER_ENEMY);
	CallFunc *call = CallFunc::create([&]() {
		effect->removeFromParentAndCleanup(true);
		this->removeFromParentAndCleanup(true);
		auto world = this->getB2Body()->GetWorld();
		//world->DestroyBody(this->getB2Body());
		this->setB2Body(nullptr);
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
	//this->removeFromParentAndCleanup(true);
	//log("delete coin");
}

void Coin::updateMe(BaseHero *hero)
{
	B2Sprite::updateMe(hero);
}

void Coin::setAngle(float radian)
{
	float vec = CCRANDOM_0_1();
	float vx = SCREEN_SIZE.width * vec / PTM_RATIO * cosf(radian);
	float vy = SCREEN_SIZE.width * vec / PTM_RATIO * sinf(radian);
	this->body->SetLinearVelocity(b2Vec2(vx, vy));

}
