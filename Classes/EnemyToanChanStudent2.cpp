#include "EnemyToanChanStudent2.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"

EnemyToanChanStudent2::EnemyToanChanStudent2(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
	controlAttack = 100;
}

EnemyToanChanStudent2::EnemyToanChanStudent2(spSkeletonData*data):EnemyToanChanStudent(data)
{
	controlAttack = 100;
}

EnemyToanChanStudent2 * EnemyToanChanStudent2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyToanChanStudent2 *enemy = new EnemyToanChanStudent2(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 15;
	enemy->autorelease();
	return enemy;

}

EnemyToanChanStudent2 * EnemyToanChanStudent2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyToanChanStudent2(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 15;
	enemy->setControlAttack(115);
	enemy->autorelease();
	return enemy;
}

void EnemyToanChanStudent2::attack()
{
	//EnemyToanChanStudent::attack();
	playsoundAt();
	if (!this->getIsDie()) {
		this->clearTracks();
		this->addAnimation(0, "attack", false);
		//this->splash->setVisible(true);
		this->setToSetupPose();
	}
	/*slash->getB2Body()->SetTransform(b2Vec2((this->getBoneLocation("bone32").x+this->getParent()->getPosition().x)/PTM_RATIO,
		(this->getBoneLocation("bone32").y+this->getParent()->getPosition().y)/PTM_RATIO),0);*/
	slash->setVisible(true);
	if (slash->getB2Body() != nullptr) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
	}
	slash->setAnimation(0, "animation", true);
	slash->resume();
	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("b-hand") + this->getParent()->getPosition());
	//slash->setVisible(false);
	slash->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width/3/PTM_RATIO,0));
}

void EnemyToanChanStudent2::die()
{
	playsoundDie();
	//EnemyToanChanStudent::die();
	BaseEnemy::die();

	auto world = this->body->GetWorld();
	world->DestroyBody(this->body);
	this->body = nullptr;
	this->setIsDie(false);
	this->clearTracks();
	this->setAnimation(0, "die", false);
	this->setToSetupPose();

	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
		slash->setB2Body(nullptr);
		slash->setVisible(false);
		slash->setIsDie(false);
		//slash->removeFromParentAndCleanup(true);

	}

}

void EnemyToanChanStudent2::genSlash()
{
	slash = Slash::create("Animation/Enemy_DeTuToanChan2/detu2slash.json", "Animation/Enemy_DeTuToanChan2/detu2slash.atlas",scaleEnemy*0.8f);
	//slash->updateWorldTransform();
	//slash->setAnchorPoint(Point(0.5f, 0.4f));
	
	slash->setPosition(this->getBoneLocation("b-hand"));
	slash->setVisible(true);
	this->getParent()->addChild(slash,ZORDER_ENEMY);
}

void EnemyToanChanStudent2::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1) {
			//getSlash()->setVisible(false);
			//setIsAttacking(false);
			this->clearTracks();
			this->addAnimation(0, "idle", true);
			this->setToSetupPose();
			//if(this->getB2Body())
			//this->getSlash()->setVisible(true);
		}
		//if (strcmp(getCurrent()->animation->name, "attack1") == 0 && loopCount == 1) {
		//	//getSlash()->setVisible(false);
		//	//setIsAttacking(false);
		//	this->clearTracks();
		//	this->addAnimation(0, "idle", true);
		//	this->setToSetupPose();
		//}
		//if (strcmp(getCurrent()->animation->name, "attack2") == 0 && loopCount == 1) {
		//	//getSlash()->setVisible(false);
		//	//setIsAttacking(false);
		//	this->clearTracks();
		//	this->addAnimation(0, "idle", true);
		//	this->setToSetupPose();
		//}
		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {

			this->setVisible(false);
			/*this->clearTracks();
			this->setAnimation(0, "idle", true);
			this->setToSetupPose();*/
			this->pauseSchedulerAndActions();
		}

	});
}

void EnemyToanChanStudent2::updateMe(BaseHero* hero)
{
	BaseEnemy::updateMe(hero);
	slash->updateMe(hero);
	if (slash->getIsDie()) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
		slash->setB2Body(nullptr);
		slash->setVisible(false);
		slash->setIsDie(false);
	}

	if (slash->getPositionX() < this->getPositionX() - SCREEN_SIZE.width*3/4 && slash->isVisible()) {
		//slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x / PTM_RATIO, this->getBoneLocation("bone32").y / PTM_RATIO), 0);
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
		slash->setB2Body(nullptr);
		//slash->getB2Body()->SetLinearVelocity(b2Vec2(0,0));
		slash->setVisible(false);
	}
	controlAttack++;
	if (controlAttack > 120) {
		if (this->body != nullptr) {
			if (!this->body->GetWorld()->IsLocked()) {
				controlAttack = 0;	// 2 giay 1 nhat
				this->attack();
			}
		}
	}

	if (getIsDie() && this->getB2Body() != nullptr) {
		die();
	}
}

void EnemyToanChanStudent2::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = this;			// pass sprite to bodyDef with argument: userData

	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void EnemyToanChanStudent2::playsoundAt()
{
	AudioManager::playSound(SOUND_TC2AT);
}

void EnemyToanChanStudent2::playsoundDie()
{
	AudioManager::playSound(SOUND_TC2DIE);
}



//void EnemyToanChanStudent2::removeFromParentAndCleanup(bool cleanup)
//{
//	BaseEnemy::removeAllChildrenWithCleanup(cleanup);
//	auto world = slash->getB2Body()->GetWorld();
//	world->DestroyBody(slash->getB2Body());
//	slash->removeAllChildrenWithCleanup(cleanup);
//	log("delete slash");
//}

//void EnemyToanChanStudent2::onExit()
//{
//	BaseEnemy::removeFromParentAndCleanup(cleanup);
//	auto world = slash->getB2Body()->GetWorld();
//	world->DestroyBody(slash->getB2Body());
//	slash->removeFromParentAndCleanup(cleanup);
//	log("delete slash");
//}

void EnemyToanChanStudent2::updatePos()
{
	BaseEnemy::updatePos();
}

void EnemyToanChanStudent2::onExit()
{
	BaseEnemy::onExit();
	/*auto world = slash->getB2Body()->GetWorld();
	world->DestroyBody(slash->getB2Body());
	log("delete slash");*/
}

void EnemyToanChanStudent2::prepare()
{
	BaseEnemy::prepare();
}

