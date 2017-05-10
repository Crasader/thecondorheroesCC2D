#include "BaseHero.h"
#include "manager\AudioManager.h"



BaseHero::BaseHero(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	bloodScreen = Sprite::create("red-screen.png");
	bloodScreen->setScaleX(SCREEN_SIZE.width * 1.1f / bloodScreen->getContentSize().width);
	bloodScreen->setScaleY(SCREEN_SIZE.height / bloodScreen->getContentSize().height);
	bloodScreen->setVisible(false);
	isDriverEagle = false;
	currentRunDis = 0.0f;
	preRunDis = 0.0f;
	isNoDie = false;
	noActive = false;
	isKillAll = false;
	dieHard = 1;
	coinRatio = 1;
	createMapItem();
}

BaseHero * BaseHero::create(string jsonFile, string atlasFile, float scale)
{
	BaseHero* baseHero = new BaseHero(jsonFile, atlasFile, scale);
	return baseHero;
}

void BaseHero::initSwordPhysic(b2World *world, Point position, float width)
{
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	shape.SetAsBox(width / PTM_RATIO, getTrueRadiusOfHero()* 0.85f / PTM_RATIO);

	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &shape;

	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY;

	bodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	bodyDef.type = b2_dynamicBody;

	swordBody = world->CreateBody(&bodyDef);
	swordBody->CreateFixture(&fixtureDef);
}

void BaseHero::changeSwordCategoryBitmask(uint16 bit)
{
	auto fixture = swordBody->GetFixtureList();
	b2Filter filter = fixture->GetFilterData();
	filter.categoryBits = bit;
	fixture->SetFilterData(filter);
}

void BaseHero::addStuff()
{
	auto scale = this->getScale() / 3;
	activeSkill = new SkeletonAnimation("Effect/active_skill_eff.json", "Effect/active_skill_eff.atlas", scale * 0.75f);
	slashBreak = new SkeletonAnimation("Effect/slash-break.json", "Effect/slash-break.atlas", scale / 2);
	smokeJumpX2 = new SkeletonAnimation("Effect/smoke-jumpx2.json", "Effect/smoke-jumpx2.atlas", scale / 2);
	smokeLand = new SkeletonAnimation("Effect/smoke-landing.json", "Effect/smoke-landing.atlas", scale / 2);
	smokeRun = new SkeletonAnimation("Effect/smoke-run.json", "Effect/smoke-run.atlas", scale);
	reviveMe = new SkeletonAnimation("Effect/revive.json", "Effect/revive.atlas", scale / 2);
	suctionCoin = new SkeletonAnimation("Effect/effect_magnet.json", "Effect/effect_magnet.atlas", scale * 1.43f);

	activeSkill->setVisible(false);		activeSkill->update(0.0f);
	slashBreak->setVisible(false);		slashBreak->update(0.0f);
	smokeJumpX2->setVisible(false);		smokeJumpX2->update(0.0f);
	smokeLand->setVisible(false);		smokeLand->update(0.0f);
	smokeRun->setVisible(false);		smokeRun->update(0.0f);
	reviveMe->setVisible(false);		reviveMe->update(0.0f);
	suctionCoin->setVisible(false);		suctionCoin->update(0.0f);


	activeSkill->setPosition(this->getContentSize().width / 2, 0);
	smokeLand->setPosition(this->getContentSize().width / 2, 0);
	smokeRun->setPosition(this->getContentSize().width / 2, 0);
	suctionCoin->setPosition(this->getContentSize().width / 2, 0);


	activeSkillAni();
	smokeRunAni();
	suctionAni();

	addChild(activeSkill);
	addChild(smokeLand);
	addChild(smokeRun);
	addChild(suctionCoin);

	this->getParent()->addChild(slashBreak, ZORDER_SMT);
	this->getParent()->addChild(smokeJumpX2, ZORDER_SMT);
	this->getParent()->addChild(reviveMe, ZORDER_SMT);
}

void BaseHero::activeSkillAni()
{
	activeSkill->clearTracks();
	activeSkill->addAnimation(0, "active", true);
	activeSkill->setToSetupPose();
}

void BaseHero::slashBreakAni()
{
	slashBreak->clearTracks();
	slashBreak->addAnimation(0, "slash-break", false);
	slashBreak->setToSetupPose();
}

void BaseHero::smokeJumpX2Ani()
{
	smokeJumpX2->clearTracks();
	smokeJumpX2->addAnimation(0, "smoke-jumpx2", false);
	smokeJumpX2->setToSetupPose();
}

void BaseHero::smokeLandingAni()
{
	smokeLand->clearTracks();
	smokeLand->addAnimation(0, "smoke-landing", false);
	smokeLand->setToSetupPose();
}

void BaseHero::smokeRunAni()
{
	smokeRun->clearTracks();
	smokeRun->addAnimation(0, "smoke-run", true);
	smokeRun->setToSetupPose();
}

void BaseHero::reviveAni()
{
	reviveMe->clearTracks();
	reviveMe->addAnimation(0, "revive", false);
	reviveMe->setToSetupPose();
}

void BaseHero::suctionAni()
{
	suctionCoin->clearTracks();
	suctionCoin->addAnimation(0, "effect_magnet", true);
	suctionCoin->setToSetupPose();
}

void BaseHero::createPool()
{
}

