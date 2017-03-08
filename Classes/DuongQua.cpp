#include "DuongQua.h"
#include "AudioEngine.h"
#include "Global.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
}

DuongQua * DuongQua::create(string jsonFile, string atlasFile, float scale)
{
	DuongQua* duongQua = new DuongQua(jsonFile, atlasFile, scale);
	duongQua->setTag(TAG_HERO);
	duongQua->previous_state = new Idling();
	duongQua->stateMachine = new Running();
	duongQua->setMoveVel(duongQua->SCREEN_SIZE.width / PTM_RATIO / 3.3f);
	duongQua->setJumpVel(duongQua->SCREEN_SIZE.height * 1.4f / PTM_RATIO);
	duongQua->facingRight = true;

	duongQua->numberOfJump = 2;

	duongQua->update(0.0f);
	duongQua->setScaleX(1);		// facing right

	//duongQua->setTimeScale(0.8f);

	// splash
	duongQua->slash = Sprite::create("Animation/DuongQua/slash2.png");
	duongQua->slash->setScale(scale);
	duongQua->slash->setVisible(false);
	//

	return duongQua;
}

void DuongQua::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 6.6f / PTM_RATIO;

	// True radius of hero is here
	setTrueRadiusOfHero(circle_shape.m_radius * PTM_RATIO);
	//

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.filter.categoryBits = BITMASK_HERO;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR| BITMASK_WOODER | BITMASK_COIN|BITMASK_TOANCHAN1;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);


	// connect sword with body
	initSwordPhysic(world, Point(pos.x + trueRadiusOfHero * 1.3f, pos.y), trueRadiusOfHero);
}

void DuongQua::run()
{
	clearTracks();
	addAnimation(0, "run", true);
	setToSetupPose();


}

void DuongQua::normalJump()
{

	clearTracks();
	addAnimation(0, "jump", false);
	setToSetupPose();
}

void DuongQua::doubleJump()
{
	clearTracks();
	addAnimation(0, "jumpx2", true);
	setToSetupPose();
}

void DuongQua::landing()
{
	clearTracks();
	addAnimation(0, "landing", false);
	setToSetupPose();
}

void DuongQua::die()
{
	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getBody()->SetTransform(b2Vec2(SCREEN_SIZE.width / 2 / PTM_RATIO, SCREEN_SIZE.height / PTM_RATIO), getBody()->GetAngle());
}

void DuongQua::attackNormal()
{
	clearTracks();
	auto r = rand() % 2;
	if (r) {
		addAnimation(0, "attack1", false);
	} else 
		addAnimation(0, "attack2", false);

	
	setToSetupPose();
}

void DuongQua::attackLanding()
{
	clearTracks();
	addAnimation(0, "attack3", false);
	setToSetupPose();
}

void DuongQua::attackBySkill1()
{
}

void DuongQua::attackBySkill2()
{
}

void DuongQua::attackBySkill3()
{
}

void DuongQua::injured()
{
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();
}

void DuongQua::die(Point posOfCammera)
{
	
}

void DuongQua::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if ((strcmp(getCurrent()->animation->name, "attack1") == 0 && loopCount == 1) || 
		(strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1))
		{
			changeSwordCategoryBitmask(BITMASK_ENEMY);
			//getSlash()->setVisible(false);
			getPreviousState()->run(this);
		}

		else if (strcmp(getCurrent()->animation->name, "attack3") == 0 && loopCount == 1) {
			changeSwordCategoryBitmask(BITMASK_ENEMY);
			//getSlash()->setVisible(false);
		}
	});
}

void DuongQua::updateMe(float dt)
{
	BaseHero::updateMe(dt);

	if (stateMachine && getBody()) {

		if (getPositionY() + getTrueRadiusOfHero() * 2 < 0) {
			this->die();
			//getBody()->SetTransform(b2Vec2(SCREEN_SIZE.width * 0.3f / PTM_RATIO, SCREEN_SIZE.height / PTM_RATIO), getBody()->GetAngle());
			return;
		}

		if (getBody()->GetLinearVelocity().y < 0) {
			stateMachine->land(this);
			return;
		}
		
		auto currentVelY = getBody()->GetLinearVelocity().y;
		getBody()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

	}
}


