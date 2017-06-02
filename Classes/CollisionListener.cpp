#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"
#include "duongqua/DQ_DocCoKiemPhap.h"
#include "duongqua/DQ_TieuHonChuong.h"
#include "layer/GameScene.h"
#include "coin/Coin.h"
#include "Slash.h"
#include "manager/RefManager.h"
#include "boss/EnemyBoss1.h"
#include "item/Item.h"
#include "EnemyDatNhiBa.h"


CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	uint16 bitmaskA = bodyA->GetFixtureList()->GetFilterData().categoryBits;
	uint16 bitmaskB = bodyB->GetFixtureList()->GetFilterData().categoryBits;

	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];
	//worldManifold.

	if ((bitmaskA == BITMASK_BIRD && bitmaskB == BITMASK_FLOOR) ||
		(bitmaskB == BITMASK_BIRD && bitmaskA == BITMASK_FLOOR)
		) {
		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		ChimDieu *bird;

		bird = sA ? (ChimDieu*)sA : (ChimDieu*)sB;
		bird->setIsAbleToDropHero(true);
	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_FLOOR) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_FLOOR)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		auto hero = sA ? (BaseHero *)sA : (BaseHero *)sB;

		// neu sa la floor
		if (!sA) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (bodyB->GetPosition().y < collidePoint.y || dentaX > radius / 2) {

			}
			else {
				hero->setOnGround(true);
				hero->setNumberOfJump(2);

				hero->getSmokeLand()->setVisible(true);
				hero->smokeLandingAni();
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (bodyA->GetPosition().y < collidePoint.y || dentaX > radius / 2) {

			}
			else {

				hero->setOnGround(true);
				hero->setNumberOfJump(2);

				hero->getSmokeLand()->setVisible(true);
				hero->smokeLandingAni();
			}
		}

	}


	// thuan dv toi uu
	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_ENEMY) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_ENEMY)
		) {
		auto enemy = bitmaskA == BITMASK_HERO ? (BaseEnemy*)bodyB->GetUserData() : (BaseEnemy*)bodyA->GetUserData();
		auto hero = bitmaskA == BITMASK_HERO ? (BaseHero*)bodyA->GetUserData() : (BaseHero*)bodyB->GetUserData();
		auto bodyEnemy = bitmaskA == BITMASK_HERO ? bodyB : bodyA;
		auto bodyHero = bitmaskA == BITMASK_HERO ? bodyA : bodyB;
		if (enemy->getTag() > 100 && enemy->getTag() < 120) {
			// do nothing
		}
		else {

			if (bodyHero->GetPosition().x < (bodyEnemy->GetPosition().x + bodyEnemy->GetFixtureList()->GetShape()->m_radius / 3)) {
				enemy->attack();
				if (!enemy->getIsDie() && !hero->getIsNoDie()) {
					if (!hero->getIsPriorInjured()
						&& hero->getFSM()->previousState != MInjured
						&& hero->getFSM()->previousState != MDie) {
						hero->setIsPriorInjured(true);
						hero->getFSM()->changeState(MInjured);
						hero->getBloodScreen()->setVisible(true);
						auto hp = hero->getHealth() - enemy->getDamage();
						hero->setHealth(hp <= 0 ? 0 : hp);
						//log("----");
						auto parentGameScene = (GameScene*)hero->getParent();

						parentGameScene->updateBloodBar(hero->getHealth(), false);

					}
				}
			}
		}

	}
	if ((bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_ENEMY)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_ENEMY)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_ENEMY)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_ENEMY)) {
		auto enemy = bitmaskA == BITMASK_ENEMY ? (BaseEnemy*)bodyA->GetUserData() : (BaseEnemy*)bodyB->GetUserData();
		auto parentGameScene = (GameScene*)enemy->getParent()->getParent();

		if (enemy->getTag() == TAG_ENEMY_CHONG1 || enemy->getTag() == TAG_ENEMY_CHONG2 || enemy->getTag() == TAG_ENEMY_CHONG3)
			return;
		if (bitmaskA == BITMASK_RADA_SKILL_1 || bitmaskB == BITMASK_RADA_SKILL_1) {
			if (enemy->getTag() == TAG_ENEMY_CHONG1 || enemy->getTag() == TAG_ENEMY_CHONG2 || enemy->getTag() == TAG_ENEMY_CHONG3)
				return;
			parentGameScene->getHero()->selectEnemyBySkill1(enemy);
		}
		if (bitmaskA == BITMASK_RADA_SKILL_2 || bitmaskB == BITMASK_RADA_SKILL_2) {
			parentGameScene->getHero()->selectEnemyBySkill2(enemy);
		}
	}

	if ((bitmaskA == BITMASK_ENEMY && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_ENEMY && bitmaskA == BITMASK_SWORD)
		) {
		auto bodySword = bitmaskA == BITMASK_SWORD ? bodyA : bodyB;
		auto enemy = bitmaskA == BITMASK_ENEMY ? (BaseEnemy*)bodyA->GetUserData() : (BaseEnemy*)bodyB->GetUserData();

		if (bodySword->GetUserData()) {
			enemy->setIsDie(true);
			auto thc = (B2Sprite*)(bodySword->GetUserData());
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				((DQ_TieuHonChuong*)thc)->setIsCollide(true);
		}
		else {
			enemy->hit();
		}

	}

	if ((bitmaskA == BITMASK_WOODER && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_WOODER && bitmaskA == BITMASK_SWORD)
		) {
		contact->SetEnabled(false);

		auto bodySword = bitmaskA == BITMASK_SWORD ? bodyA : bodyB;
		auto enemy = bitmaskA == BITMASK_WOODER ? (BaseEnemy*)bodyA->GetUserData() : (BaseEnemy*)bodyB->GetUserData();
		if (enemy->getTag() == TAG_ENEMY_WOODER &&  bodySword->GetUserData() && 
			((B2Sprite*) bodySword->GetUserData())->getTag() == TAG_DQ_DOC_CO_KIEM_PHAP) {
			return;
		}
		enemy->hit();
		if (bodySword->GetUserData()) {

			auto thc = (B2Sprite*)(bodySword->GetUserData());
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				((DQ_TieuHonChuong*)thc)->setIsCollide(true);
		}

	}
	// end thuan dv toi uu.


	// giu
	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_BOSS) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_BOSS)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto enemy = sA->getTag() == TAG_BOSS ? (EnemyBoss1 *)sA : (EnemyBoss1 *)sB;

		if (enemy->getControlState() < 0)
			enemy->changeState(new BossAttacking1());

	}


	// giu
	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN_BULLION) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN_BULLION)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto coin = sA->getTag() == TAG_HERO ? (CoinBullion *)sB : (CoinBullion *)sA;
		coin->picked();
		hero->setCoinExplored(hero->getCoinExplored() + 5 * hero->getCoinRatio());
	}
	//giu
	if ((bitmaskA == BITMASK_COIN_BAG && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_COIN_BAG && bitmaskA == BITMASK_SWORD)
		) {

		auto coinBody = bitmaskA == BITMASK_COIN_BAG ? bodyA : bodyB;
		CoinBag* coin = (CoinBag*)coinBody->GetUserData();

		coin->die();

		auto parentGameScene = (GameScene*)coin->getParent();
		auto hero = parentGameScene->getHero();
		hero->setCoinExplored(hero->getCoinExplored() + 10 * hero->getCoinRatio());
	}

	if ((bitmaskA == BITMASK_BOSS && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_BOSS && bitmaskA == BITMASK_SWORD)
		) {

		auto bodyBoss = bitmaskA == BITMASK_BOSS ? bodyA : bodyB;
		auto bodySword = bitmaskA == BITMASK_BOSS ? bodyB : bodyA;

		BaseEnemy *enemy = (BaseEnemy *)bodyBoss->GetUserData();
		enemy->die();

		if (bodySword->GetUserData()) {		// sA and sB != nullptr

			auto thc = (TieuHonChuong*)bodySword->GetUserData();
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}


	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_SLASH) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_SLASH)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		if (!hero->getIsPriorInjured() && !hero->getIsNoDie()
			&& hero->getFSM()->previousState != MInjured
			&& hero->getFSM()->previousState != MDie) {
			hero->setIsPriorInjured(true);
			hero->getFSM()->changeState(MInjured);
			hero->getBloodScreen()->setVisible(true);
			hero->setHealth(hero->getHealth() - 1);

			auto parentGameScene = (GameScene*)hero->getParent();
			parentGameScene->updateBloodBar(hero->getHealth(), false);
		}

	}

	if ((bitmaskA == BITMASK_SWORD && bitmaskB == BITMASK_SLASH) ||
		(bitmaskB == BITMASK_SWORD && bitmaskA == BITMASK_SLASH)
		) {

		Slash* sA = (Slash*)bodyA->GetUserData();
		Slash* sB = (Slash*)bodyB->GetUserData();
		Slash *slash;

		if (sA && sB)		// sA and sB != nullptr
			slash = sA->getTag() == TAG_SLASH ? (Slash *)sA : (Slash *)sB;
		else
			slash = sA ? (Slash *)sA : (Slash *)sB;

		GameScene * layer;
		Vec2 posLayer = Vec2::ZERO;
		auto parentGameLayer = slash->getParent();
		if (parentGameLayer->getName() == "gameLayer") {
			layer = (GameScene *)parentGameLayer;
		}
		else {
			layer = (GameScene *)parentGameLayer->getParent();
			posLayer = parentGameLayer->getPosition();
		}

		slash->setIsDie(true);
		auto hero = layer->getHero();
		hero->getSlashBreak()->setPosition(slash->getPosition() + posLayer);
		hero->getSlashBreak()->setVisible(true);
		hero->slashBreakAni();
	}


	if ((bitmaskA == BITMASK_SWORD && bitmaskB == BITMASK_UNDER_GROUND) ||
		(bitmaskB == BITMASK_SWORD && bitmaskA == BITMASK_UNDER_GROUND)
		) {

		B2Sprite* sA = (B2Sprite*)bodyA->GetUserData();
		B2Sprite* sB = (B2Sprite*)bodyB->GetUserData();
		auto not_ground = sA ? (B2Sprite *)sA : (B2Sprite *)sB;

		auto parentGameScene = (GameScene*)not_ground->getParent();
		parentGameScene->shakeTheScreen();

		if (not_ground->getTag() == TAG_DQ_DOC_CO_KIEM_PHAP) {
			auto kp = (KiemPhap*)not_ground;
			kp->hitGround();
			kp->setIsCollide(true);
			kp->setTextureRect(Rect(Vec2::ZERO,
				Size(kp->getContentSize().width, kp->getContentSize().height * random(0.61f, 0.63f))));
		}

	}
}

