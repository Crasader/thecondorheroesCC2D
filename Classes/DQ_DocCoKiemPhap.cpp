#include "DQ_DocCoKiemPhap.h"

DQ_DocCoKiemPhap::DQ_DocCoKiemPhap()
{
}

DQ_DocCoKiemPhap::~DQ_DocCoKiemPhap()
{
}

DQ_DocCoKiemPhap * DQ_DocCoKiemPhap::create(string file)
{
	DQ_DocCoKiemPhap* kp = new DQ_DocCoKiemPhap();
	kp->initWithFile(file);
	kp->setTag(TAG_DQ_DOC_CO_KIEM_PHAP);
	kp->isCollide = false;
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
	fixtureDef.filter.maskBits = BITMASK_UNDER_GROUND | BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_BOSS;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);
	body->SetLinearVelocity(b2Vec2(0, -SCREEN_SIZE.height * 4 / PTM_RATIO));
}


void DQ_DocCoKiemPhap::hitGround()
{
	auto gameLayer = this->getParent();

	/*hitGroundEffect = Sprite::create("Animation/DuongQua/halfball.png");
	hitGroundEffect->setAnchorPoint(Vec2(0.5f, 0));

	auto hideFX = CallFunc::create([&]() {
		hitGroundEffect->removeFromParentAndCleanup(true);
	});

	hitGroundEffect->setScale(this->getScale() * 0.75f);
	hitGroundEffect->setPosition(this->getPositionX(), this->getPositionY() - this->getBoundingBox().size.height * 0.65f);
	gameLayer->addChild(hitGroundEffect, ZORDER_ENEMY);
	auto action = ScaleBy::create(0.3f, 1.6f);

	auto seq = Sequence::create(action, hideFX, nullptr);
	hitGroundEffect->runAction(seq);*/


	particle = ParticleSystemQuad::create("Effect/breakearth.plist");
	particle->setScale(this->getScale() * 0.7f);
	particle->setPosition(this->getPositionX(), this->getPositionY() - this->getBoundingBox().size.height * 0.5f);
	gameLayer->addChild(particle, ZORDER_ENEMY);

	auto hideParticle = CallFunc::create([&]() {
		particle->removeFromParentAndCleanup(true);
	});

	auto seq2 = Sequence::create(DelayTime::create(0.5f), hideParticle, nullptr);
	particle->runAction(seq2);
}


void DQ_DocCoKiemPhap::updateMe(float dt)
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO);
	}
}
//
//void DQ_DocCoKiemPhap::updateMe(float dt)
//{
//}

