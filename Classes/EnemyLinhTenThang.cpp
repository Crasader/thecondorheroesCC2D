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





void EnemyLinhTenThang::playsoundAt()
{
	AudioManager::playSound(SOUND_HD2AT);
}

void EnemyLinhTenThang::playsoundDie()
{
	AudioManager::playSound(SOUND_HD2DIE);
}


