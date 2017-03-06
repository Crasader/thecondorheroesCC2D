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
	duongQua->stateMachine = new Running();
	duongQua->setMoveVel(duongQua->SCREEN_SIZE.width / PTM_RATIO / 4.0f);
	duongQua->setJumpVel(duongQua->SCREEN_SIZE.height * 1.2f / PTM_RATIO);
	duongQua->facingRight = true;

	duongQua->numberOfJump = 2;
	duongQua->onGround = false;
	duongQua->isAttacking = false;

	duongQua->update(0.0f);
	duongQua->setScaleX(1);		// facing right

	duongQua->setTimeScale(0.8f);

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
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
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
}

void DuongQua::attackNormal()
{
	clearTracks();
	//setAnimation(0, "attack1", false);
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
		if (strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1) {
			setIsAttacking(false);
		}

		else if (strcmp(getCurrent()->animation->name, "attack3") == 0 && loopCount == 1) {
			setIsAttacking(false);
		}
	});
}

void DuongQua::update(float dt)
{
	BaseHero::update(dt);

	if (stateMachine && getBody()) {

		if (getPositionY() + getTrueRadiusOfHero() * 2 < 0) {
			getBody()->SetTransform(b2Vec2(getPositionX() / PTM_RATIO, SCREEN_SIZE.height / PTM_RATIO), getBody()->GetAngle());
			return;
		}

		if (getBody()->GetLinearVelocity().y < 0) {
			stateMachine->land(this);
			return;
		}
		auto currentVelY = getBody()->GetLinearVelocity().y;

		getBody()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

		if (getOnGround() && !getIsAttacking())
			stateMachine->run(this);
	}
}

void DuongQua::checkNearBy(BaseEnemy * enemy)
{
	if ((enemy->getPositionX() - this->getPositionX() > 0) && 
		(enemy->getPositionX() - this->getPositionX() < getTrueRadiusOfHero() * 2.2f)) {
		if (fabs(this->getPositionY() - enemy->getPositionY()) < getTrueRadiusOfHero() / 4) {
			// enemy die
			log("Enemy Die");
		}
	}
}


