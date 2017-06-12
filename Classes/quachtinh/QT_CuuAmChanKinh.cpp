#include "QT_CuuAmChanKinh.h"

QT_CuuAmChanKinh::QT_CuuAmChanKinh()
{
	isAdded = false;
	isCollide = false;
}

QT_CuuAmChanKinh::~QT_CuuAmChanKinh()
{

}

QT_CuuAmChanKinh * QT_CuuAmChanKinh::create()
{
	QT_CuuAmChanKinh* ck = new QT_CuuAmChanKinh();

	ck->initWithSpriteFrameName("skill1_firedown_1.png");
	ck->setTag(TAG_QT_CUU_AM_CHAN_KINH);

	ck->angle = -45.0;
	ck->setRotation(ck->angle);

	ck->autorelease();

	return ck;
}

void QT_CuuAmChanKinh::initCirclePhysic(b2World * world, Point pos)
{
	auto size = this->getBoundingBox().size;

	// config physic manually
	b2BodyDef bodyDef;
	b2CircleShape shape;
	b2FixtureDef fixtureDef;

	shape.m_radius = size.width / 6 / PTM_RATIO;

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;

	fixtureDef.filter.categoryBits = BITMASK_SWORD;
	fixtureDef.filter.maskBits = BITMASK_UNDER_GROUND | BITMASK_WOODER | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetGravityScale(0);

	body->SetFixedRotation(true);
}

void QT_CuuAmChanKinh::updateMe()
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getBoundingBox().size.width / 5);
		this->setPositionY(body->GetPosition().y * PTM_RATIO + this->getBoundingBox().size.height / 5);
	}
}

void QT_CuuAmChanKinh::runAni()
{
	Vector<SpriteFrame* > aniFrames;
	for (int i = 1; i <= 5; ++i) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("skill1_firedown_%i.png", i));
		aniFrames.pushBack(frame);
	}

	auto animate = Animate::create(Animation::createWithSpriteFrames(aniFrames, 0.07f));
	this->runAction(RepeatForever::create(animate));
}

void QT_CuuAmChanKinh::explosion()
{
	fireEffect = Sprite::createWithSpriteFrameName("skill1_broken_1.png");
	fireEffect->setFlippedX(-1);
	fireEffect->setAnchorPoint(Vec2(0, 0));
	fireEffect->setScale(getScale() * 0.45f);
	

	fireEffect->setPosition(this->getPositionX(), 
							this->getPositionY() - this->getBoundingBox().size.height * 0.4f);
	auto gameLayer = this->getParent();
	gameLayer->addChild(fireEffect, ZORDER_ENEMY);

	Vector<SpriteFrame* > aniFrames;
	for (int i = 1; i <= 8; ++i) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("skill1_broken_%i.png", i));
		aniFrames.pushBack(frame);
	}

	auto animate = Animate::create(Animation::createWithSpriteFrames(aniFrames, 0.06f));

	auto hideEffect = CallFunc::create([&]() {
		fireEffect->removeFromParentAndCleanup(true);
	});

	auto seq = Sequence::createWithTwoActions(animate, hideEffect);
	fireEffect->runAction(seq);
}