void BaseHero::idle()
{
}

void BaseHero::run()
{

}

void BaseHero::normalJump()
{
	AudioManager::playSound(SOUND_MCJUMP);
}

void BaseHero::doubleJump()
{
	AudioManager::playSound(SOUND_MCJUMP);
}

void BaseHero::landing()
{
}

void BaseHero::die()
{
}

void BaseHero::attackNormal()
{
	AudioManager::playSound(SOUND_MCAT);
}

void BaseHero::attackLanding()
{
	AudioManager::playSound(SOUND_MCAT);
}

void BaseHero::attackBySkill1()
{
}

void BaseHero::attackBySkill2()
{
}

void BaseHero::attackBySkill3()
{
}

void BaseHero::injured()
{
}

void BaseHero::revive()
{
	AudioManager::playSound(SOUND_MCREVIVE);
}

void BaseHero::die(Point posOfCammera)
{
}

void BaseHero::listener()
{
}

void BaseHero::updateMe(float dt)
{
	if (getB2Body() != nullptr) {

		currentRunDis = this->getPositionX() / (SCREEN_SIZE.width / 23);

		this->setPositionX(this->getB2Body()->GetPosition().x * PTM_RATIO);
		this->setPositionY(this->getB2Body()->GetPosition().y * PTM_RATIO - trueRadiusOfHero);


		getSwordBody()->SetTransform(b2Vec2(getB2Body()->GetPosition().x + getTrueRadiusOfHero() * 2.2f / PTM_RATIO, getB2Body()->GetPosition().y)
			, getSwordBody()->GetAngle());
	}

	if (health <= 1 && !bloodScreen->isVisible()) {
		bloodScreen->setVisible(true);
	}

	if (health <= 0) {
		getFSM()->changeState(MDie);
	}
	updateMapItem();
}

void BaseHero::doCounterSkill1()
{
}

void BaseHero::doCounterSkill2()
{
}

void BaseHero::doCounterSkill3()
{
}

void BaseHero::doDestroyBodies(b2World *world)
{
	world->DestroyBody(getB2Body());
	world->DestroyBody(swordBody);
	setB2Body(nullptr);
	swordBody = nullptr;
}

void BaseHero::updateAttackBossAsTarget(BaseEnemy * p_pBoss)
{
}

void BaseHero::selectEnemyBySkill1(BaseEnemy * p_pEnemySelected)
{
	m_lEnemiesSelectedBySkill1.push_back(p_pEnemySelected);
}

void BaseHero::deSelectEnemyBySkill1()
{
	if (!m_lEnemiesSelectedBySkill1.empty()) {
		m_lEnemiesSelectedBySkill1.pop_front();
	}
}

void BaseHero::selectEnemyBySkill2(BaseEnemy * p_pEnemySelected)
{
	m_lEnemiesSelectedBySkill2.push_back(p_pEnemySelected);
}

void BaseHero::deSelectEnemyBySkill2()
{
	if (!m_lEnemiesSelectedBySkill2.empty()) {
		m_lEnemiesSelectedBySkill2.pop_front();
	}
}

void BaseHero::selectEnemyBySkill3(BaseEnemy * p_pEnemySelected)
{
	m_lEnemiesSelectedBySkill3.push_back(p_pEnemySelected);
}

void BaseHero::deSelectEnemyBySkill3()
{
	if (!m_lEnemiesSelectedBySkill3.empty()) {
		m_lEnemiesSelectedBySkill3.pop_front();
	}
}

void BaseHero::killThemAll()
{
	auto boss = (BaseEnemy*) this->getParent()->getChildByTag(TAG_BOSS);
	if (boss != nullptr && boss->getPositionX() < this->getPositionX() + SCREEN_SIZE.width * 0.75f) {
		boss->die();
		//log("%i", boss->getHealth());
	}

	isKillAll = true;
	blash->setVisible(true);
	//auto originScale = blash->getScale();
	auto scaleFactor = Director::getInstance()->getContentScaleFactor();
	auto scale = ScaleBy::create(0.5f, 150 * scaleFactor);

	auto hide = CallFunc::create([&]() {
		blash->setVisible(false);
		isKillAll = false;
	});

	blash->runAction(Sequence::create(scale, hide, scale->reverse(), nullptr));
}

void BaseHero::createMapItem()
{
	checkItem[KEY_ITEM_MAGNET] = 0;
	checkItem[KEY_ITEM_DOUPLE_COIN] = 0;
}

void BaseHero::updateMapItem()
{
	for (int i = KEY_ITEM_MAGNET; i <= KEY_ITEM_DOUPLE_COIN; i++) {
		int value = checkItem[i];
		if (value > 0) {
			value--;
			checkItem[i] = value;
		}
		
		if (suctionCoin->isVisible() && checkItem[KEY_ITEM_MAGNET] <= 0) {
			suctionCoin->setVisible(false);
		}
		
		if (coinRatio != 1 && checkItem[KEY_ITEM_DOUPLE_COIN] <= 0) {
			coinRatio = 1;
		}
	}
}


int BaseHero::getItemValue(int keyItem)
{
	return checkItem[keyItem];
}

void BaseHero::setItemValue(int keyItem, int value)
{
	checkItem[keyItem]= value;
}


StateMachine * BaseHero::getFSM()
{
	return stateMachine;
}
