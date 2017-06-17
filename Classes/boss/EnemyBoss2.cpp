#include "EnemyBoss2.h"
#include "manager\AudioManager.h"

EnemyBoss2::EnemyBoss2(string jsonFile, string atlasFile, float scale) :EnemyBoss1(jsonFile, atlasFile, scale)
{
	health = 30;
}

EnemyBoss2 * EnemyBoss2::create(string jsonFile, string atlasFile, float scale)
{
	EnemyBoss2* boss = new EnemyBoss2(jsonFile, atlasFile, scale);
	boss->setAnimation(0, "idle", true);
	boss->update(0.0f);
	boss->normalState = new BossIdling();
	boss->crazyState = nullptr;
	boss->setTag(TAG_BOSS);
	boss->scaleBoss = scale;
	boss->setLevelBoss(2);
	boss->autorelease();
	return boss;
}

void EnemyBoss2::createPool()
{
	slashPool = CCArray::createWithCapacity(6);
	slashPool->retain();
	for (int i = 0; i < 6; i++) {
		auto scale = SCREEN_SIZE.height / 20 / 96;
		auto slash = SlashBoss::create("Animation/Enemy_Boss2/skill-boss-2.json", "Animation/Enemy_Boss2/skill-boss-2.atlas", scale);
		slash->setVisible(false);
		slash->setPosition(this->getPosition());
		//slash->setScale(SCREEN_SIZE.height / 8 / slash->getContentSize().height);
		this->getParent()->addChild(slash, ZORDER_ENEMY);
		auto tmpbody = slash->getB2Body();
		tmpbody = nullptr;
		slashPool->addObject(slash);
	}

	indexSlash = 0;
}

Vec2 EnemyBoss2::getPosGenSlash()
{
	return this->getBoneLocation("bone5");
}

void EnemyBoss2::creatSlash(float angel)
{
	auto slash = (SlashBoss*)slashPool->getObjectAtIndex(indexSlash);
	slash->setVisible(true);
	indexSlash++;
	if (indexSlash >= slashPool->count()) {
		indexSlash = 0;
	}
	if (slash->getB2Body() != nullptr) {
		auto world = slash->getB2Body()->GetWorld();
		world->DestroyBody(slash->getB2Body());
	}
	auto world = this->getB2Body()->GetWorld();
	slash->initCirclePhysic(world, this->getBoneLocation("bone5"));
	slash->changeBodyCategoryBits(BITMASK_SLASH);
	slash->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD);
	//slash->setRotation(180 - 180 / 4);
	slash->setAngle(angel);
	slash->setRotation(-angel / PI * 180 + 180);
	slash->getB2Body()->SetLinearVelocity(slash->getB2Body()->GetLinearVelocity() + this->getB2Body()->GetLinearVelocity());
}

void EnemyBoss2::playSoundAttack1()
{
	AudioManager::playSound(SOUND_BOSS2CHEM);
}

void EnemyBoss2::playSoundAttack2()
{
	AudioManager::playSound(SOUND_BOSS2SKILL);
}


void EnemyBoss2::playSoundDie()
{
	AudioManager::playSound(SOUND_BOSS2DIE);
}
