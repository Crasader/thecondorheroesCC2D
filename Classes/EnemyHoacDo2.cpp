#include "EnemyHoacDo2.h"
#include "BaseHero.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"

EnemyHoacDo2::EnemyHoacDo2(string jsonFile, string atlasFile, float scale) :EnemyToanChanStudent2(jsonFile, atlasFile, scale)
{
	controlAttack = 0;
}

EnemyHoacDo2::EnemyHoacDo2(spSkeletonData*data) : EnemyToanChanStudent2(data)
{
	controlAttack = 0;
}

EnemyHoacDo2 * EnemyHoacDo2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyHoacDo2 *enemy = new EnemyHoacDo2(jsonFile, atlasFile, scale);
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

void EnemyHoacDo2::attack(Vec2 posHero)
{
	playsoundAt();
	if (!this->getIsDie()) {
		this->clearTracks();
		this->addAnimation(0, "attack", false);
		//this->splash->setVisible(true);
		this->setToSetupPose();
	}

	slash->setVisible(true);
	if (slash->getB2Body() != nullptr) {
		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
	}
	auto thisToHero = posHero - (this->getPosition()+this->getParent()->getPosition());
	auto angle = thisToHero.getAngle();
	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone22") + this->getParent()->getPosition());
	slash->setAngle(angle);
	slash->setRotation(-angle / PI * 180 + 180);
}

void EnemyHoacDo2::updateMe(BaseHero * hero)
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
	auto thisToHero = hero->getPosition() - (this->getPosition()+ this->getParent()->getPosition());
	if (thisToHero.x < 0) {
		if (-thisToHero.x < SCREEN_SIZE.width *0.6f && fabs(thisToHero.y) <SCREEN_SIZE.height/2.2f ) {
			if (controlAttack % 120 == 0) {
				if (this->body != nullptr) {
					if (!this->body->GetWorld()->IsLocked()) {
						controlAttack = 1;	// 2 giay 1 nhat
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

//void EnemyHoacDo2::attack()
//{
//	playsoundAt();
//	if (!this->getIsDie()) {
//		this->clearTracks();
//		this->addAnimation(0, "attack", false);
//		//this->splash->setVisible(true);
//		this->setToSetupPose();
//	}
//	/*slash->getB2Body()->SetTransform(b2Vec2((this->getBoneLocation("bone32").x+this->getParent()->getPosition().x)/PTM_RATIO,
//	(this->getBoneLocation("bone32").y+this->getParent()->getPosition().y)/PTM_RATIO),0);*/
//	slash->setVisible(true);
//	if (slash->getB2Body() != nullptr) {
//		slash->getB2Body()->GetWorld()->DestroyBody(slash->getB2Body());
//	}
//	slash->initCirclePhysic(this->getB2Body()->GetWorld(), this->getBoneLocation("bone22") + this->getParent()->getPosition());
//	slash->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width / 3 / PTM_RATIO, 0));
//}

void EnemyHoacDo2::playsoundAt()
{
	AudioManager::playSound(SOUND_HD2AT);
}

void EnemyHoacDo2::playsoundDie()
{
	AudioManager::playSound(SOUND_HD2DIE);
}


