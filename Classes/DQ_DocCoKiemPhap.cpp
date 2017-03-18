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

	/*kp->hitGroundEffect = Sprite::create("Animation/DuongQua/halfball.png");
	kp->hitGroundEffect->setAnchorPoint(Vec2(0.5f, 0));
	kp->addChild(kp->hitGroundEffect);*/

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

	fixtureDef.filter.categoryBits = BITMASK_SPECIAL_SWORD;
	fixtureDef.filter.maskBits = BITMASK_FLOOR | BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_WOODER;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);
	body->SetLinearVelocity(b2Vec2(0, -SCREEN_SIZE.height * 3 / PTM_RATIO));
}


void DQ_DocCoKiemPhap::die()
{
	this->setVisible(false);
}


void DQ_DocCoKiemPhap::hitGround()
{
	particle = ParticleSystemQuad::create("Effect/particle.plist");
	particle->setTag(999);

	particle->setPosition(this->getPosition());
	this->getParent()->addChild(particle, ZORDER_ENEMY);

	/*auto hideMe = CallFunc::create([&]() {
		particle->removeAllChildrenWithCleanup(true);
	});

	auto seq = Sequence::create(DelayTime::create(0.5f), hideMe, nullptr);
	particle->runAction(seq);*/
}


void DQ_DocCoKiemPhap::updateMe(float dt)
{
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height * 0.05f);
	}
}
//
//void DQ_DocCoKiemPhap::updateMe(float dt)
//{
//}

