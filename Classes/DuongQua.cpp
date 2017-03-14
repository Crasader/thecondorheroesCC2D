#include "DuongQua.h"
#include "DQ_TieuHonChuong.h"
#include "AudioEngine.h"
#include "Global.h"


DuongQua::DuongQua(string jsonFile, string atlasFile, float scale) : BaseHero(jsonFile, atlasFile, scale)
{
	checkCanShoot = 0;
}

DuongQua * DuongQua::create(string jsonFile, string atlasFile, float scale)
{
	DuongQua* duongQua = new DuongQua(jsonFile, atlasFile, scale);
	duongQua->setTag(TAG_HERO);

	duongQua->update(0.0f);

	duongQua->stateMachine = new StateMachine(duongQua);
	duongQua->stateMachine->setCurrentState(MLand);

	duongQua->setMoveVel(duongQua->SCREEN_SIZE.width / PTM_RATIO / 2.3f);
	duongQua->setJumpVel(duongQua->SCREEN_SIZE.height * 1.4f / PTM_RATIO);

	duongQua->health = 4;

	duongQua->facingRight = true;

	duongQua->numberOfJump = 2;

	duongQua->setOnGround(false);
	duongQua->setIsPrior(false);		// future, we need to add props into base class
	duongQua->setIsPriorSkill1(false);
	duongQua->setIsPriorSkill2(false);
	duongQua->setIsPriorSkill3(false);

	// splash
	duongQua->slash_1 = Sprite::create("Animation/DuongQua/slash1-1.png");
	duongQua->slash_1->setVisible(false);

	duongQua->slash_2 = Sprite::create("Animation/DuongQua/slash2-2.png");
	duongQua->slash_2->setVisible(false);
	//

	return duongQua;
}

void DuongQua::createPool()
{
	poolTieuHonChuong = CCArray::createWithCapacity(5);		// 5
	poolTieuHonChuong->retain();

	for (int i = 0; i < 5; ++i) {
		auto thc = TieuHonChuong::create("Animation/DuongQua/tieuhonchuong.png");
		thc->setScale(this->getTrueRadiusOfHero() / thc->getContentSize().width);
		thc->setB2Body(nullptr);
		poolTieuHonChuong->addObject(thc);
	}

	indexOfTieuHonChuong = 0;
}

void DuongQua::createTieuHonChuong(Point posHand)
{
	auto thc = (TieuHonChuong*)poolTieuHonChuong->getObjectAtIndex(indexOfTieuHonChuong);
	auto world = this->getB2Body()->GetWorld();
	if (thc->getB2Body() != nullptr) {
		world->DestroyBody(thc->getB2Body());
	}

	thc->setVisible(true);
	thc->setPosition(posHand);
	thc->initCirclePhysic(world, thc->getPosition());
	thc->changeBodyCategoryBits(BITMASK_SWORD);
	thc->changeBodyMaskBits(BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2);
	

	this->getParent()->addChild(thc, 5);

	thc->setAngel(0);		//
	++indexOfTieuHonChuong;

	if (indexOfTieuHonChuong == 5) {
		indexOfTieuHonChuong = 0;
	}
}

void DuongQua::shoot()
{
	this->schedule([&](float dt) {
		if (checkCanShoot % 4 == 0)
			createTieuHonChuong(getBoneXLocation("bone47"));
		
		checkCanShoot++;

		if (checkCanShoot >= getDurationSkill3() * 10) {
			checkCanShoot = 0;
			unschedule("KeySkill3");
		}

	}, 0.1f, "KeySkill3");		//  run every 0.1 second
}

void DuongQua::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 6.7f / PTM_RATIO;

	// True radius of hero is here
	setTrueRadiusOfHero(circle_shape.m_radius * PTM_RATIO);
	//

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.filter.categoryBits = BITMASK_HERO;
	fixtureDef.filter.maskBits = BITMASK_HERO | BITMASK_FLOOR| BITMASK_WOODER | BITMASK_COIN | 
									BITMASK_TOANCHAN1 | BITMASK_SLASH;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);


	// connect sword with body
	initSwordPhysic(world, Point(pos.x + trueRadiusOfHero * 2.2f, pos.y), trueRadiusOfHero);
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
	this->setTimeScale(1.3f);
	clearTracks();
	addAnimation(0, "jumpx2", false);
	setToSetupPose();
}

void DuongQua::landing()
{
	clearTracks();
	addAnimation(0, "landing", true);
	setToSetupPose();
}

