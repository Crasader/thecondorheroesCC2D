#include "DQ_DocCoKiemPhap.h"

DQ_DocCoKiemPhap::DQ_DocCoKiemPhap()
{
	isCollide = false;
}

DQ_DocCoKiemPhap::~DQ_DocCoKiemPhap()
{
}

DQ_DocCoKiemPhap * DQ_DocCoKiemPhap::create()
{
	DQ_DocCoKiemPhap* kp = new DQ_DocCoKiemPhap();
	kp->initWithFile("Animation/DuongQua/Sword.png");
	kp->setTag(TAG_DQ_DOC_CO_KIEM_PHAP);

	kp->effectLand = Sprite::createWithSpriteFrameName("star2.png");
	kp->effectLand->setPosition(kp->getContentSize().width * 0.5f, kp->getContentSize().height * 0.6f);
	kp->addChild(kp->effectLand);
	return kp;
}

void DQ_DocCoKiemPhap::initBoxPhysic(b2World * world, Point pos)
{

	auto size = this->getBoundingBox().size;

	// config physic manually
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(size.width / 4 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_SWORD;
	fixtureDef.filter.maskBits = BITMASK_UNDER_GROUND | BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_DATNHIBA;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);
	body->SetLinearVelocity(b2Vec2(0, -SCREEN_SIZE.height * 4 / PTM_RATIO));
}


void DQ_DocCoKiemPhap::landingEffect()
{
	auto hideFX = CallFunc::create([&]() {
		effectLand->setVisible(false);
	});

	auto action = ScaleBy::create(0.3f, 2);

	auto seq = Sequence::create(action, hideFX, nullptr);
	effectLand->runAction(seq);
}

void DQ_DocCoKiemPhap::hitGround()
{
	auto gameLayer = this->getParent();

	particle = ParticleSystemQuad::create("Effect/breakearth.plist");
	particle->setScale(this->getScale() * 0.7f);
	particle->setPosition(this->getPositionX(), this->getPositionY() - this->getBoundingBox().size.height * 0.5f);
	gameLayer->addChild(particle, ZORDER_ENEMY);

	auto hideParticle = CallFunc::create([&]() {
		particle->removeFromParentAndCleanup(true);
	});

	auto seq2 = Sequence::create(DelayTime::create(0.5f), hideParticle, nullptr);
	particle->runAction(seq2);

	this->schedule([&](float dt) {

		if (opacity <= 0) {
			this->unschedule("key_opacity");
		}
		opacity -= 15;

		this->setOpacity(opacity = opacity < 0 ? 0 : opacity);
	}, 0.1f, "key_opacity");
}


void DQ_DocCoKiemPhap::updateMe()
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO);	
	}
}

