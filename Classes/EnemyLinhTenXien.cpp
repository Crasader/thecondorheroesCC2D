#include "EnemyLinhTenXien.h"
#include "BaseHero.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"



EnemyLinhTenXien::EnemyLinhTenXien(spSkeletonData*data) : EnemyLinhTenThang(data)
{
	controlAttack = 0;
}


EnemyLinhTenXien * EnemyLinhTenXien::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhTenXien(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINH_TEN_XIEN);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 15;
	enemy->autorelease();
	return enemy;
}



void EnemyLinhTenXien::attack(Vec2 posHero)
{
	playsoundAt();
	if (!this->getIsDie()) {
		this->clearTracks();
		this->addAnimation(0, "attack2", false);
		//this->splash->setVisible(true);
		this->setToSetupPose();
	}

	slash->setVisible(true);
	if (slash->getB2Body() != nullptr) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
	}
	auto thisToHero = posHero - (this->getPosition() + this->getParent()->getPosition());
	auto angle = 2.5*PI/4;
	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone17") + this->getParent()->getPosition());
	slash->getB2Body()->SetGravityScale(1.1f);
	slash->setAngle(angle,2);
	slash->setRotation(-angle / PI * 180 + 180);
}

void EnemyLinhTenXien::updateMe(BaseHero * hero)
{
	BaseEnemy::updateMe(hero);
	slash->updateMe(hero);
	if (slash->getIsDie()) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
		slash->setB2Body(nullptr);
		slash->setVisible(false);
		slash->setIsDie(false);
	}

	if (slash->getPositionX() < this->getPositionX() - SCREEN_SIZE.width * 3 / 4 && slash->isVisible()) {
		//slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x / PTM_RATIO, this->getBoneLocation("bone32").y / PTM_RATIO), 0);
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
		slash->setB2Body(nullptr);
		//slash->getB2Body()->SetLinearVelocity(b2Vec2(0,0));
		slash->setVisible(false);
	}
	auto thisToHero = hero->getPosition() - (this->getPosition() + this->getParent()->getPosition());
	if (thisToHero.x < 0) {
		if (-thisToHero.x < SCREEN_SIZE.width  && thisToHero.y < SCREEN_SIZE.height/2) {
			if (controlAttack % 80 == 0) {
				if (this->body != nullptr) {
					if (!this->body->GetWorld()->IsLocked()) {
						controlAttack = 1;	// 1 giay 1 nhat
						this->attack(hero->getPosition());
					}
				}
			}
			controlAttack++;
		}
	}

	if (getIsDie() && this->getB2Body() != nullptr) {
		die();
	}
}



