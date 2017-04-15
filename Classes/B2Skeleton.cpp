#include "B2Skeleton.h"



B2Skeleton::B2Skeleton():SkeletonAnimation()
{
	body = nullptr;
}

B2Skeleton::~B2Skeleton()
{
}

B2Skeleton::B2Skeleton(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{
	body = nullptr;
}

B2Skeleton::B2Skeleton(spSkeletonData * data) : SkeletonAnimation(data)
{
	body = nullptr;
}

B2Skeleton * B2Skeleton::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
	
}

B2Skeleton * B2Skeleton::create(spSkeletonData * data)
{
	auto skeleton = new B2Skeleton(data);
	//skeleton->initWithData(data);
	skeleton->update(1.0f);
	return skeleton;
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
	//circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
	this->getBoundingBox().size.height > this->getBoundingBox().size.width ? circle_shape.m_radius = this->getBoundingBox().size.width / 2 / PTM_RATIO :
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

Point B2Skeleton::getBoneLocation(string boneName)
{
	auto gun = findBone(boneName);
	auto pos = Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	pos = pos + this->getPosition();
	return pos;
}

void B2Skeleton::updateMe(BaseHero * hero)
{
	if (body != nullptr /*&& body->GetType() == b2_staticBody*/) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO
			- this->getParent()->getPositionY());
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
}


//void B2Skeleton::updateMe(BaseHero* hero)
//{
//	if (body != nullptr) {
//		this->setPositionX(this->getB2Body()->GetPosition().x * PTM_RATIO);
//		this->setPositionY(this->getB2Body()->GetPosition().y * PTM_RATIO - this->getBoundingBox().size.height / 2);
//	}
//}

void B2Skeleton::onExit()
{
	SkeletonAnimation::onExit();
	if (body != nullptr) {
		auto world = body->GetWorld();
		world->DestroyBody(body);
	}
}

