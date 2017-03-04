#include "HUD.h"

bool Hud::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	auto ref = UserDefault::getInstance()->sharedUserDefault();
	/*int choice = ref->getIntegerForKey(KEY_SELECTION);*/

	//if (choice == 0) {
	
	auto jtXRatio = ref->getFloatForKey(KEYJOYSTICK_X);
	auto jtYRatio = ref->getFloatForKey(KEYJOYSTICK_Y);

	auto jumpXRatio = ref->getFloatForKey(KEYBTNJUMP_X);
	auto jumpYRatio = ref->getFloatForKey(KEYBTNJUMP_Y);

	auto fireXRatio = ref->getFloatForKey(KEYBTNFIRE_X);
	auto fireYRatio = ref->getFloatForKey(KEYBTNFIRE_Y);

	if (jtXRatio == NULL) {
		jtXRatio = 0.17f * winSize.width;
		jtYRatio = 0.20f * winSize.height;
		
		jumpXRatio = 0.73f * winSize.width;
		jumpYRatio = 0.19f * winSize.height;

		fireXRatio = 0.83f * winSize.width;
		fireYRatio = 0.38f * winSize.height;
	}

	addJoystick(jtXRatio, jtYRatio);
	addJoystickButtonJump(jumpXRatio, jumpYRatio);
	addJoystickButtonFire(fireXRatio, fireYRatio);
	addJoystickButtonPause();

	menuLife = new Sprite*[5];

	for (int i = 0; i < 5; i++) {
		menuLife[i] = Sprite::create("send/item-life.png");
		menuLife[i]->setScale(SCREEN_SIZE.height / 12 / menuLife[i]->getContentSize().height);
		menuLife[i]->setPosition(Point(origin.x + winSize.width * (0.05f + i * 0.03f), origin.y + winSize.height * 0.9f));
		addChild(menuLife[i]);
	}

	/*} else
		addButton();*/

	shield = Sprite::create("send/icon-shield.png");
	shield->setScale(SCREEN_SIZE.height / 12 / shield->getContentSize().height);
	shield->setPosition(Point(origin.x + winSize.width * 0.06f, origin.y + winSize.height * 0.78f));
	shield->setVisible(false);
	addChild(shield);

	defense = Label::createWithTTF("3", "fonts/Roboto_Light.ttf", 200);
	defense->enableBold();
	defense->setScale(SCREEN_SIZE.height / 23.0f / defense->getContentSize().height);
	defense->setPosition(shield->getPositionX() + shield->getBoundingBox().size.width / 2, 
						shield->getPositionY() - shield->getBoundingBox().size.height / 2);
	defense->setVisible(false);
	addChild(defense);

	return true;
}

void Hud::addJoystick(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 2.4f;
	Rect joystickBaseDimensions = Rect(0, 0, rect_size, rect_size);

	SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setAnchorPoint(Vec2::ZERO);
	joystickBase->setPosition(px, py);

	auto joystick_thumb = Sprite::create("send/btn-move.png");
	auto joystick_bg = Sprite::create("send/move-area.png");
	joystickBase->setThumbSprite(joystick_thumb);
	joystickBase->setBackgroundSprite(joystick_bg);
	joystickBase->setScale(SCREEN_SIZE.height / 2.4f / joystick_bg->getBoundingBox().size.height);

	joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystick->autorelease();
	joystick->retain();

	joystickBase->setJoystick(joystick);
	addChild(joystickBase);
}

void Hud::addJoystickButtonJump(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 6.0f;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-jump.png"); b->setOpacity(140);

	joystickButtonBase->setAnchorPoint(Vec2::ZERO);
	joystickButtonBase->setPosition(px, py);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-jump.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-jump2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 6.0f / b->getContentSize().height);

	btnJump = new SneakyButton();
	btnJump->initWithRect(joystickButtonRect);
	btnJump->autorelease();
	btnJump->retain();


	joystickButtonBase->setButton(btnJump);
	addChild(joystickButtonBase);
}

void Hud::addJoystickButtonFire(float px, float py)
{
	auto rect_size = SCREEN_SIZE.height / 6.0f;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/btn-shoot.png"); b->setOpacity(140);

	joystickButtonBase->setAnchorPoint(Vec2::ZERO);
	joystickButtonBase->setPosition(px, py);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/btn-shoot.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/btn-shoot2.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 6.0f / b->getContentSize().height);

	btnFire = new SneakyButton();
	btnFire->initWithRect(joystickButtonRect);
	btnFire->autorelease();
	btnFire->retain();


	joystickButtonBase->setButton(btnFire);
	addChild(joystickButtonBase);
}

void Hud::addJoystickButtonPause()
{
	auto rect_size = SCREEN_SIZE.height / 9.0f;
	Rect joystickButtonRect = Rect(0, 0, rect_size, rect_size);

	SneakyButtonSkinnedBase *joystickButtonBase = new SneakyButtonSkinnedBase();
	joystickButtonBase->init();
	auto b = Sprite::create("send/pause-button.png");
	auto origin = Director::getInstance()->getVisibleOrigin();

	joystickButtonBase->setAnchorPoint(Vec2::ZERO);
	joystickButtonBase->setPosition(origin + SCREEN_SIZE * 0.9f);
	joystickButtonBase->setDefaultSprite(b);

	joystickButtonBase->setActivatedSprite(Sprite::create("send/pause-button.png"));
	joystickButtonBase->setPressSprite(Sprite::create("send/pause-button.png"));
	joystickButtonBase->setScale(SCREEN_SIZE.height / 11.0f / b->getContentSize().height);

	btnPause = new SneakyButton();
	btnPause->initWithRect(joystickButtonRect);
	btnPause->autorelease();
	btnPause->retain();


	joystickButtonBase->setButton(btnPause);
	addChild(joystickButtonBase);
}

//void Hud::addButton()
//{
//	btnLeft = Button::create("send/move-left.png", Point(SCREEN_SIZE.width * 0.15f, SCREEN_SIZE.height * 0.165f));
//
//	btnLeft->setScale(SCREEN_SIZE.height / 5.5f / btnLeft->getContentSize().height);
//	addChild(btnLeft);
//
//
//	btnRight = Button::create("send/move-right.png", Point(SCREEN_SIZE.width * 0.33f, SCREEN_SIZE.height * 0.165f));
//	btnRight->setScale(SCREEN_SIZE.height / 5.5f / btnLeft->getContentSize().height);
//	addChild(btnRight);
//}



