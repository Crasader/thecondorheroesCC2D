#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"

CollisionListener::CollisionListener() {

}

CollisionListener::~CollisionListener()
{

}

void CollisionListener::BeginContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	//// dùng để tính toán các vị trí contact
	b2WorldManifold	worldManifold;
	contact->GetWorldManifold(&worldManifold);
	auto collidePoint = worldManifold.points[0];
	//worldManifold.

	log("Category bitmask A %d", bodyA->GetFixtureList()->GetFilterData().categoryBits);
	log("Mask bitmask A: %d", bodyA->GetFixtureList()->GetFilterData().maskBits);
	log("Category bitmask B: %d", bodyB->GetFixtureList()->GetFilterData().categoryBits);
	log("Mask bitmask B: %d", bodyB->GetFixtureList()->GetFilterData().maskBits);
	log("------------------------------------------------------------");

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_FLOOR)
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

	if ((bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER) ||
		(bodyB->GetFixtureList()->GetFilterData().categoryBits == BITMASK_HERO && bodyA->GetFixtureList()->GetFilterData().categoryBits == BITMASK_WOODER)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto wooder = sA->getTag() == TAG_ENEMY_WOODER ? (BaseEnemy *)sA : (BaseEnemy *)sB;
		wooder->die();
		
	}

	//// neu hero va cham coin
	//else if ((sA->getTag() == TAG_HERO && (sB->getTag() > 100)) ||
	//	(sB->getTag() == TAG_HERO && (sA->getTag() > 100))) {
	//	log("Enemy");
	//}


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
			if (fabs(bodyA->GetPosition().y - radius * 0.99f)< collidePoint.y || dentaX > radius / 2) {
				contact->SetEnabled(false);
			}
		}

	}
}

