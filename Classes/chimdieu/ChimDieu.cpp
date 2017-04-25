#include "ChimDieu.h"

ChimDieu::ChimDieu(spSkeletonData * data) :B2Skeleton(data) {
}

ChimDieu::ChimDieu(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale) {
	isCarry = false;
}

ChimDieu * ChimDieu::create(string jsonFile, string atlasFile, float scale) {
	ChimDieu *_pEagle = new ChimDieu(jsonFile, atlasFile, scale);
	_pEagle->setAnimation(0, "fly", true);
	_pEagle->setSkin("Free");
	_pEagle->setSlotsToSetupPose();
	_pEagle->update(0.0f);
	_pEagle->setTag(TAG_EAGLE);
	_pEagle->isUp = false;
	_pEagle->isDown = false;
	_pEagle->setVisible(false);
	_pEagle->sequenceCloud = 0.9f;
	return _pEagle;
}

ChimDieu * ChimDieu::create(spSkeletonData * data) {
	return nullptr;
}

void ChimDieu::updateMe(float dt) {
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}
	else
		return;


	if (isCarry && isUp && body->GetPosition().y > SCREEN_SIZE.height * 2.0f / PTM_RATIO) {
		this->getB2Body()->SetLinearVelocity(b2Vec2(this->getB2Body()->GetLinearVelocity().x, 0.0f));
	}
	if (isCarry && isDown && body->GetPosition().y < SCREEN_SIZE.height * 0.3f / PTM_RATIO) {
		this->getB2Body()->SetLinearVelocity(b2Vec2(this->getB2Body()->GetLinearVelocity().x, 0.0f));
	}
	if (isCarry) {
		if (sequenceCloud < 0.2f) {
			sequenceCloud = 0.2f;
		}
		if (sequenceCloud > 0.9f) {
			sequenceCloud = 0.9f;
		}
		if (CCRANDOM_0_1() > sequenceCloud) {
			float _fCloudMaxScale = 0.5f;
			float _fCloudMinScale = 0.2f;
			float _fCloudScale = (_fCloudMinScale + CCRANDOM_0_1() * (_fCloudMaxScale - _fCloudMinScale));
			if (CCRANDOM_0_1() > 0.4f) {
				auto _aTempCloud = Sprite::create("cloud.png");
				_aTempCloud->setScale(SCREEN_SIZE.width / _aTempCloud->getContentSize().width * _fCloudScale);
				_aTempCloud->setPositionX(this->getB2Body()->GetPosition().x + SCREEN_SIZE.width * CCRANDOM_0_1());
				_aTempCloud->setPositionY(this->getB2Body()->GetPosition().y + SCREEN_SIZE.height / 2 * (CCRANDOM_0_1() - 0.5f));
				this->addChild(_aTempCloud, CCRANDOM_0_1() > 0.3f ? 1 : -1);

				MoveBy *_pMoveCloud = MoveBy::create(CCRANDOM_0_1() + 0.2f, Vec2(-SCREEN_SIZE.width * 3, 0.0f));
				auto _aMoveCloudCallback = CallFunc::create([&]() {
				});
				_aTempCloud->runAction(Sequence::create(_pMoveCloud, _aMoveCloudCallback, NULL));
			}
			else {
				auto _aTempLight = Sprite::create("light.png");
				_aTempLight->setScale(SCREEN_SIZE.height / _aTempLight->getContentSize().height * 0.02f);
				_aTempLight->setPositionX(this->getB2Body()->GetPosition().x + SCREEN_SIZE.width * CCRANDOM_0_1());
				_aTempLight->setPositionY(this->getB2Body()->GetPosition().y + SCREEN_SIZE.height / 2 * (CCRANDOM_0_1() - 0.5f));
				this->addChild(_aTempLight, CCRANDOM_0_1() > 0.3f ? 1 : -1);

				MoveBy *_pMoveLight = MoveBy::create(CCRANDOM_0_1() + 0.2f, Vec2(-SCREEN_SIZE.width * 3, 0.0f));
				auto _aMoveLightCallback = CallFunc::create([&]() {
				});
				_aTempLight->runAction(Sequence::create(_pMoveLight, _aMoveLightCallback, NULL));
			}
		}
	}
	else {
		if (body->GetPosition().y > SCREEN_SIZE.height * 3.0f / PTM_RATIO) {
			// this->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			this->getB2Body()->GetWorld()->DestroyBody(this->getB2Body());
			this->setB2Body(nullptr);
			this->removeFromParentAndCleanup(true);
		}
	}
}

void ChimDieu::initCirclePhysic(b2World * world, Point pos) {
    b2CircleShape circle_shape;
    //circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
    this->getBoundingBox().size.height > this->getBoundingBox().size.width ? circle_shape.m_radius = this->getBoundingBox().size.width / 6 / PTM_RATIO :
    circle_shape.m_radius = this->getBoundingBox().size.height / 6 / PTM_RATIO;
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
    
	this->getB2Body()->SetType(b2_dynamicBody);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_BIRD);
	this->changeBodyMaskBits(0);
}

void ChimDieu::flyUp(b2Vec2 p_b2v2Velocity) {
	this->setSkin(stringHero);
	this->setSlotsToSetupPose();
	this->getB2Body()->SetLinearVelocity(p_b2v2Velocity);
	this->isCarry = true;
	this->isUp = true;
	this->isDown = false;
}

void ChimDieu::flyDown(b2Vec2 p_b2v2Velocity) {
	this->getB2Body()->SetLinearVelocity(p_b2v2Velocity);
	this->isUp = false;
	this->isDown = true;
	this->isAbleToDropHero = false;
}

void ChimDieu::flyAway() {
	this->setSkin("Free");
	this->setSlotsToSetupPose();
	this->getB2Body()->SetLinearVelocity(b2Vec2(15.0f, 5.0f));
	this->isCarry = false;
	this->isUp = false;
	this->isDown = false;
	this->removeAllChildrenWithCleanup(true);
}
