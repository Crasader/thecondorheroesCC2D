#include "IntroScene.h"
#include "manager/AudioManager.h"
#include "Global.h"
#include "thirdsdkhelper\AdmobHelper.h"
#include "thirdsdkhelper\FacebookHelper.h"

Scene* SceneIntro::createScene() {
    auto scene = Scene::create();
	auto layer = SceneIntro::create();
    scene->addChild(layer);

    return scene;
}

bool SceneIntro::init() {
    if ( !Layer::init() ) {
        return false;
	}
	FacebookHelper::getInstance()->logout();
	
	AdmobHelper::getInstance()->showBanner();
	AudioManager::playMusic(MUSIC_MENU);

	auto origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("item/coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map/bg.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/skill.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/Select_Stage/boss_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/QuachTinh/chidori_eff.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animation/QuachTinh/rock_eff.plist");

	auto _aIntroBackground = Sprite::create("UI/UI_intro/background.jpg");
	_aIntroBackground->setScaleX(m_szVisibleSize.width / _aIntroBackground->getContentSize().width); // full screen size width
	_aIntroBackground->setScaleY(m_szVisibleSize.height / _aIntroBackground->getContentSize().height); // full screen size height
	_aIntroBackground->setPosition(origin + Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2)); // center screen

	this->addChild(_aIntroBackground, 0);

	auto _aCharacter = Sprite::create("UI/UI_intro/character_startmenu.png");
	_aCharacter->setScale(m_szVisibleSize.height / _aCharacter->getContentSize().height * 0.8f);
	_aCharacter->setPosition(origin + Vec2(m_szVisibleSize.width / 4, m_szVisibleSize.height / 2)); // center screen
	this->addChild(_aCharacter, 1);

	auto _aGameName = Sprite::create("UI/UI_intro/namegame.png");
	_aGameName->setScale(m_szVisibleSize.height / _aGameName->getContentSize().height * 0.3f);
	_aGameName->setPosition(origin + Vec2(m_szVisibleSize.width * 3 / 4, m_szVisibleSize.height * 3 / 4)); // center screen
	this->addChild(_aGameName, 1);

	auto _pStartNormal = Sprite::create("UI/UI_intro/btn_play.png");
	auto _pStartSelected = Sprite::create("UI/UI_intro/btn_play.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_1(SceneIntro::goToMainMenuScene, this));
	_aStartButton->setScale(m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.3f);

	_aStartButton->setPosition(origin.x + m_szVisibleSize.width * 3 / 4, origin.y + m_szVisibleSize.height / 4);
	ScaleBy *_pZoomOut = ScaleBy::create(1.5f, 1.1f);
	Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
	RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
	_aStartButton->runAction(_pZoomRepeat);

	Menu *_pMainMenu = Menu::create(_aStartButton, NULL);
	_pMainMenu->setPosition(0, 0);
	this->addChild(_pMainMenu, 3);

	auto _aParticleFeather1 = ParticleSystemQuad::create("UI/UI_intro/feather1.plist");
	_aParticleFeather1->setDuration(-1);
	_aParticleFeather1->setScale(0.5f);
	_aParticleFeather1->setPosition(origin + Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));

	this->addChild(_aParticleFeather1, 2);

	auto _aParticleFeather2 = ParticleSystemQuad::create("UI/UI_intro/feather2.plist");
	_aParticleFeather2->setDuration(-1);
	_aParticleFeather2->setScale(0.5f);
	_aParticleFeather2->setPosition(origin + Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	this->addChild(_aParticleFeather2, 2);
	FacebookHelper::getInstance()->login();
	FacebookHelper::getInstance()->captureScreen();
	return true;
}

void SceneIntro::goToMainMenuScene(Ref* p_pSender) {
	FacebookHelper::getInstance()->requestPostPermission();
	//FacebookHelper::getInstance()->scrShotAndDialog("SwordmanLegend");
	FacebookHelper::getInstance()->dialogPhoto("SwordManLegend");
	AdmobHelper::getInstance()->hideBanner();
	AudioManager::playSound(SOUND_BTCLICK);
	Layer *_pMenuScene = MenuLayer::create(false);
	auto _aMainMenuScene = Scene::create();
	_aMainMenuScene->addChild(_pMenuScene);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, _aMainMenuScene));
}
