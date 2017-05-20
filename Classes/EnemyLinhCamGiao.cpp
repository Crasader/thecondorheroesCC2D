#include "EnemyLinhCamGiao.h"
#include "manager/SkeletonManager.h"
#include "manager/AudioManager.h"


EnemyLinhCamGiao::EnemyLinhCamGiao(spSkeletonData * data):EnemyToanChanStudent(data)
{
}

EnemyLinhCamGiao::EnemyLinhCamGiao(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
}

EnemyLinhCamGiao * EnemyLinhCamGiao::create(string jsonFile, string atlasFile, float scale)
{
	EnemyLinhCamGiao *enemy = new EnemyLinhCamGiao(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setScaleEnemy(scale);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;

}


EnemyLinhCamGiao * EnemyLinhCamGiao::create(string filename, float scale)
{
	if (!SkeletonManager::getSkeletonData(filename)) {
		SkeletonManager::getInstance()->cacheSkeleton(filename, scale);
	}
	auto data = SkeletonManager::getSkeletonData(filename);
	auto enemy = new EnemyLinhCamGiao(data);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_LINHCAMGIAO1);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	//enemy->setTimeScale(1.4f);
	enemy->health = 1;
	enemy->exp = 12;
	return enemy;
}

void EnemyLinhCamGiao::run()
{

}

void EnemyLinhCamGiao::updateMe(BaseHero * hero)
{
	log("linh cam giao %d", getTag());
	BaseEnemy::updateMe(hero);
	if (getIsDie() && this->getB2Body() != nullptr) {
		die();
	}
}

void EnemyLinhCamGiao::playsoundAt()
{
	AudioManager::playSound(SOUND_TC1AT);
}

void EnemyLinhCamGiao::playsoundDie()
{
	AudioManager::playSound(SOUND_TC1DIE);
}


//void EnemyLinhCamGiao::genSplash()
//{
//	splash = Sprite::create("Animation/Enemy-DeTuToanChan1/slashenemy.png");
//	splash->setScale(SCREEN_SIZE.height/5/splash->getContentSize().height);
//	splash->setAnchorPoint(Point(1, 0));
//	
//	splash->setPosition(-this->getBody()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO, 0);
//	splash->setVisible(false);
//	this->addChild(splash);
//}