void CollisionListener::EndContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	uint16 bitmaskA = bodyA->GetFixtureList()->GetFilterData().categoryBits;
	uint16 bitmaskB = bodyB->GetFixtureList()->GetFilterData().categoryBits;

	if ((bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_ENEMY)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_ENEMY)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_ENEMY)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_ENEMY)) {

		auto enemy = bitmaskA == BITMASK_ENEMY ? (BaseEnemy*)bodyA->GetUserData() : (BaseEnemy*)bodyB->GetUserData();

		auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
		if (bitmaskA == BITMASK_RADA_SKILL_1 || bitmaskB == BITMASK_RADA_SKILL_1) {
			if (enemy->getTag() == TAG_ENEMY_CHONG1 || enemy->getTag() == TAG_ENEMY_CHONG2 || enemy->getTag() == TAG_ENEMY_CHONG3)
				return;
			parentGameScene->getHero()->deSelectEnemyBySkill1();
		}
		if (bitmaskA == BITMASK_RADA_SKILL_2 || bitmaskB == BITMASK_RADA_SKILL_2) {
			parentGameScene->getHero()->deSelectEnemyBySkill2();
		}

	}
}


void CollisionListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];


	// one way collision
	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();

		auto hero = sA ? (BaseHero *)sA : (BaseHero *)sB;

		if (!sA) {
			auto dentaX = fabs(collidePoint.x - bodyB->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (fabs(bodyB->GetPosition().y - radius * 0.99f) < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}
		else {
			auto dentaX = fabs(collidePoint.x - bodyA->GetPosition().x);
			auto radius = (hero->getTrueRadiusOfHero() / PTM_RATIO);
			if (fabs(bodyA->GetPosition().y - radius * 0.99f) < collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}
	}
}