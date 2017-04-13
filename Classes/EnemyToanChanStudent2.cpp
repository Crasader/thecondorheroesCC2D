#include "EnemyToanChanStudent2.h"
#include "manager/SkeletonManager.h"


EnemyToanChanStudent2::EnemyToanChanStudent2(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
	controlAttack = 60;
	//isDie = false;
}

EnemyToanChanStudent2::EnemyToanChanStudent2(spSkeletonData*data):EnemyToanChanStudent(data)
{
	controlAttack = 60;
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
	return enemy;
}

void EnemyToanChanStudent2::attack()
{
	EnemyToanChanStudent::attack();
	slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x/PTM_RATIO, this->getBoneLocation("bone32").y/PTM_RATIO),0);
	slash->setVisible(true);
	slash->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width/3/PTM_RATIO,0));
}

void EnemyToanChanStudent2::die()
{
	EnemyToanChanStudent::die();
	auto world = slash->getB2Body()->GetWorld();
	world->DestroyBody(slash->getB2Body());
	slash->removeFromParentAndCleanup(true);

}

void EnemyToanChanStudent2::genSlash()
{
	slash = Slash::create("Animation/Enemy_DeTuToanChan2/detu2slash.json", "Animation/Enemy_DeTuToanChan2/detu2slash.atlas",scaleEnemy*0.8f);
	//slash->updateWorldTransform();
	//slash->setAnchorPoint(Point(0.5f, 0.4f));
	
	slash->setPosition(this->getBoneLocation("bone32"));
	slash->setVisible(false);
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
		}

		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			//slash->removeFromParentAndCleanup(true);
			this->removeFromParentAndCleanup(true);
		}

	});
}

void EnemyToanChanStudent2::updateMe(float dt)
{
	BaseEnemy::updateMe(dt);
	slash->updateMe(dt);
	if (slash->getIsDie()) {
		slash->getB2Body()->SetTransform(b2Vec2(-10, -10), 0);
		slash->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
		slash->setVisible(false);
		slash->setIsDie(false);
	}
	if (slash->getPositionX() < this->getPositionX() - SCREEN_SIZE.width) {
		//slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x / PTM_RATIO, this->getBoneLocation("bone32").y / PTM_RATIO), 0);
		slash->getB2Body()->SetTransform(b2Vec2(-10,-10), 0);
		slash->getB2Body()->SetLinearVelocity(b2Vec2(0,0));
		slash->setVisible(false);
	}
	controlAttack++;
	if (controlAttack > 120) {
		controlAttack = 0;	// 2 giay 1 nhat
		this->attack();
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

void EnemyToanChanStudent2::onExit()
{
	BaseEnemy::onExit();
	/*auto world = slash->getB2Body()->GetWorld();
	world->DestroyBody(slash->getB2Body());
	log("delete slash");*/
}
