#include "CollisionListener.h"
#include "BaseHero.h"
#include "BaseEnemy.h"
#include "duongqua/DQ_DocCoKiemPhap.h"
#include "duongqua/DQ_TieuHonChuong.h"
#include "layer/GameScene.h"
#include "coin/Coin.h"
#include "Slash.h"
#include "manager/RefManager.h"
#include "boss1/EnemyBoss1.h"
#include "item/Item.h"


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

	if ((bitmaskA == BITMASK_BIRD && bitmaskB == BITMASK_FLOOR) ||
		(bitmaskB == BITMASK_BIRD && bitmaskA == BITMASK_FLOOR)
		) {
		B2Skeleton* sA = (BaseHero*)bodyA->GetUserData();
		B2Skeleton* sB = (BaseHero*)bodyB->GetUserData();
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

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TOANCHAN1) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TOANCHAN1)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto enemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 || sA->getTag() == TAG_ENEMY_HONGLANGBA1 ? (BaseEnemy *)sA : (BaseEnemy *)sB;

		enemy->attack();
		if (!enemy->getIsDie()) {
			if (!hero->getIsPriorInjured() 
				&& hero->getFSM()->previousState != MInjured
				&& hero->getFSM()->previousState != MDie) {
				hero->setIsPriorInjured(true);
				hero->getFSM()->changeState(MInjured);
				hero->getBloodScreen()->setVisible(true);
				hero->setHealth(hero->getHealth() - 1);
				//log("----");
				auto parentGameScene = (GameScene*)hero->getParent();
				parentGameScene->updateBloodBar(hero->getHealth(), false);
			}
		}

	}

	if ((bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_TOANCHAN1)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_TOANCHAN1)
		|| (bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_TOANCHAN2)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_TOANCHAN2)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_TOANCHAN1)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_TOANCHAN1)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_TOANCHAN2)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_TOANCHAN2)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

		auto _aEnemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 || sA->getTag() == TAG_ENEMY_TOANCHAN2
        || sA->getTag() == TAG_ENEMY_HONGLANGBA1 || sA->getTag() == TAG_ENEMY_HONGLANGBA2 ? (BaseEnemy *)sA : (BaseEnemy *)sB;


		auto parentGameScene = (GameScene*)_aEnemy->getParent()->getParent();
		if (bitmaskA == BITMASK_RADA_SKILL_1 || bitmaskB == BITMASK_RADA_SKILL_1) {
			parentGameScene->getHero()->selectEnemyBySkill1(_aEnemy);
		}
		if (bitmaskA == BITMASK_RADA_SKILL_2 || bitmaskB == BITMASK_RADA_SKILL_2) {
			parentGameScene->getHero()->selectEnemyBySkill2(_aEnemy);
		}
	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_BOSS) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_BOSS)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		//auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto enemy = sA->getTag() == TAG_BOSS ? (EnemyBoss1 *)sA : (EnemyBoss1 *)sB;

		if (enemy->getControlState() < 0)
			enemy->changeState(new Boss1Attacking1());

	}


	/*if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto coin = sA->getTag() == TAG_COIN ? (Coin *)sA : (Coin *)sB;
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		if (coin->getB2Body() && coin->getParent()) {
			coin->getB2Body()->GetFixtureList()->SetSensor(true);
			coin->picked();
			hero->setCoinExplored(hero->getCoinExplored() + 1);
		}

	}
*/
	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_ITEM) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_ITEM)
		) {

		auto sA = (B2Skeleton*) bodyA->GetUserData();
		auto sB = (B2Skeleton*) bodyB->GetUserData();
		auto item = sA->getTag() == TAG_ITEM ? (Item *)sA : (Item *)sB;
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;

		auto parentGameScene = (GameScene*)hero->getParent();
		if(hero->getHealth() <= 0) return;		// you cannot eat item while dying

		if (item->getTypeItem() == Item_type::HEALTH && hero->getHealth() < REF->getCurrentHealth()) {
			hero->setHealth(hero->getHealth() + 1);
			parentGameScene->updateBloodBar(hero->getHealth() - 1, true);		
		}

		if (item->getTypeItem() == Item_type::MAGNET) {
			parentGameScene->runnerItem(DURATION_MAGNET);
		}

		item->setTaken(true);
		item->picked();
		
	}

	if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_COIN_BULLION) ||
		(bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_COIN_BULLION)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
		auto coin = sA->getTag() == TAG_HERO ? (CoinBullion *)sB : (CoinBullion *)sA;
		coin->picked();
		hero->setCoinExplored(hero->getCoinExplored() + 5);
	}

	if ((bitmaskA == BITMASK_COIN_BAG && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_COIN_BAG && bitmaskA == BITMASK_SWORD)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
		CoinBag* coin;
		if (sA && sB) {
			coin = sA->getTag() == TAG_COINBAG ? (CoinBag *)sA : (CoinBag *)sB;
		}
		else {
			coin = sA ? (CoinBag*)sA : (CoinBag*)sB;
		}

		coin->die();

		auto parentGameScene = (GameScene*)coin->getParent();
		auto hero = parentGameScene->getHero();
		hero->setCoinExplored(hero->getCoinExplored() + 10);
	}

	if ((bitmaskA == BITMASK_WOODER && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_WOODER && bitmaskA == BITMASK_SWORD)
		) {

		B2Sprite* sA = (B2Sprite*)bodyA->GetUserData();
		B2Sprite* sB = (B2Sprite*)bodyB->GetUserData();
		EnemyWooder* enemy;
		if (sA && sB) {
			enemy = sA->getTag() == TAG_ENEMY_WOODER ? (EnemyWooder *)sA : (EnemyWooder *)sB;
		}
		else {
			enemy = sA ? (EnemyWooder*)sA : (EnemyWooder*)sB;
		}
		
		auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
		parentGameScene->setLastScore(enemy->getExp());

		auto hero = parentGameScene->getHero();
		hero->setScore(hero->getScore() + enemy->getExp());
		enemy->setIsDie(true);
	}

	if ((bitmaskA == BITMASK_TOANCHAN1 && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_TOANCHAN1 && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();

		EnemyToanChanStudent *enemy;

		if (sA && sB) {	// sA and sB != nullptr

			enemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 || sA->getTag() == TAG_ENEMY_HONGLANGBA1 ? (EnemyToanChanStudent *)sA : (EnemyToanChanStudent *)sB;
			auto thc = sA->getTag() == TAG_DQ_TIEU_HON_CHUONG ? (TieuHonChuong*)sA : (TieuHonChuong*)sB;
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}
		else
			enemy = sA ? (EnemyToanChanStudent *)sA : (EnemyToanChanStudent *)sB;

		auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
		parentGameScene->setLastScore(enemy->getExp());

		auto hero = parentGameScene->getHero();
		hero->setScore(hero->getScore() + enemy->getExp());
		enemy->setIsDie(true);

	}

	if ((bitmaskA == BITMASK_TOANCHAN2 && bitmaskB == BITMASK_SWORD) ||
		(bitmaskB == BITMASK_TOANCHAN2 && bitmaskA == BITMASK_SWORD)
		) {

		BaseEnemy* sA = (BaseEnemy*)bodyA->GetUserData();
		BaseEnemy* sB = (BaseEnemy*)bodyB->GetUserData();

		EnemyToanChanStudent2 *enemy;

		if (sA && sB) {		// sA and sB != nullptr
			enemy = sA->getTag() == TAG_ENEMY_TOANCHAN2 || sA->getTag() == TAG_ENEMY_HONGLANGBA2 ? (EnemyToanChanStudent2 *)sA : (EnemyToanChanStudent2 *)sB;

			auto thc = sA->getTag() == TAG_DQ_TIEU_HON_CHUONG ? (TieuHonChuong*)sA : (TieuHonChuong*)sB;
			if (thc->getTag() == TAG_DQ_TIEU_HON_CHUONG)
				thc->setIsCollide(true);
		}
		else
			enemy = sA ? (EnemyToanChanStudent2 *)sA : (EnemyToanChanStudent2 *)sB;


		auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
		parentGameScene->setLastScore(enemy->getExp());
		auto hero = parentGameScene->getHero();
		hero->setScore(hero->getScore() + enemy->getExp());
		enemy->setIsDie(true);
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
		if (!hero->getIsPriorInjured() 
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
			layer = (GameScene *) parentGameLayer;
		}
		else {
			layer = (GameScene *) parentGameLayer->getParent();
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

		KiemPhap* sA = (KiemPhap*)bodyA->GetUserData();
		KiemPhap* sB = (KiemPhap*)bodyB->GetUserData();
		auto kp = sA ? (KiemPhap *)sA : (KiemPhap *)sB;

		auto parentGameScene = (GameScene*)kp->getParent();
		parentGameScene->shakeTheScreen();

		kp->hitGround();
		kp->setIsCollide(true);
		kp->setTextureRect(Rect(Vec2::ZERO,
			Size(kp->getContentSize().width, kp->getContentSize().height * random(0.61f, 0.63f))));

	}
    
    if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TOONG)
        || (bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TOONG)
        || (bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TNB)
        || (bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TNB)
        ) {
        
        B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
        B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
        auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
        auto enemy = sA->getTag() == TAG_ENEMY_TOONG || sA->getTag() == TAG_ENEMY_TNB ? (BaseEnemy *)sA : (BaseEnemy *)sB;
        
        if (!enemy->getIsDie()) {
            if (!hero->getIsPriorInjured()
                && hero->getFSM()->previousState != MInjured
                && hero->getFSM()->previousState != MDie) {
                hero->setIsPriorInjured(true);
                hero->getFSM()->changeState(MInjured);
                hero->getBloodScreen()->setVisible(true);
                hero->setHealth(hero->getHealth() - 1);
                //log("----");
                auto parentGameScene = (GameScene*)hero->getParent();
                parentGameScene->updateBloodBar(hero->getHealth(), false);
            }
        }
    }
    
    if ((bitmaskA == BITMASK_TNB && bitmaskB == BITMASK_SWORD) ||
        (bitmaskB == BITMASK_TNB && bitmaskA == BITMASK_SWORD)
        ) {
        
        B2Sprite* sA = (B2Sprite*)bodyA->GetUserData();
        B2Sprite* sB = (B2Sprite*)bodyB->GetUserData();
        EnemyTNB* enemy;
        if (sA && sB) {
            enemy = sA->getTag() == TAG_ENEMY_TNB ? (EnemyTNB *)sA : (EnemyTNB *)sB;
        }
        else {
            enemy = sA ? (EnemyTNB*)sA : (EnemyTNB *)sB;
        }
        
        auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
        parentGameScene->setLastScore(enemy->getExp());
        
        auto hero = parentGameScene->getHero();
        hero->setScore(hero->getScore() + enemy->getExp());
        enemy->setIsDie(true);
    }
    
    if ((bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TOONG)
        || (bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TOONG)
        || (bitmaskA == BITMASK_HERO && bitmaskB == BITMASK_TNB)
        || (bitmaskB == BITMASK_HERO && bitmaskA == BITMASK_TNB)
        ) {
        
        B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
        B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();
        auto hero = sA->getTag() == TAG_HERO ? (BaseHero *)sA : (BaseHero *)sB;
        auto enemy = sA->getTag() == TAG_ENEMY_TOONG || sA->getTag() == TAG_ENEMY_TNB ? (BaseEnemy *)sA : (BaseEnemy *)sB;
        
        if (!enemy->getIsDie()) {
            if (!hero->getIsPriorInjured()
                && hero->getFSM()->previousState != MInjured
                && hero->getFSM()->previousState != MDie) {
                hero->setIsPriorInjured(true);
                hero->getFSM()->changeState(MInjured);
                hero->getBloodScreen()->setVisible(true);
                hero->setHealth(hero->getHealth() - 1);
                //log("----");
                auto parentGameScene = (GameScene*)hero->getParent();
                parentGameScene->updateBloodBar(hero->getHealth(), false);
            }
        }
    }
    
    if ((bitmaskA == BITMASK_TOONG && bitmaskB == BITMASK_SWORD) ||
        (bitmaskB == BITMASK_TOONG && bitmaskA == BITMASK_SWORD)
        ) {
        
        B2Sprite* sA = (B2Sprite*)bodyA->GetUserData();
        B2Sprite* sB = (B2Sprite*)bodyB->GetUserData();
        EnemyToOng* enemy;
        if (sA && sB) {
            enemy = sA->getTag() == TAG_ENEMY_TOONG ? (EnemyToOng *)sA : (EnemyToOng *)sB;
        }
        else {
            enemy = sA ? (EnemyToOng*)sA : (EnemyToOng*)sB;
        }
        
        auto parentGameScene = (GameScene*)enemy->getParent()->getParent();
        parentGameScene->setLastScore(enemy->getExp());
        
        auto hero = parentGameScene->getHero();
        hero->setScore(hero->getScore() + enemy->getExp());
        enemy->setIsDie(true);
    }
}

