#include "IntroScene.h"
#include "manager/AudioManager.h"
#include "Global.h"
#include "thirdsdkhelper\AdmobHelper.h"
#include "thirdsdkhelper\FacebookHelper.h"
#include "thirdsdkhelper\SdkboxPlay.h"

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
	SPHelper::getInstance()->signIn();
	
	AdmobHelper::getInstance()->showBanner();
	AudioManager::playMusic(MUSIC_MENU);

	auto _aOrigin = Director::getInstance()->getVisibleOrigin();
	this->setPosition(_aOrigin);

	auto _aIntroBackground = Sprite::create("UI/UI_intro/background.jpg");
	float _fTemp = _aIntroBackground->getContentSize().height * m_szVisibleSize.width / _aIntroBackground->getContentSize().width;
	if (_fTemp > m_szVisibleSize.height) {
		_aIntroBackground->setScale(m_szVisibleSize.width / _aIntroBackground->getContentSize().width);
	}
	else {
		_aIntroBackground->setScale(m_szVisibleSize.height / _aIntroBackground->getContentSize().height);
	}
	_aIntroBackground->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height * 0.5f));
	this->addChild(_aIntroBackground, 0);

	auto _aCharacter = Sprite::create("UI/UI_intro/character_startmenu.png");
	_fTemp = _aCharacter->getContentSize().height * m_szVisibleSize.width / _aCharacter->getContentSize().width * 0.45f;
	if (_fTemp > m_szVisibleSize.height * 0.8f) {
		_aCharacter->setScale(m_szVisibleSize.height / _aCharacter->getContentSize().height * 0.9f);
	}
	else {
		_aCharacter->setScale(m_szVisibleSize.width / _aCharacter->getContentSize().width * 0.45f);
	}
	_aCharacter->setAnchorPoint(Vec2(1.0f, 0.5f));
	_aCharacter->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height * 0.5f));
	this->addChild(_aCharacter, 1);

	auto _aGameName = Sprite::create("UI/UI_intro/namegame.png");
	_fTemp = _aGameName->getContentSize().height * m_szVisibleSize.width / _aGameName->getContentSize().width * 0.45f;
	if (_fTemp > m_szVisibleSize.height * 0.45f) {
		_aGameName->setScale(m_szVisibleSize.height / _aGameName->getContentSize().height * 0.45f);
	}
	else {
		_aGameName->setScale(m_szVisibleSize.width / _aGameName->getContentSize().width * 0.45f);
	}
	_aGameName->setAnchorPoint(Vec2(0.5f, 0.0f));
	_aGameName->setPosition(Vec2(m_szVisibleSize.width * 0.75f, m_szVisibleSize.height * 0.5f));
	this->addChild(_aGameName, 1);

	auto _pStartNormal = Sprite::create("UI/UI_intro/btn_play.png");
	auto _pStartSelected = Sprite::create("UI/UI_intro/btn_play.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_1(SceneIntro::goToMainMenuScene, this));
	_aStartButton->setScale(m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.3f);
	_aStartButton->setPosition(Vec2(m_szVisibleSize.width * 0.75f, m_szVisibleSize.height * 0.25f));
	ScaleBy *_pZoomOut = ScaleBy::create(1.5f, 1.1f);
	Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomOut->reverse(), NULL);
	RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
	_aStartButton->runAction(_pZoomRepeat);

	Menu *_pMainMenu = Menu::create(_aStartButton, NULL);
	_pMainMenu->setPosition(0, 0);
	this->addChild(_pMainMenu, 3);

	auto _aParticleFeather1 = ParticleSystemQuad::create("UI/UI_intro/feather1.plist");
	_aParticleFeather1->setDuration(-1);
	_aParticleFeather1->setScale(1.0f);
	_aParticleFeather1->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));

	this->addChild(_aParticleFeather1, 2);

	auto _aParticleFeather2 = ParticleSystemQuad::create("UI/UI_intro/feather2.plist");
	_aParticleFeather2->setDuration(-1);
	_aParticleFeather2->setScale(1.0f);
	_aParticleFeather2->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	this->addChild(_aParticleFeather2, 2);
	/*FacebookHelper::getInstance()->login();
	FacebookHelper::getInstance()->captureScreen();*/
	return true;
}

void SceneIntro::goToMainMenuScene(Ref* p_pSender) {
	//FacebookHelper::getInstance()->requestPostPermission();
	////FacebookHelper::getInstance()->scrShotAndDialog("SwordmanLegend");
	//FacebookHelper::getInstance()->dialogPhoto("SwordManLegend");
	AdmobHelper::getInstance()->hideBanner();
	AudioManager::playSound(SOUND_BTCLICK);
	Layer *_pMenuScene = MenuLayer::create(false);
	auto _aMainMenuScene = Scene::create();
	_aMainMenuScene->addChild(_pMenuScene);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, _aMainMenuScene));
}
