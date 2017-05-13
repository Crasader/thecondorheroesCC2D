#include "EnemyHongLangBa2.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"
#include "BaseHero.h"


EnemyHongLangBa2::EnemyHongLangBa2(string jsonFile, string atlasFile, float scale):EnemyHongLangBa(jsonFile, atlasFile,scale)
{
	controlAttack = 100;
	//isDie = false;
}

EnemyHongLangBa2::EnemyHongLangBa2(spSkeletonData*data):EnemyHongLangBa(data)
{
	controlAttack = 100;
}


EnemyHongLangBa2 * EnemyHongLangBa2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyHongLangBa2 *enemy = new EnemyHongLangBa2(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 15;
	return enemy;

}

EnemyHongLangBa2 * EnemyHongLangBa2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyHongLangBa2(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 15;
	return enemy;
}

void EnemyHongLangBa2::attack()
{
	EnemyHongLangBa::attack();
	AudioManager::playSound(SOUND_HLB2AT);
	/*slash->getB2Body()->SetTransform(b2Vec2((this->getBoneLocation("bone32").x+this->getParent()->getPosition().x)/PTM_RATIO,
		(this->getBoneLocation("bone32").y+this->getParent()->getPosition().y)/PTM_RATIO),0);*/
	enemyDarts->setVisible(true);
	if (enemyDarts->getB2Body() != nullptr) {
		enemyDarts->getB2Body()->GetWorld()->DestroyBody(enemyDarts->getB2Body());
	}
	enemyDarts->setRotation(180 - angle / PI * 180);
	enemyDarts->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone32") + this->getParent()->getPosition());

	float vx = SCREEN_SIZE.width / 1.9f / PTM_RATIO * cosf(angle);
	float vy = SCREEN_SIZE.width / 1.9f / PTM_RATIO * sinf(angle);

	enemyDarts->getB2Body()->SetLinearVelocity(b2Vec2(vx, vy));
}

void EnemyHongLangBa2::die()
{
	EnemyHongLangBa::die();
	AudioManager::playSound(SOUND_HLB2DIE);
	if (enemyDarts->getB2Body()!=nullptr) {
		auto world = enemyDarts->getB2Body()->GetWorld();
		world->DestroyBody(enemyDarts->getB2Body());
		enemyDarts->setB2Body(nullptr);
		//enemyDarts->removeFromParentAndCleanup(true);
		enemyDarts->setVisible(false);
		enemyDarts->setIsDie(false);
	}

}

void EnemyHongLangBa2::genDarts()
{
	enemyDarts = Darts::create("Animation/Enemy_HongLangBa2/HLB2-amkhi.json", "Animation/Enemy_HongLangBa2/HLB2-amkhi.atlas", scaleEnemy*2.0f);
	
	enemyDarts->setPosition(this->getBoneLocation("bone32"));
	enemyDarts->setVisible(false);
	this->getParent()->addChild(enemyDarts,ZORDER_ENEMY);
}

void EnemyHongLangBa2::listener()
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
			//this->removeFromParentAndCleanup(true);
			this->setVisible(false);
			/*this->clearTracks();
			this->setAnimation(0, "idle", true);
			this->setToSetupPose();*/
			this->pauseSchedulerAndActions();
		}

	});
}

void EnemyHongLangBa2::updateMe(BaseHero* hero)
{
	BaseEnemy::updateMe(hero);
	enemyDarts->updateMe(hero);
	if (enemyDarts->getIsDie()) {
		enemyDarts->getB2Body()->GetWorld()->DestroyBody(enemyDarts->getB2Body());
		enemyDarts->setB2Body(nullptr);
		enemyDarts->setVisible(false);
		enemyDarts->setIsDie(false);
	}

	if (enemyDarts->getPositionX() < this->getPositionX() - SCREEN_SIZE.width*3/4 && enemyDarts->isVisible()) {
		//slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x / PTM_RATIO, this->getBoneLocation("bone32").y / PTM_RATIO), 0);
		enemyDarts->getB2Body()->GetWorld()->DestroyBody(enemyDarts->getB2Body());
		enemyDarts->setB2Body(nullptr);
		//slash->getB2Body()->SetLinearVelocity(b2Vec2(0,0));
		enemyDarts->setVisible(false);
	}

	
	controlAttack++;
	if (hero->getPositionX() < this->getPositionX() + this->getParent()->getPositionX() && 
		hero->getPositionX() > this->getPositionX() + this->getParent()->getPositionX() - SCREEN_SIZE.width * 0.6f &&
		hero->getPositionY() + SCREEN_SIZE.height * 0.33f > this->getPositionY() &&
		hero->getPositionY() - SCREEN_SIZE.height * 0.33f < this->getPositionY()) {

		Vec2 vector = Vec2(hero->getPosition() - (this->getPosition() + this->getParent()->getPosition()));
		angle = vector.getAngle();
		

		if (controlAttack > 120 && !hero->getIsDriverEagle()) {
			if (this->body != nullptr) {
				if (!this->body->GetWorld()->IsLocked()) {
					controlAttack = 0;	// 2 giay 1 nhat
					this->attack();
				}
			}
		}
	}
	if (getIsDie() && this->getB2Body() != nullptr) {
		die();
	}
}

void EnemyHongLangBa2::initCirclePhysic(b2World * world, Point pos)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = this->getBoundingBox().size.height / 2 / PTM_RATIO;
//	log("hlb2: boundingbox height: %f", this->getBoundingBox().size.height);

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

void EnemyHongLangBa2::updatePos()
{
	BaseEnemy::updatePos();
}


void EnemyHongLangBa2::onExit()
{
	BaseEnemy::onExit();
}
