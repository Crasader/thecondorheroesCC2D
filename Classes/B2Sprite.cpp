#include "B2Sprite.h"
#include "AudioEngine.h"

B2Sprite::B2Sprite()
{
}

B2Sprite::~B2Sprite()
{
}

B2Sprite * B2Sprite::create()
{
	return nullptr;
}


void B2Sprite::initBoxPhysic(b2World * world, Point pos)
{
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);

	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;
	

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void B2Sprite::initCirclePhysic(b2World * world, Point pos)
{
	b2BodyDef bodyDef;
	b2CircleShape shape;
	b2FixtureDef fixtureDef;

	auto size = this->getBoundingBox().size;
	shape.m_p.SetZero();
	shape.m_radius = size.width / 2 / PTM_RATIO;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.5f;
	fixtureDef.shape = &shape;

	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void B2Sprite::initPhysicWithShapeCache(b2World * world, Point pos, string key)
{
	b2BodyDef bodyDef;

	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x/PTM_RATIO, pos.y / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);
	GB2ShapeCache::sharedGB2ShapeCache()->addFixturesToBody(body, key);
}

void B2Sprite::changeBodyCategoryBits(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = mask;
	fixture->SetFilterData(filter);
}

void B2Sprite::changeBodyMaskBits(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.maskBits = mask;
	fixture->SetFilterData(filter);
}

void B2Sprite::setAngle(float radian)
{
	float vx = SCREEN_SIZE.width * 1.3f / PTM_RATIO * cosf(radian);
	float vy = SCREEN_SIZE.width * 1.3f / PTM_RATIO * sinf(radian);
	this->body->SetLinearVelocity(b2Vec2(vx, vy));
}

//void B2Sprite::updateMe(float dt)
//{
//}


void B2Sprite::updateMe(BaseHero *hero)
{
	if (body != nullptr) {
		if (this->getParent()) {
			//if (!strcmp(this->getParent()->getName().c_str(), "gameLayer")) {
				this->setPositionX(body->GetPosition().x * PTM_RATIO);
				this->setPositionY(body->GetPosition().y * PTM_RATIO);
				this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));	// rotate
			//}
			//else {
			//	this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
			//	this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getParent()->getPositionY());
			//	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));	// rotate
			//}
		}
	}
}



void B2Sprite::onExit()
{
	Sprite::onExit();
	if (body != nullptr) {
		auto world = body->GetWorld();
		world->DestroyBody(body);
		body = nullptr;
	}
}



