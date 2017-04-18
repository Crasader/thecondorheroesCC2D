#include "IntroScene.h"

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

	auto _aIntroBackground = Sprite::create("UI/UI_intro/background.jpg");
	_aIntroBackground->setScaleX(m_szVisibleSize.width / _aIntroBackground->getContentSize().width); // full screen size width
	_aIntroBackground->setScaleY(m_szVisibleSize.height / _aIntroBackground->getContentSize().height); // full screen size height
	_aIntroBackground->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2)); // center screen
	this->addChild(_aIntroBackground, 0);

	auto _aCharacter = Sprite::create("UI/UI_intro/character_startmenu.png");
	_aCharacter->setScale(m_szVisibleSize.height / _aCharacter->getContentSize().height * 0.8f);
	_aCharacter->setPosition(Vec2(m_szVisibleSize.width / 4, m_szVisibleSize.height / 2)); // center screen
	this->addChild(_aCharacter, 1);

	auto _aGameName = Sprite::create("UI/UI_intro/namegame.png");
	_aGameName->setScale(m_szVisibleSize.height / _aGameName->getContentSize().height * 0.3f);
	_aGameName->setPosition(Vec2(m_szVisibleSize.width * 3 / 4, m_szVisibleSize.height * 3 / 4)); // center screen
	this->addChild(_aGameName, 1);

	auto _pStartNormal = Sprite::create("UI/UI_intro/btn_play.png");
	auto _pStartSelected = Sprite::create("UI/UI_intro/btn_play.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_1(SceneIntro::goToMainMenuScene, this));
	_aStartButton->setScale(m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.3f);
	_aStartButton->setPosition(m_szVisibleSize.width * 3 / 4, m_szVisibleSize.height / 4);
    float _nBigScale = m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.35f;
    float _nSmallScale = m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.3f;
	ScaleTo *_pZoomOut = ScaleTo::create(1.5f, _nBigScale, _nBigScale);
	ScaleTo *_pZoomIn = ScaleTo::create(1.5f, _nSmallScale, _nSmallScale);
	Sequence *_pZoomSequence = Sequence::create(_pZoomOut, _pZoomIn, NULL);
	RepeatForever* _pZoomRepeat = RepeatForever::create(_pZoomSequence);
	_aStartButton->runAction(_pZoomRepeat);

	Menu *_pMainMenu = Menu::create(_aStartButton, NULL);
	_pMainMenu->setPosition(0, 0);
	this->addChild(_pMainMenu, 3);

	auto _aParticleFeather1 = ParticleSystemQuad::create("UI/UI_intro/feather1.plist");
	_aParticleFeather1->setDuration(-1);
	_aParticleFeather1->setScale(0.5f);
	_aParticleFeather1->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	this->addChild(_aParticleFeather1, 2);

	auto _aParticleFeather2 = ParticleSystemQuad::create("UI/UI_intro/feather2.plist");
	_aParticleFeather2->setDuration(-1);
	_aParticleFeather2->setScale(0.5f);
	_aParticleFeather2->setPosition(Vec2(m_szVisibleSize.width * 0.5f, m_szVisibleSize.height));
	this->addChild(_aParticleFeather2, 2);

	this->scheduleUpdate();
	
	return true;
}

void SceneIntro::update(float p_fDelta) {
	/*if (CCRANDOM_0_1() > 0.95f) {
		Sprite *_pFeather = Sprite::create(CCRANDOM_0_1() > 0.5f ? "UI/UI_intro/feather1.png" : "UI/UI_intro/feather2.png");
		float _fFeatherScale = 0.05f + CCRANDOM_0_1() * (0.15f - 0.05f);
		_pFeather->setScale(m_szVisibleSize.height / _pFeather->getContentSize().height * _fFeatherScale);
		_pFeather->setPosition(Vec2(m_szVisibleSize.width * CCRANDOM_0_1(), m_szVisibleSize.height * 1.2f));
		this->addChild(_pFeather, 2);

		float _fTimeAction = 4.0f + CCRANDOM_0_1() * (7.0f - 4.0f);
		float _fFeatherScaleBy = (CCRANDOM_0_1() > 0.5f) ? 1.2f : 0.9f;
		_pFeather->runAction(ScaleBy::create(_fTimeAction, _fFeatherScaleBy, _fFeatherScaleBy, _fFeatherScaleBy));

		float _fXMove = m_szVisibleSize.width * CCRANDOM_0_1();
		float _fYMove = -m_szVisibleSize.height;
		_pFeather->runAction(MoveTo::create(_fTimeAction, Vec2(_fXMove, _fYMove)));
	}*/
}

void SceneIntro::goToMainMenuScene(Ref* p_pSender) {
	auto _aMainMenuScene = MenuLayer::createScene(); // create main menu scene
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, _aMainMenuScene)); // replace current scene by main menu scene, replacing duration is 500ms
}
