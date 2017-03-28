#include "BaseHero.h"
#include "AudioEngine.h"
#include "Global.h"


BaseHero::BaseHero(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	bloodScreen = Sprite::create("red-screen.png");
	bloodScreen->setScaleX(SCREEN_SIZE.width / bloodScreen->getContentSize().width);
	bloodScreen->setScaleY(SCREEN_SIZE.height / bloodScreen->getContentSize().height);
	bloodScreen->setVisible(false);
	isDriverEagle = false;
	currentRunDis = 0.0f;
	preRunDis = 0.0f;
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

	shape.SetAsBox(width / PTM_RATIO, getTrueRadiusOfHero()* 0.85f / PTM_RATIO);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_TOANCHAN1 |BITMASK_TOANCHAN2 |BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BAG;

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




void BaseHero::idle()
{
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
	if (getB2Body() != nullptr) {

		currentRunDis = this->getPositionX() / (SCREEN_SIZE.width / 23);
		log("cur: %f", currentRunDis);

		this->setPositionX(this->getB2Body()->GetPosition().x * PTM_RATIO);
		this->setPositionY(this->getB2Body()->GetPosition().y * PTM_RATIO - trueRadiusOfHero);


		getSwordBody()->SetTransform(b2Vec2(getB2Body()->GetPosition().x + getTrueRadiusOfHero() * 2.2f / PTM_RATIO, getB2Body()->GetPosition().y)
			,getSwordBody()->GetAngle());
	}

	if (health <= 0 /* || getPositionY() + getTrueRadiusOfHero() * 2 < 0 */) {
		getFSM()->changeState(MDie);
	}
}

void BaseHero::doCounterSkill1()
{
}

void BaseHero::doCounterSkill2()
{
}

void BaseHero::doCounterSkill3()
{
}


StateMachine * BaseHero::getFSM()
{
	return stateMachine;
}

