#include "QT_ThanLongBatVi.h"

QT_ThanLongBatVi::QT_ThanLongBatVi()
{
	isAdded = false;
}

QT_ThanLongBatVi::~QT_ThanLongBatVi()
{
}

QT_ThanLongBatVi * QT_ThanLongBatVi::create()
{
	QT_ThanLongBatVi* tl = new QT_ThanLongBatVi();
	tl->initWithSpriteFrameName("skill2_lightning_1.png");
	tl->setTag(TAG_QT_THAN_LONG_BAT_VI);
	return tl;
}

void QT_ThanLongBatVi::initBoxPhysic(b2World * world, Point pos)
{

	auto size = this->getBoundingBox().size;

	// config physic manually
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 4 / PTM_RATIO, size.height / 8 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_SWORD;
	fixtureDef.filter.maskBits = BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY | BITMASK_WOODER;
	fixtureDef.isSensor = true;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);
	body->SetLinearVelocity(b2Vec2(0, -SCREEN_SIZE.height * 15 / PTM_RATIO));
}

void QT_ThanLongBatVi::runAni()
{
	Vector<SpriteFrame* > aniFrames;
	for (int i = 1; i <= 3; ++i) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("skill2_lightning_%i.png", i));
		aniFrames.pushBack(frame);
	}

	auto animate = Animate::create(Animation::createWithSpriteFrames(aniFrames, 0.15f));
	auto hide = CallFunc::create([&]() {
		this->setVisible(false);
	});
	this->runAction(Sequence::createWithTwoActions(animate, hide));
}


