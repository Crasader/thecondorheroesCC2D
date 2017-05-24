#include "QT_SongLongXuatThuy.h"


QT_SongLongXuatThuy::QT_SongLongXuatThuy(string jsonFile, string atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale)
{

}

QT_SongLongXuatThuy * QT_SongLongXuatThuy::create(string jsonFile, string atlasFile, float scale)
{
	QT_SongLongXuatThuy *songLong = new QT_SongLongXuatThuy(jsonFile, atlasFile, scale);
	songLong->setTag(TAG_QT_SONG_LONG_XUAT_THUY);
	songLong->update(0.0f);
	return songLong;
}

void QT_SongLongXuatThuy::initPhysic(b2World * world, float radius, Point posHead1, Point posHead2)
{
	b2CircleShape circle_shape;
	circle_shape.m_radius = radius / PTM_RATIO;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.shape = &circle_shape;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef1;
	bodyDef1.type = b2_dynamicBody;
	bodyDef1.position.Set(posHead1.x / PTM_RATIO, posHead1.y / PTM_RATIO);

	head_1 = world->CreateBody(&bodyDef1);
	head_1->CreateFixture(&fixtureDef);

	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(posHead2.x / PTM_RATIO, posHead2.y / PTM_RATIO);

	head_2 = world->CreateBody(&bodyDef2);
	head_2->CreateFixture(&fixtureDef);
}

void QT_SongLongXuatThuy::updateDragons(Point posHead1, Point posHead2)
{
	head_1->SetTransform(b2Vec2(posHead1.x / PTM_RATIO, posHead1.y / PTM_RATIO), 0);
	head_2->SetTransform(b2Vec2(posHead2.x / PTM_RATIO, posHead2.y / PTM_RATIO), 0);
}
