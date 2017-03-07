#include "EnemyToanChanStudent.h"

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
	enemy->setScaleEnemy(scale);
	return enemy;

}

void EnemyToanChanStudent::run()
{
}

void EnemyToanChanStudent::attack()
{
	this->setTimeScale(0.05f);
	this->clearTracks();
	this->addAnimation(0, "attack", false);
//	this->addAnimation(0, "idle", true);
	this->splash->setVisible(true);
	this->setToSetupPose();
}

void EnemyToanChanStudent::die()
{
//	auto world = this->body->GetWorld();
	//world->DestroyBody(this->body);
	//body->SetType(b2_dynamicBody);
	this->setIsDie(true);
	//this->setTimeScale(0.05f);
	this->clearTracks();
	this->addAnimation(0,"die",false);
	this->setToSetupPose();
}

void EnemyToanChanStudent::update(float dt)
{
	BaseEnemy::update(dt);
	
}

void EnemyToanChanStudent::genSplash()
{
	splash = Sprite::create("Animation/Enemy_DeTuToanChan1/DeTuSo1ToanChan/slashenemy.png");
	splash->setScale(scaleEnemy);
	splash->setAnchorPoint(Point(1, 0));
	splash->setPosition(0, 0);
	splash->setVisible(false);
	this->addChild(splash);
}

void EnemyToanChanStudent::listener()
{
	this->setCompleteListener([&](int trackIndex, int loopCount) {
		if (strcmp(getCurrent()->animation->name, "attack") == 0 && loopCount == 1) {
			getSplash()->setVisible(false);
			//setIsAttacking(false);
		}

	});
}
