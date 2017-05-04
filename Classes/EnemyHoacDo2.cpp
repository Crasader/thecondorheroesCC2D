#include "EnemyHoacDo2.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"

EnemyHoacDo2::EnemyHoacDo2(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent2(jsonFile, atlasFile,scale)
{
	controlAttack = 100;
}

EnemyHoacDo2::EnemyHoacDo2(spSkeletonData*data):EnemyToanChanStudent2(data)
{
	controlAttack = 100;
}

EnemyHoacDo2 * EnemyHoacDo2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyHoacDo2 *enemy = new EnemyHoacDo2(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_HOACDO2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 15;
	return enemy;

}

EnemyHoacDo2 * EnemyHoacDo2::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyHoacDo2(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_HOACDO2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 15;
	return enemy;
}

void EnemyHoacDo2::genSlash()
{
	slash = Slash::create("Animation/Enemy_HoacDo2/HD2-amkhi.json", "Animation/Enemy_HoacDo2/HD2-amkhi.atlas", scaleEnemy);
	//slash->updateWorldTransform();
	//slash->setAnchorPoint(Point(0.5f, 0.4f));

	slash->setPosition(this->getBoneLocation("bone22"));
	slash->setVisible(false);
	this->getParent()->addChild(slash, ZORDER_ENEMY);
}

void EnemyHoacDo2::attack()
{
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
	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone22") + this->getParent()->getPosition());
	slash->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width / 3 / PTM_RATIO, 0));
}

void EnemyHoacDo2::playsoundAt()
{
	AudioManager::playSound(SOUND_HD2AT);
}

void EnemyHoacDo2::playsoundDie()
{
	AudioManager::playSound(SOUND_HD2DIE);
}


