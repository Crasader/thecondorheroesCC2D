#include "EnemyBoss1.h"
#include "MenuScene.h"

EnemyBoss1::EnemyBoss1(string jsonFile, string atlasFile, float scale):BaseEnemy(jsonFile,atlasFile,scale)
{
	isNodie = false;
	isDie = false;
	control = 0;
	controlAttack = 2;
	controlState = 0;
	hp = 5;
	baseVelocity =Vec2(SCREEN_SIZE.width/2.3f, SCREEN_SIZE.height/10);
	moveVelocity = Vec2(SCREEN_SIZE.height/2,SCREEN_SIZE.height/2);
	realtimeVec = Vec2(SCREEN_SIZE.width / 2.3f, SCREEN_SIZE.height / 10);
	realMoveVelocity = Vec2::ZERO;
	//lockState = false;
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
	this->clearTracks();
	this->setAnimation(0, "idle", true);
	this->setToSetupPose();
}

void EnemyBoss1::attack()
{
	this->isNodie = true;
	this->clearTracks();
	this->setAnimation(0,"attack",false);
	this->creatHidenSlash((heroLocation-this->getPosition()).getAngle());
}

void EnemyBoss1::attack2()
{
	this->isNodie = true;
	this->clearTracks();
	this->setAnimation(0, "attack2", false);
}



void EnemyBoss1::die()
{
	if (!isDie && !isNodie) {
		hp--;
		if (hp > 0) {
			this->isNodie = true;
			this->clearTracks();
			this->setAnimation(0, "injured", false);
			this->setToSetupPose();
		}
		else {
			this->isNodie = true;
			this->clearTracks();
			this->setAnimation(0, "injured-red", false);
			this->setToSetupPose();
		}
		if (hp <= 0) {
			spHp->setVisible(false);
			//isDie = true;
		}
		else {
			auto scale1 = spHp->getScaleX();
			auto scale2 = ((float)this->hp / (float)(this->hp + 1))*spHp->getScaleX();
			spHp->setScaleX(scale2);

		}
	}
}

void EnemyBoss1::createPool()
{
	slashPool = CCArray::createWithCapacity(3);
	slashPool->retain();
	for (int i = 0; i < 3; i++) {
		auto scale = SCREEN_SIZE.height /5 / (367/2);
		auto slash = SlashBoss::create("Animation/Enemy_Boss1/skill-boss.json", "Animation/Enemy_Boss1/skill-boss.atlas",scale);
		slash->setVisible(false);
		slash->setPosition(this->getPosition());
		//slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		this->getParent()->addChild(slash, ZORDER_ENEMY);
		auto tmpbody = slash->getB2Body();
		tmpbody = nullptr;
		slashPool->addObject(slash);
	}

	indexSlash = 0;
}

void EnemyBoss1::creatSlash(float angel)
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
	slash->changeBodyMaskBits(BITMASK_HERO|BITMASK_SWORD);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngel(angel);
	slash->setRotation(-angel / PI * 180 + 180);
	slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss1::creatHidenSlash(float angel)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setVisible(false);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world,this->getPosition());
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngel(angel);
	//slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss1::creatHpSprite()
{
	spHp = Sprite::create("UI/hp.png");
	auto tmp = SCREEN_SIZE.width / 6 / spHp->getContentSize().width;
	spHp->setScaleX(SCREEN_SIZE.width/6/spHp->getContentSize().width);
	spHp->setScaleY(SCREEN_SIZE.height / 100 / spHp->getContentSize().height);
	spHp->setPosition(0,0);
	this->addChild(spHp);
	spHp->update(0.0f);
}

void EnemyBoss1::updateMe(Point posHero)
{
	this->heroLocation = posHero;
	if (body != nullptr) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO);
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO);
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
	}

	state->execute(this);
	
	this->getB2Body()->SetLinearVelocity(b2Vec2(this->realtimeVec.x / PTM_RATIO, this->realtimeVec.y*cosf(control / 120.0f * 2 * PI) / PTM_RATIO) +
		b2Vec2(realMoveVelocity.x / PTM_RATIO, realMoveVelocity.y / PTM_RATIO));
	//////////////
	control++;
	if (control == maxControl) {
		control = 1;
	}
	
	

	for (int i = 0; i < slashPool->count(); i++) {
		auto slash = (SlashBoss*)slashPool->getObjectAtIndex(i);
		slash->updateMe(0.0f);
		if (slash->getB2Body() != nullptr && slash->getPositionX() < posHero.x ) {
			slash->die();
		}
	}
}

void EnemyBoss1::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (getCurrent()) {
			if ((strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1)) {
				this->idle();
				setIsNodie(false);
			}
			else if ((strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1)) {
				this->idle();
				setIsNodie(false);
			}
			else if ((strcmp(getCurrent()->animation->name, "injured") == 0 && loopCount == 1)) {
				setIsNodie(false);
				this->idle();
			}
			else if ((strcmp(getCurrent()->animation->name, "injured-red") == 0 && loopCount == 1)) {
				//Director::getInstance()->replaceScene(MenuLayer::createScene());
				setIsDie(true);
			}
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

void EnemyBoss1::changeState(StateBoss1 * state)
{
	auto tmp = this->state;
	this->state = state;
	state->enter(this);
	delete tmp;
}

