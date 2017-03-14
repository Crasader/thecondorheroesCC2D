#include "EnemyToanChanStudent2.h"

EnemyToanChanStudent2::EnemyToanChanStudent2(string jsonFile, string atlasFile, float scale):EnemyToanChanStudent(jsonFile, atlasFile,scale)
{
	controlAttack = 0;
}

EnemyToanChanStudent2 * EnemyToanChanStudent2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyToanChanStudent2 *enemy = new EnemyToanChanStudent2(jsonFile, atlasFile,scale);
	enemy->update(0.0f);
	enemy->setTag(TAG_ENEMY_TOANCHAN2);
	enemy->setScaleX(1);
	enemy->setAnimation(0, "idle", true);
	enemy->setScaleEnemy(scale);
	return enemy;

}

void EnemyToanChanStudent2::attack()
{
	EnemyToanChanStudent::attack();
	slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x/PTM_RATIO, this->getBoneLocation("bone32").y/PTM_RATIO),0);
	slash->setVisible(true);
	slash->getB2Body()->SetLinearVelocity(b2Vec2(-SCREEN_SIZE.width/PTM_RATIO,0));
}

void EnemyToanChanStudent2::die()
{
	EnemyToanChanStudent::die();

}

void EnemyToanChanStudent2::genSlash()
{
	slash = Slash::create("Animation/Enemy_DeTuToanChan2/slashenemy.png");
	slash->setScale(scaleEnemy);
	//slash->setAnchorPoint(Point(1, 0));
	
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
		}

		if (strcmp(getCurrent()->animation->name, "die") == 0 && loopCount == 1) {
			slash->removeFromParentAndCleanup(true);
			this->removeFromParentAndCleanup(true);
		}

	});
}

void EnemyToanChanStudent2::updateMe(float dt)
{
	BaseEnemy::updateMe(dt);
	slash->updateMe(dt);
	if (slash->getPositionX() < this->getPositionX() - SCREEN_SIZE.width) {
		slash->getB2Body()->SetTransform(b2Vec2(this->getBoneLocation("bone32").x / PTM_RATIO, this->getBoneLocation("bone32").y / PTM_RATIO), 0);
		slash->getB2Body()->SetLinearVelocity(b2Vec2(0,0));
		slash->setVisible(false);
	}
	controlAttack++;
	if (controlAttack > 90) {
		controlAttack = 0;// 2 giay 1 nhat
		this->attack();
	}
}

void EnemyToanChanStudent2::removeFromParentAndCleanup(bool cleanup)
{
	BaseEnemy::removeFromParentAndCleanup(cleanup);
	auto world = slash->getB2Body()->GetWorld();
	world->DestroyBody(slash->getB2Body());
	slash->removeFromParentAndCleanup(cleanup);
	log("delete slash");
}

void EnemyToanChanStudent2::onExit()
{
	BaseEnemy::onExit();
	/*auto world = slash->getB2Body()->GetWorld();
	world->DestroyBody(slash->getB2Body());
	log("delete slash");*/
}
