#include "B2Skeleton.h"



B2Skeleton::B2Skeleton(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{
	body = nullptr;
}

B2Skeleton * B2Skeleton::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}


b2Body * B2Skeleton::getB2Body()
{
	return body;
}

void B2Skeleton::initBoxPhysic(b2World *world, Point pos)
{

	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

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

void B2Skeleton::initCirclePhysic(b2World * world, Point pos)
{
	b2BodyDef bodyDef;
	b2CircleShape shape;
	b2FixtureDef fixtureDef;

	auto size = this->getBoundingBox().size;
	shape.m_p.SetZero();
	shape.m_radius = size.height / 2 / PTM_RATIO;
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

void B2Skeleton::initPhysicWithShapeCache(b2World * world, Point pos, string key)
{
	b2BodyDef bodyDef;

	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	body = world->CreateBody(&bodyDef);
	GB2ShapeCache::sharedGB2ShapeCache()->addFixturesToBody(body, key);
}

void B2Skeleton::changeBodyCategoryBits(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = mask;
	fixture->SetFilterData(filter);
}

void B2Skeleton::changeBodyMaskBits(uint16 mask)
{
	auto fixture = this->body->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.maskBits = mask;
	fixture->SetFilterData(filter);
}

void B2Skeleton::update(float dt)
{
	SkeletonAnimation::update(dt);
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height / 2);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
}






//void B2Skeleton::changeBodyBitMask(uint16 mask)
//{
//	auto fixture = this->body->GetFixtureList();
//	b2Filter filter = fixture->GetFilterData();
//	filter.categoryBits = mask;
//	fixture->SetFilterData(filter);
//}





