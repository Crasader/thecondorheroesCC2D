#include "Item.h"
#include "BaseHero.h"
#include "manager\AudioManager.h"

Item::Item()
{
	body = nullptr;
}

Item::~Item()
{
}

Item * Item::create(string frameName, Item_type type, Point pos)
{
	Item* item = new Item();
	item->initWithFile(frameName);
	item->typeItem = type;
	item->bubble = Sprite::create("item/bubble_item.png");
	item->bubble->setPosition(item->getContentSize() / 2);
	item->addChild(item->bubble);
	item->setPosition(pos);
	item->alpha = 0;
	item->taken = false;
	item->setTag(TAG_ITEM);
	return item;
}


void Item::initCirclePhysic(b2World * world, Point pos)
{
	B2Sprite::initCirclePhysic(world, pos);
	this->getB2Body()->GetFixtureList()->SetSensor(true);
	this->getB2Body()->SetGravityScale(0);
	this->changeBodyCategoryBits(BITMASK_ITEM);
	this->changeBodyMaskBits(BITMASK_HERO);
}


void Item::picked()
{
	AudioManager::playSound(SOUND_ITEM);
}




void Item::updateMe(BaseHero *hero)
{
	if (body != nullptr) {
		if (hero->getPositionX() + SCREEN_SIZE.width * 0.8f > this->getPositionX() && 
			hero->getPositionY() + SCREEN_SIZE.height * 0.8f  > this->getPositionY()) {

			this->setPositionX(body->GetPosition().x * PTM_RATIO);
			this->setPositionY(body->GetPosition().y * PTM_RATIO);
			this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

			auto vLenght = PI * SCREEN_SIZE.height / 12;
			auto tmp = PI / 22;
			alpha += tmp;
			auto vx = vLenght * cos(alpha) / PTM_RATIO;
			auto vy = vLenght * sin(alpha) / PTM_RATIO;
			this->body->SetLinearVelocity(b2Vec2(vx, vy) + b2Vec2(SCREEN_SIZE.width / 10.0f / PTM_RATIO, 0));

			if (alpha > 2 * PI) alpha = 0;
		}
	
	}
}

