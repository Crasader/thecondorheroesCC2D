#include "BaseEnemy.h"
#include "BaseHero.h"
#include "manager/RefManager.h"
#include "layer/GameScene.h"


BaseEnemy::BaseEnemy() :B2Skeleton()
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
	damage = 1;
	health = 1;
	isPrepare = false;
}

BaseEnemy::~BaseEnemy()
{
}

BaseEnemy::BaseEnemy(spSkeletonData * data) :B2Skeleton(data)
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
	damage = 1;
	health = 1;
	isPrepare = false;
}

BaseEnemy::BaseEnemy(string jsonFile, string atlasFile, float scale) :B2Skeleton(jsonFile, atlasFile, scale)
{
	isDie = false;
	isOccur = false;
	isEndOfScreen = false;
	damage = 1;
	health = 1;
	isPrepare = false;
}

BaseEnemy * BaseEnemy::create(string jsonFile, string atlasFile, float scale)
{
	return nullptr;
}

BaseEnemy * BaseEnemy::create(spSkeletonData * data)
{
	auto skeleton = new BaseEnemy(data);
	//skeleton->initWithData(data);
	skeleton->update(1.0f);
	skeleton->autorelease();
	return skeleton;
}

void BaseEnemy::prepare()
{
	//if (!isPrepare) {
	isPrepare = true;
	//}
}

void BaseEnemy::unprepare()
{
	isPrepare = false;
}

void BaseEnemy::hit()
{
	health--;

	if (health <= 0) {
		this->setIsDie(true);
	}
}

void BaseEnemy::run()
{
}

void BaseEnemy::attack()
{
}

void BaseEnemy::die()
{
	if (!isEndOfScreen) {
		auto gameLayer = (GameScene*) this->getParent()->getParent();
		gameLayer->updateMultiKills();
	}

	auto parentGameScene = (GameScene*)this->getParent()->getParent();
	parentGameScene->setLastScore(this->getExp());

	auto hero = parentGameScene->getHero();
	hero->setScore(hero->getScore() + this->getExp() * hero->getScoreRatio());
	unprepare();
	// when Authur using Skill, hero's score ratio is 2. // Ending return to 1.

	if (!REF->getIsLockedHero()) {
		switch (this->getTag()) {
		case TAG_ENEMY_WOODER:
			REF->setUpNumberQuest(INDEX_QUEST_WOODER, 1);
			break;

		case TAG_ENEMY_TOANCHAN2:
			REF->setUpNumberQuest(INDEX_QUEST_TOANCHAN_2, 1);
			break;

		case TAG_ENEMY_TOONG:
			REF->setUpNumberQuest(INDEX_QUEST_BEE, 1);
			break;

		case TAG_ENEMY_TNB:
			REF->setUpNumberQuest(INDEX_QUEST_ICE, 1);
			break;

		case TAG_ENEMY_HONGLANGBA1:
			REF->setUpNumberQuest(INDEX_QUEST_HLB_1, 1);
			break;

		case TAG_ENEMY_HONGLANGBA2:
			REF->setUpNumberQuest(INDEX_QUEST_HLB_2, 1);
			break;

		case TAG_ENEMY_HOACDO1:
			REF->setUpNumberQuest(INDEX_QUEST_HD_1, 1);
			break;

		case TAG_ENEMY_HOACDO2:
			REF->setUpNumberQuest(INDEX_QUEST_HD_2, 1);
			break;

		case TAG_ENEMY_DATNHIBA1:
			REF->setUpNumberQuest(INDEX_QUEST_DNB, 1);
			break;

		default:
			break;
		}
	}
}

void BaseEnemy::updateMe(BaseHero* hero)
{
	updatePos();
	bool a = this->isPrepare;
	if (this->getPositionX() + this->getParent()->getPositionX() -hero->getPositionX() < SCREEN_SIZE.width / 2 && this->getPositionX() + +this->getParent()->getPositionX() > hero->getPositionX() && !this->isPrepare) {
		this->prepare();
	}

	if ( this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() && this->isPrepare) {
		this->unprepare();
	}

	if (!isEndOfScreen && this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() - SCREEN_SIZE.width * 0.25f) {
		isEndOfScreen = true;
	}

	if (hero->getIsKillAll() && this->getB2Body() != nullptr) {
		if (this->getPositionX() + this->getParent()->getPositionX() < hero->getPositionX() + SCREEN_SIZE.width * 0.77f &&
			this->getPositionX() + this->getParent()->getPositionX() > hero->getPositionX() - SCREEN_SIZE.width * 0.28f) {
			auto gameLayer = (GameScene *)hero->getParent();
			if (gameLayer->getFollow()->getPositionY() + SCREEN_SIZE.height * 0.55f > this->getPositionY() &&
				gameLayer->getFollow()->getPositionY() - SCREEN_SIZE.height * 0.6f < this->getPositionY())

				die();
		}
	}
}

void BaseEnemy::initCirclePhysic(b2World * world, Point pos)
{
	B2Skeleton::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

void BaseEnemy::initBoxPhysic(b2World * world, Point pos)
{
	B2Skeleton::initBoxPhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
}

// khong dung cho boss
void BaseEnemy::makeMask()
{
	this->changeBodyCategoryBits(BITMASK_ENEMY);
	// phan vung co hai
	if (this->getTag() >= 120 && this->getTag() <= 140) {
		this->changeBodyMaskBits(BITMASK_HERO | BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	}
	// phan vung vo hai
	else if (this->getTag() >= 100 && this->getTag() < 120) {
		this->changeBodyMaskBits(BITMASK_SWORD | BITMASK_RADA_SKILL_1 | BITMASK_RADA_SKILL_2);
	}
}

void BaseEnemy::updatePos()
{
	if (body != nullptr /*&& body->GetType() == b2_staticBody*/) {
		this->setPositionX(body->GetPosition().x * PTM_RATIO - this->getParent()->getPositionX());
		this->setPositionY(body->GetPosition().y * PTM_RATIO - this->body->GetFixtureList()->GetShape()->m_radius*PTM_RATIO - this->getParent()->getPositionY());
	}
}

