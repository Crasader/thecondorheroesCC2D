#include "BaseHero.h"
#include "AudioEngine.h"
#include "Global.h"


BaseHero::BaseHero(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	
}

BaseHero * BaseHero::create(string jsonFile, string atlasFile, float scale)
{
	BaseHero* baseHero = new BaseHero(jsonFile, atlasFile, scale);
	return baseHero;
}

void BaseHero::initSwordPhysic(b2World *world, Point position, float width)
{
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(width / PTM_RATIO, getTrueRadiusOfHero() / 5 / PTM_RATIO);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_TOANCHAN1 |BITMASK_TOANCHAN2;

	bodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	swordBody = world->CreateBody(&bodyDef);
	swordBody->CreateFixture(&fixtureDef);
}

void BaseHero::changeSwordCategoryBitmask(uint16 bit)
{
	auto fixture = swordBody->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = bit;
	fixture->SetFilterData(filter);
}




void BaseHero::run()
{

}

void BaseHero::normalJump()
{
}

void BaseHero::doubleJump()
{
}

void BaseHero::landing()
{
}

void BaseHero::die()
{
}

void BaseHero::attackNormal()
{
}

void BaseHero::attackLanding()
{
}

void BaseHero::attackBySkill1()
{
}

void BaseHero::attackBySkill2()
{
}

void BaseHero::attackBySkill3()
{
}

void BaseHero::injured()
{
}

void BaseHero::die(Point posOfCammera)
{
}

void BaseHero::listener()
{
}

void BaseHero::updateMe(float dt)
{
	if (getBody() != nullptr) {
		this->setPositionX(this->getBody()->GetPosition().x * PTM_RATIO);
		this->setPositionY(this->getBody()->GetPosition().y * PTM_RATIO - trueRadiusOfHero);

		getSwordBody()->SetTransform(b2Vec2(getBody()->GetPosition().x + getTrueRadiusOfHero() * 1.3f / PTM_RATIO, getBody()->GetPosition().y), 
										getSwordBody()->GetAngle());
	}
}


StateMachine * BaseHero::getFSM()
{
	return stateMachine;
}