void CollisionListener::EndContact(b2Contact * contact)
{
	b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();

	uint16 bitmaskA = bodyA->GetFixtureList()->GetFilterData().categoryBits;
	uint16 bitmaskB = bodyB->GetFixtureList()->GetFilterData().categoryBits;

	if ((bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_TOANCHAN1)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_TOANCHAN1)
		|| (bitmaskA == BITMASK_RADA_SKILL_1 && bitmaskB == BITMASK_TOANCHAN2)
		|| (bitmaskB == BITMASK_RADA_SKILL_1 && bitmaskA == BITMASK_TOANCHAN2)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_TOANCHAN1)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_TOANCHAN1)
		|| (bitmaskA == BITMASK_RADA_SKILL_2 && bitmaskB == BITMASK_TOANCHAN2)
		|| (bitmaskB == BITMASK_RADA_SKILL_2 && bitmaskA == BITMASK_TOANCHAN2)
		) {

		B2Skeleton* sA = (B2Skeleton*)bodyA->GetUserData();
		B2Skeleton* sB = (B2Skeleton*)bodyB->GetUserData();

		auto _aEnemy = sA->getTag() == TAG_ENEMY_TOANCHAN1 || sA->getTag() == TAG_ENEMY_TOANCHAN2
        || sA->getTag() == TAG_ENEMY_HONGLANGBA1 || sA->getTag() == TAG_ENEMY_HONGLANGBA2 ? (BaseEnemy *)sA : (BaseEnemy *)sB;


		auto parentGameScene = (GameScene*)_aEnemy->getParent()->getParent();
		if (bitmaskA == BITMASK_RADA_SKILL_1 || bitmaskB == BITMASK_RADA_SKILL_1) {
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

