#include "B2Skeleton.h"

B2Skeleton::B2Skeleton(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{
	body = nullptr;
}

B2Skeleton::B2Skeleton(spSkeletonData * data):SkeletonAnimation(data)
{
}

B2Skeleton * B2Skeleton::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

B2Skeleton * B2Skeleton::create(spSkeletonData * data)
{
	return nullptr;
}



void B2Skeleton::die()
{

}



void B2Skeleton::initBoxPhysic(b2World *world, Point pos)
{

	b2PolygonShape shape;
	auto size = this->getBoundingBox().size;
	shape.SetAsBox(size.width / 2 / PTM_RATIO, 0 / PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;		// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

}

void B2Skeleton::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

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


//void B2Skeleton::updateSkeleton(float dt)
//{
//	//SkeletonAnimation::update(dt);
//	if (body != nullptr) {
//		this->setPositionX(body->GetPosition().x * PTM_RATIO);
//		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height / 2);
//		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
//	}
//}

