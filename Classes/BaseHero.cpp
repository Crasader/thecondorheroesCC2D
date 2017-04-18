#include "BaseHero.h"
#include "AudioEngine.h"
#include "Global.h"


BaseHero::BaseHero(string jsonFile, string atlasFile, float scale) : B2Skeleton(jsonFile, atlasFile, scale)
{
	bloodScreen = Sprite::create("red-screen.png");
	bloodScreen->setScaleX(SCREEN_SIZE.width * 1.1f / bloodScreen->getContentSize().width);
	bloodScreen->setScaleY(SCREEN_SIZE.height / bloodScreen->getContentSize().height);
	bloodScreen->setVisible(false);
	isDriverEagle = false;
	currentRunDis = 0.0f;
	preRunDis = 0.0f;

	dieHard = 1;
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
	fixtureDef.filter.maskBits = BITMASK_WOODER | BITMASK_TOANCHAN1 | BITMASK_TOANCHAN2 | BITMASK_SLASH | BITMASK_BOSS | BITMASK_COIN_BAG;

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
}




void BaseHero::idle()
{
}

void BaseHero::run()
{

}

void BaseHero::normalJump()
{
}

void BaseHero::doubleJump()
{
}

void BaseHero::landing()
{
}

void BaseHero::die()
{
}

void BaseHero::attackNormal()
{
}

void BaseHero::attackLanding()
{
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

	if (blash->isVisible()) {
		blash->setPosition(this->getPosition());
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

void BaseHero::killThemAll(list<BaseEnemy*> listToKill)
{
	blash->setVisible(true);
	//auto originScale = blash->getScale();
	auto scaleFactor = Director::getInstance()->getContentScaleFactor();
	auto scale = ScaleBy::create(0.7f, 100 * scaleFactor);

	auto hide = CallFunc::create([&]() {
		blash->setVisible(false);
	});

	blash->runAction(Sequence::create(scale, hide, scale->reverse(), nullptr));

	for (auto enemy : listToKill) {
		enemy->setIsDie(true);
	}

	auto boss = (BaseEnemy*) this->getParent()->getChildByTag(TAG_BOSS);
	if (boss != nullptr && boss->getPositionX() < this->getPositionX() + SCREEN_SIZE.width * 0.75f) {
		boss->die();
		log("%i", boss->getHealth());
	}
}

void BaseHero::createMapItem()
{
	checkItem[KEY_ITEM_MAGNET]= 0;
	checkItem[KEY_ITEM_DOUPLE_COIN]= 0;
}

void BaseHero::updateMapItem()
{
	for (int i = KEY_ITEM_MAGNET; i < KEY_ITEM_DOUPLE_COIN; i++) {
		int value = checkItem[i];
		if (value > 0) {
			value--;
			checkItem[i]= value;
		};
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
