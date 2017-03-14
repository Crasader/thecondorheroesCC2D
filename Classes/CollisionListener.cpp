#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"
#include "Coin.h"

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
			hero->setHealth(hero->getHealth() - 1);
		}

	}


	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto coin = sA->getTag() == TAG_COIN ? (Coin *)sA : (Coin *)sB;
		coin->picked();

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
		auto enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_TOANCHAN2 && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_TOANCHAN2 && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();
		auto enemy = sA ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->die();
	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_SLASH) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_SLASH)
		) {

		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;

		hero->setIsPrior(true);
		hero->getFSM()->changeState(MInjured);
		hero->setHealth(hero->getHealth() - 1);

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