void DuongQua::die()
{
	clearTracks();
	addAnimation(0, "die", false);
	setToSetupPose();
	getB2Body()->SetLinearDamping(10);

}

void DuongQua::attackNormal()
{

	getSlash_2()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero() * 2,
		this->getPositionY() + this->getTrueRadiusOfHero());

	getSlash_2()->setVisible(true);

	clearTracks();
	auto r = rand() % 2;

	if (r) {
		addAnimation(0, "attack1", false);
	}
	else {
		addAnimation(0, "attack2", false);
	}


	setToSetupPose();
}

void DuongQua::attackLanding()
{
	getSlash_1()->setPosition(this->getPositionX() + this->getTrueRadiusOfHero() * 2,
		this->getPositionY());

	getSlash_1()->setVisible(true);

	clearTracks();
	addAnimation(0, "attack3", false);
	setToSetupPose();
}

void DuongQua::attackBySkill1()
{
	clearTracks();
	addAnimation(0, "skill1", false);
	setToSetupPose();
}

void DuongQua::attackBySkill2()
{
	clearTracks();
	addAnimation(0, "skill2", false);
	setToSetupPose();
}

void DuongQua::attackBySkill3()
{
	clearTracks();
	addAnimation(0, "skill3", false);
	setToSetupPose();

	//shoot();
}

void DuongQua::injured()
{
	this->setTimeScale(0.7f);
	clearTracks();
	addAnimation(0, "injured", false);
	setToSetupPose();
}

void DuongQua::die(Point posOfCammera)
{

}

void DuongQua::listener()
{
	// get Duration here
	this->setDurationSkill1(spSkeletonData_findAnimation(this->getSkeleton()->data, "skill1")->duration);
	this->setDurationSkill2(spSkeletonData_findAnimation(this->getSkeleton()->data, "skill2")->duration);
	this->setDurationSkill3(spSkeletonData_findAnimation(this->getSkeleton()->data, "skill3")->duration);


	this->setEndListener([&](int trackIndex) {
		if ((strcmp(getCurrent()->animation->name, "jumpx2") == 0)) {
			this->setTimeScale(1.0f);
			getFSM()->changeState(MLand);
		}

		else if ((strcmp(getCurrent()->animation->name, "injured") == 0)) {
			this->setTimeScale(1.0f);
			getFSM()->revertToGlobalState();
			setIsPrior(false);
		}

		else if ((strcmp(getCurrent()->animation->name, "attack1") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack2") == 0) ||
			(strcmp(getCurrent()->animation->name, "attack3") == 0)) {

			changeSwordCategoryBitmask(BITMASK_ENEMY);
			getSlash_1()->setVisible(false);
			getSlash_2()->setVisible(false);

			if (getFSM()->globalState != MDoubleJump)
				getFSM()->revertToGlobalState();
			else
				getFSM()->changeState(MLand);

			setIsPrior(false);

		} 
		
		else if (strcmp(getCurrent()->animation->name, "skill1") == 0) {
			setIsPriorSkill1(false);
		}

		else if (strcmp(getCurrent()->animation->name, "skill2") == 0) {
			setIsPriorSkill2(false);
		}

		else if (strcmp(getCurrent()->animation->name, "skill3") == 0) {
			setIsPriorSkill3(false);
		}

	});


	/*this->setCompleteListener([&](int trackIndex, int loopCount) {
		

	});*/
}

void DuongQua::updateMe(float dt)
{
	BaseHero::updateMe(dt);
	getFSM()->Update();


	auto currentVelY = getB2Body()->GetLinearVelocity().y;

	if (getFSM()->currentState == MDie) {
		getB2Body()->SetLinearVelocity(b2Vec2(0, currentVelY));
		return;
	}


	if (getPositionY() + getTrueRadiusOfHero() * 2 < 0) {
		getB2Body()->SetTransform(b2Vec2(SCREEN_SIZE.width * 0.25f / PTM_RATIO, SCREEN_SIZE.height / PTM_RATIO), getB2Body()->GetAngle());
		return;
	}

	getB2Body()->SetLinearVelocity(b2Vec2(getMoveVel(), currentVelY));

	if (getB2Body()) {
		if (!getIsPrior() && !getIsPriorSkill1() && !getIsPriorSkill2() && !getIsPriorSkill3()) {

			if (getB2Body()->GetLinearVelocity().y < 0) {
				if (getNumberOfJump() > 0)
					getFSM()->changeState(MLand);
				return;
			}

			if (getOnGround()) {
				getFSM()->changeState(MRun);
			}
		}
	}
}



