#include "EnemyBoss1.h"

EnemyBoss1::EnemyBoss1(string jsonFile, string atlasFile, float scale):BaseEnemy(jsonFile,atlasFile,scale)
{
	control = 0;
	controlAttack = 2;
	hp = 15;
	baseVelocity =Vec2(SCREEN_SIZE.width/2.3f, SCREEN_SIZE.height/10);
	moveVelocity = Vec2(SCREEN_SIZE.height/2,SCREEN_SIZE.height/2);
	realtimeVec = Vec2(SCREEN_SIZE.width / 2.3f, SCREEN_SIZE.height / 10);
	realMoveVelocity = Vec2::ZERO;
}

EnemyBoss1 * EnemyBoss1::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss1* boss = new EnemyBoss1(jsonFile, atlasFile, scale);
	boss->setAnimation(0,"idle",true);
	boss->update(0.0f);
	boss->state = new Boss1Idling();
	boss->setTag(TAG_BOSS);
	return boss;
}

void EnemyBoss1::idle()
{
	//this->realtimeVec = baseVelocity;
	state->idle(this);
}

void EnemyBoss1::attack()
{
	state->attack1(this);
}

void EnemyBoss1::attack2()
{
	state->attack2(this);
}

void EnemyBoss1::stupid()
{
	state->stupid(this);
}

void EnemyBoss1::fixStupid()
{
	state->fixStupid(this);
	srand(time(NULL));
	controlAttack = rand()%4+1;
}

void EnemyBoss1::die()
{

}

void EnemyBoss1::createPool()
{
	slashPool = CCArray::createWithCapacity(3);
	slashPool->retain();
	for (int i = 0; i < 3; i++) {
		auto slash = SlashBoss::create("Animation/Enemy_Boss1/slash.png");
		slash->setVisible(false);
		slash->setPosition(this->getPosition());
		slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		this->getParent()->addChild(slash, ZORDER_ENEMY);
		auto tmpbody = slash->getB2Body();
		tmpbody = nullptr;
		slashPool->addObject(slash);
	}

	indexSlash = 0;
}

void EnemyBoss1::creatSlash()
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setVisible(true);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world,this->getBoneLocation("bone65"));
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO);
	slash->setRotation(180 - 180 / 4);
	slash->setAngel(PI + PI/4);
}

void EnemyBoss1::updateMe(Point posHero)
{
	this->heroLocation = posHero;
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}


	// update van toc
	/*if (this->getPositionY() <= posHero.y || this->getPositionY() >= SCREEN_SIZE.height/4*3) {
		realMoveVelocity.y = 0;
	}

	if (this->getPositionY() >= SCREEN_SIZE.height / 4 * 3) {
		this->idle();
	}*/

	state->updateVec(this);
	
	this->getB2Body()->SetLinearVelocity(b2Vec2(this->realtimeVec.x / PTM_RATIO, this->realtimeVec.y*cosf(control / 120.0f * 2 * PI) / PTM_RATIO) +
		b2Vec2(realMoveVelocity.x / PTM_RATIO, realMoveVelocity.y / PTM_RATIO));
	//////////////
	control++;
	if (control > maxControl) {
		control = 0;
	}
	if (control == 240) {
		this->attack2();
	}

	if (control == 255 || control == 270|| control == 285) {
		this->creatSlash();
	}

	if (control == 360) {
		this->stupid();
	}
	
	
	if (this->getPositionX() >= posHero.x + SCREEN_SIZE.width/2) {
		this->idle();
	}

	for (int i = 0; i < slashPool->count(); i++) {
		auto slash = (SlashBoss*)slashPool->getObjectAtIndex(i);
		slash->updateMe(0.0f);
		if (slash->getB2Body() != nullptr && slash->getPositionX() < posHero.x - SCREEN_SIZE.width / 2) {
			slash->die();
		}
	}
}

void EnemyBoss1::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if ((strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1)) {
			idle();
		}
		else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
			this->setControlAttack(this->getControlAttack() - 1);
			if(this->getControlAttack() == 0)
			fixStupid();
		}
	});
}

bool EnemyBoss1::checkStop()
{
	if (this->getPositionX() - heroLocation.x > SCREEN_SIZE.width/1.8f) {
		return true;
	}
	return false;
}

