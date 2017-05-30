#include "Item.h"
#include "BaseHero.h"
#include "QuachTinh.h"
#include "GameScene.h"
#include "AudioManager.h"
#include "RefManager.h"

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
	item->bubble->setScale(item->getContentSize().height * 1.3f / item->bubble->getContentSize().height);
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
	this->changeBodyCategoryBits(0);
	this->changeBodyMaskBits(0);
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

			if (this->isVisible() && hero->getB2Body()) {
				if (((this->getPosition() -
					(hero->getPosition() + Vec2(0, hero->getB2Body()->GetFixtureList()->GetShape()->m_radius*PTM_RATIO))).length()
					< SCREEN_SIZE.height / 8)) {

					auto parentGameScene = (GameScene*) this->getParent();

					if (typeItem == Item_type::HEALTH && hero->getHealth() < hero->getMaxHealth()) {
						if (hero->getHealth() == 1)
							hero->getBloodScreen()->setVisible(false);
						hero->setHealth(hero->getHealth() + 1);
						parentGameScene->updateBloodBar(hero->getHealth() - 1, true);
						if (REF->getSelectedHero() == 4) {	// QT
							auto qt = (QuachTinh*)hero;
							qt->setCheckHealth(0);
						}
					}

					if (typeItem == Item_type::MAGNET) {
						REF->getSelectedHero() == 1 ? 
							parentGameScene->runnerItem(Item_type::MAGNET, DURATION_MAGNET * 1.15f) : 
							parentGameScene->runnerItem(Item_type::MAGNET, DURATION_MAGNET);
					}

					if (typeItem == Item_type::DOUBLE_COIN) {
						hero->setCoinRatio(2);
						parentGameScene->runnerItem(Item_type::DOUBLE_COIN, DURATION_DOUBLE_COIN);
					}

					taken = true;
					picked();
				}

			}
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

