#include "QT_SongLongXuatThuy.h"


QT_SongLongXuatThuy::QT_SongLongXuatThuy(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{
	head_1 = nullptr;
	head_2 = nullptr;
}

QT_SongLongXuatThuy * QT_SongLongXuatThuy::create(string jsonFile, string atlasFile, float scale)
{
	QT_SongLongXuatThuy *songLong = new QT_SongLongXuatThuy(jsonFile, atlasFile, scale);
	songLong->setTag(TAG_QT_SONG_LONG_XUAT_THUY);
	songLong->update(0.0f);
	songLong->setVisible(false);
	return songLong;
}

void QT_SongLongXuatThuy::initPhysic(b2World * world, float radius)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = radius / PTM_RATIO;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.shape = &circle_shape;

	fixtureDef.filter.categoryBits = BITMASK_WOODER;
	fixtureDef.filter.maskBits = BITMASK_BOSS | BITMASK_COIN_BAG | BITMASK_ENEMY | BITMASK_WOODER | BITMASK_SLASH;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef1;
	bodyDef1.type = b2_dynamicBody;
	auto posHead1 = getBoneLocation("bone32");
	bodyDef1.position.Set(posHead1.x / PTM_RATIO, posHead1.y / PTM_RATIO);

	head_1 = world->CreateBody(&bodyDef1);
	head_1->SetGravityScale(0);
	head_1->CreateFixture(&fixtureDef);
	head_1->SetSleepingAllowed(false);

	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	auto posHead2 = getBoneLocation("bone13");
	bodyDef2.position.Set(posHead2.x / PTM_RATIO, posHead2.y / PTM_RATIO);

	head_2 = world->CreateBody(&bodyDef2);
	head_2->SetGravityScale(0);
	head_2->CreateFixture(&fixtureDef);
	head_2->SetSleepingAllowed(false);
}

void QT_SongLongXuatThuy::updateDragons()
{
	//if (this->isVisible()) {
	auto posHead1 = getBoneLocation("bone32");
	head_1->SetTransform(b2Vec2(posHead1.x / PTM_RATIO, posHead1.y / PTM_RATIO), head_1->GetAngle());
	auto posHead2 = getBoneLocation("bone13");
	head_2->SetTransform(b2Vec2(posHead2.x / PTM_RATIO, posHead2.y / PTM_RATIO), head_2->GetAngle());
	//}
}

void QT_SongLongXuatThuy::changeBodyCategoryBits(uint16 mask)
{
	auto fixture1 = head_1->GetFixtureList();
	b2Filter filter1 = fixture1->GetFilterData();
	filter1.categoryBits = mask;
	fixture1->SetFilterData(filter1);

	auto fixture2 = head_2->GetFixtureList();
	b2Filter filter2 = fixture2->GetFilterData();
	filter2.categoryBits = mask;
	fixture2->SetFilterData(filter2);
}

Point QT_SongLongXuatThuy::getBoneLocation(string name)
{
	auto gun = findBone(name);
	auto pos = this->getParent()->getPosition() + Vec2(this->getScaleX()*gun->worldX, gun->worldY);
	pos = pos + this->getPosition();
	return pos;
}
