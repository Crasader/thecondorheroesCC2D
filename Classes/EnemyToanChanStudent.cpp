#include "EnemyToanChanStudent.h"
#include "manager/SkeletonManager.h"

EnemyToanChanStudent::EnemyToanChanStudent(spSkeletonData * data):BaseEnemy(data)
{
}

EnemyToanChanStudent::EnemyToanChanStudent(string jsonFile, string atlasFile, float scale):BaseEnemy(jsonFile, atlasFile,scale)
{
}

EnemyToanChanStudent * EnemyToanChanStudent::create(string jsonFile, string atlasFile, float scale)
{
	EnemyToanChanStudent *enemy = new EnemyToanChanStudent(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	return enemy;

}


EnemyToanChanStudent * EnemyToanChanStudent::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyToanChanStudent(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	return enemy;
}

void EnemyToanChanStudent::run()
{

}

void EnemyToanChanStudent::attack()
{
	if (!this->getIsDie()) {
		this->clearTracks();
		this->addAnimation(0, "attack", false);
		//this->splash->setVisible(true);
		this->setToSetupPose();
	}
}

void EnemyToanChanStudent::die()
{
	auto world = this->body->GetWorld();
	world->DestroyBody(this->body);
	this->body = nullptr;
	this->setIsDie(true);
	this->clearTracks();
	this->setAnimation(0,"die",false);
	this->setToSetupPose();
}

void EnemyToanChanStudent::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 4 / PTM_RATIO;

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


//void EnemyToanChanStudent::genSplash()
//{
//	splash = Sprite::create("Animation/Enemy-DeTuToanChan1/slashenemy.png");
//	splash->setScale(SCREEN_SIZE.height/5/splash->getContentSize().height);
//	splash->setAnchorPoint(Point(1, 0));
//	
//	splash->setPosition(-this->getBody()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO, 0);
//	splash->setVisible(false);
//	this->addChild(splash);
//}

void EnemyToanChanStudent::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1) {
			//getSplash()->setVisible(false);
			//setIsAttacking(false);
			this->clearTracks();
			this->addAnimation(0, "idle", true);
			this->setToSetupPose();
		}
		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			this->removeFromParentAndCleanup(true);
		}

	});
}
