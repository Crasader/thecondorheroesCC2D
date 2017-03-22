#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"
#include "DQ_DocCoKiemPhap.h"
#include "DQ_TieuHonChuong.h"
#include "GameScene.h"
#include "Coin.h"
#include "Slash.h"
#include "EffectManager.h"
#include "boss1/EnemyBoss1.h"
#include "boss1\StateBoss1.h"

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

	//// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];
	//worldManifold.

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

				EM->getSmokeLanding()->setPosition(hero->getPosition());
				EM->getSmokeLanding()->setVisible(true);
				EM->smokeLandingAni();
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

				EM->getSmokeLanding()->setPosition(hero->getPosition());
				EM->getSmokeLanding()->setVisible(true);
				EM->smokeLandingAni();
			}
		}

	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TOANCHAN1) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TOANCHAN1)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto enemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->attack();
		if (!enemy->getIsDie()) {
			hero->setIsPrior(true);
			hero->getFSM()->changeState(MInjured);
			hero->getBloodScreen()->setVisible(true);
			hero->setHealth(hero->getHealth() - 1);
		}

	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_BOSS) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_BOSS)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto enemy = sA->getTag() == TAG_BOSS ? (EnemyBoss1 *)sA : (EnemyBoss1 *)sB;

		if (!enemy->lockState) {
			enemy->changeState(new Boss1Attacking1);
			enemy->lockState = true;
		}
		/*if (!enemy->getIsDie()) {
			hero->setIsPrior(true);
			hero->getFSM()->changeState(MInjured);
			hero->setHealth(hero->getHealth() - 1);
		}*/

	}


	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto coin = sA->getTag() == TAG_COIN ? (Coin *)sA : (Coin *)sB;
		coin->picked();

	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN_BULLION) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN_BULLION)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto coin = sA->getTag() == TAG_COINBULLION ? (CoinBullion *)sA : (CoinBullion *)sB;
		coin->picked();

	}

	if ((bitmaskA == BITMASK_COIN_BAG && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_COIN_BAG && bitmaskA == BITMASK_SWORD)
		) {

		B2Skeleton* sA = (BaseEnemy*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseEnemy*)bodyB->GetUserData();
		auto coin = sA ? (B2Skeleton *)sA : (B2Skeleton *)sB;

		coin->die();
	}

	if ((bitmaskA == BITMASK_WOODER && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_WOODER && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();
		auto enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_TOANCHAN1 && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_TOANCHAN1 && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();

		BaseEnemy *enemy;

		if (sA && sB) {	// sA and sB != nullptr
			enemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 ? (BaseEnemy *)sA : (BaseEnemy *)sB;
			auto thc = sA->getTag() == TAG_DQ_TIEU_HON_CHUONG ? (TieuHonChuong*)sA : (TieuHonChuong*)sB;
			if(thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}
		else
			enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_TOANCHAN2 && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_TOANCHAN2 && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();

		BaseEnemy *enemy;

		if (sA && sB) {		// sA and sB != nullptr
			enemy = sA->getTag() == TAG_ENEMY_TOANCHAN2 ? (BaseEnemy *)sA : (BaseEnemy *)sB;
			auto thc = sA->getTag() == TAG_DQ_TIEU_HON_CHUONG ? (TieuHonChuong*)sA : (TieuHonChuong*)sB;
			if(thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}
		else
			enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_BOSS && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_BOSS && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();

		BaseEnemy *enemy;

		if (sA && sB) {		// sA and sB != nullptr
			enemy = sA->getTag() == TAG_BOSS ? (BaseEnemy *)sA : (BaseEnemy *)sB;
			auto thc = sA->getTag() == TAG_DQ_TIEU_HON_CHUONG ? (TieuHonChuong*)sA : (TieuHonChuong*)sB;
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}
		else
			enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_SLASH) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_SLASH)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;

		hero->setIsPrior(true);
		hero->getBloodScreen()->setVisible(true);
		hero->getFSM()->changeState(MInjured);
		hero->setHealth(hero->getHealth() - 1);

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


		slash->setIsDie(true);

		EM->getSlashBreak()->setPosition(slash->getPosition());
		EM->getSlashBreak()->setVisible(true);
		EM->slashBreakAni();

	}


	if ((bitmaskA == BITMASK_SWORD && bitmaskB == BITMASK_UNDER_GROUND) ||
		(bitmaskB == BITMASK_SWORD && bitmaskA == BITMASK_UNDER_GROUND)
		) {

		KiemPhap* sA = (KiemPhap*) bodyA->GetUserData();
		KiemPhap* sB = (KiemPhap*) bodyB->GetUserData();
		auto kp = sA ? (KiemPhap *)sA : (KiemPhap *)sB;

		auto parentGameScene = (GameScene*) kp->getParent();
		parentGameScene->shakeTheScreen();

		kp->hitGround();
		kp->setIsCollide(true);
		kp->setTextureRect(Rect(Vec2::ZERO, 
			Size(kp->getContentSize().width, kp->getContentSize().height * random(0.49f, 0.51f))));
		
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

