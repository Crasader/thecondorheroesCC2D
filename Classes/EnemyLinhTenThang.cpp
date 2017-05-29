#include "EnemyLinhTenThang.h"
#include "BaseHero.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"



EnemyLinhTenThang::EnemyLinhTenThang(spSkeletonData*data) : EnemyHoacDo2(data)
{
	controlAttack = 0;
}


EnemyLinhTenThang * EnemyLinhTenThang::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhTenThang(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINH_TEN_THANG);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 15;
	enemy->autorelease();
	return enemy;
}

void EnemyLinhTenThang::genSlash()
{
	slash = Slash::create("Animation/Enemy_LinhBanTen/LBC-ten1.json", "Animation/Enemy_LinhBanTen/LBC-ten1.atlas", scaleEnemy);
	//slash->updateWorldTransform();
	//slash->setAnchorPoint(Point(0.5f, 0.4f));

	slash->setPosition(this->getBoneLocation("bone17"));
	slash->setVisible(false);
	this->getParent()->addChild(slash, ZORDER_ENEMY);
}

void EnemyLinhTenThang::attack(Vec2 posHero)
{
	playsoundAt();
	if (!this->getIsDie()) {
		this->clearTracks();
		this->addAnimation(0, "attack1", false);
		//this->splash->setVisible(true);
		this->setToSetupPose();
	}

	slash->setVisible(true);
	if (slash->getB2Body() != nullptr) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
	}
	auto thisToHero = posHero - (this->getPosition()+this->getParent()->getPosition());
	auto angle = -PI;
	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone17") + this->getParent()->getPosition());
	slash->setAngle(-PI,1);
	slash->setRotation(-angle / PI * 180 + 180);
}

void EnemyLinhTenThang::updateMe(BaseHero * hero)
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
		if (-thisToHero.x < SCREEN_SIZE.width  && thisToHero.y < SCREEN_SIZE.height / 2) {
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





void EnemyLinhTenThang::playsoundAt()
{
	AudioManager::playSound(SOUND_HD2AT);
}

void EnemyLinhTenThang::playsoundDie()
{
	AudioManager::playSound(SOUND_HD2DIE);
}


