#include "IntroScene.h"
#include "ui_custom/Twinkle.h"
#include "manager/AudioManager.h"
#include "manager/RefManager.h"
#include "Global.h"
#include "thirdsdkhelper\AdmobHelper.h"
#include "thirdsdkhelper\FacebookHelper.h"
#include "thirdsdkhelper\SdkboxPlay.h"
#include "GameScene.h"

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

	auto _aOrigin = Director::getInstance()->getVisibleOrigin();
	this->setPosition(_aOrigin);

	if (REF->getIsShowStory() == false) {
		REF->setShowedStory();
		showStoryLayer();
	}
	else {
		initIntroScene();
	}

	return true;
}

void SceneIntro::showStoryLayer() {
	LayerColor *_pBlackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(_pBlackLayer, 1);

	auto _aStoryScene = TMXTiledMap::create("UI/UI_intro/intro_story/story_scenes.tmx");
	_aStoryScene->setScaleX(m_szVisibleSize.width / _aStoryScene->getContentSize().width);
	_aStoryScene->setScaleY(m_szVisibleSize.height / _aStoryScene->getContentSize().height);
	_aStoryScene->setPosition(Vec2(0.0f, 0.0f));
	this->addChild(_aStoryScene, 2);
	auto _aScene1 = _aStoryScene->getLayer("Scene1"); _aScene1->setOpacity(0.5f);
	auto _aScene2 = _aStoryScene->getLayer("Scene2"); _aScene2->setOpacity(0.5f);
	auto _aScene3 = _aStoryScene->getLayer("Scene3"); _aScene3->setOpacity(0.5f);
	auto _aScene4 = _aStoryScene->getLayer("Scene4"); _aScene4->setOpacity(0.5f);
	auto _aScene5 = _aStoryScene->getLayer("Scene5"); _aScene5->setOpacity(0.5f);
	_aStoryScene->update(0.0f);
	_aStoryScene->retain();

	/*Sprite *_pScene1 = Sprite::create("UI/UI_intro/intro_story/scene_1.png");
	_pScene1->getTexture()->setAntiAliasTexParameters();
	_pScene1->setScaleX(m_szVisibleSize.width / _pScene1->getContentSize().width);
	_pScene1->setScaleY(m_szVisibleSize.height / _pScene1->getContentSize().height);
	_pScene1->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2));
	_pScene1->setOpacity(0.0f);
	this->addChild(_pScene1, 2);

	Sprite *_pScene2 = Sprite::create("UI/UI_intro/intro_story/scene_2.png");
	_pScene2->getTexture()->setAntiAliasTexParameters();
	_pScene2->setScaleX(m_szVisibleSize.width / _pScene2->getContentSize().width);
	_pScene2->setScaleY(m_szVisibleSize.height / _pScene2->getContentSize().height);
	_pScene2->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2));
	_pScene2->setOpacity(0.0f);
	this->addChild(_pScene2, 2);

	Sprite *_pScene3 = Sprite::create("UI/UI_intro/intro_story/scene_3.png");
	_pScene3->getTexture()->setAntiAliasTexParameters();
	_pScene3->setScaleX(m_szVisibleSize.width / _pScene3->getContentSize().width);
	_pScene3->setScaleY(m_szVisibleSize.height / _pScene3->getContentSize().height);
	_pScene3->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2));
	_pScene3->setOpacity(0.0f);
	this->addChild(_pScene3, 2);

	Sprite *_pScene4 = Sprite::create("UI/UI_intro/intro_story/scene_4.png");
	_pScene4->getTexture()->setAntiAliasTexParameters();
	_pScene4->setScaleX(m_szVisibleSize.width / _pScene4->getContentSize().width);
	_pScene4->setScaleY(m_szVisibleSize.height / _pScene4->getContentSize().height);
	_pScene4->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2));
	_pScene4->setOpacity(0.0f);
	this->addChild(_pScene4, 2);

	Sprite *_pScene5 = Sprite::create("UI/UI_intro/intro_story/scene_5.png");
	_pScene5->getTexture()->setAntiAliasTexParameters();
	_pScene5->setScaleX(m_szVisibleSize.width / _pScene5->getContentSize().width);
	_pScene5->setScaleY(m_szVisibleSize.height / _pScene5->getContentSize().height);
	_pScene5->setPosition(Vec2(m_szVisibleSize.width / 2, m_szVisibleSize.height / 2));
	_pScene5->setOpacity(0.0f);
	this->addChild(_pScene5, 2);*/

	float _fStepTime = 0.5f;
	FadeIn *_pSceneFadeIn = FadeIn::create(_fStepTime * 4);

	/*_pScene1->runAction(Sequence::create(DelayTime::create(_fStepTime * 0), EaseIn::create(_pSceneFadeIn->clone(), 5.0f), NULL));
	_pScene2->runAction(Sequence::create(DelayTime::create(_fStepTime * 4), EaseIn::create(_pSceneFadeIn->clone(), 5.0f), NULL));
	_pScene3->runAction(Sequence::create(DelayTime::create(_fStepTime * 8), EaseIn::create(_pSceneFadeIn->clone(), 5.0f), NULL));
	_pScene4->runAction(Sequence::create(DelayTime::create(_fStepTime * 12), EaseIn::create(_pSceneFadeIn->clone(), 5.0f), NULL));
	_pScene5->runAction(Sequence::create(DelayTime::create(_fStepTime * 16), EaseIn::create(_pSceneFadeIn->clone(), 5.0f), NULL));*/
	this->runAction(Sequence::create(DelayTime::create(_fStepTime * 40),
		CCCallFunc::create(CC_CALLBACK_0(SceneIntro::hideStoryLayerAndInitIntroScene, this)), NULL));
	
	auto _pSkipNormal = Sprite::create("UI/UI_intro/intro_story/btn_skip.png");
	auto _pSkipSelected = Sprite::create("UI/UI_intro/intro_story/btn_skip.png");
	_pSkipSelected->setColor(Color3B(128, 128, 128));
	auto _aSkipButton = MenuItemSprite::create(_pSkipNormal, _pSkipSelected, CC_CALLBACK_0(SceneIntro::hideStoryLayerAndInitIntroScene, this));
	_aSkipButton->setScale(m_szVisibleSize.height / _aSkipButton->getContentSize().height * 0.05f);
	_aSkipButton->setAnchorPoint(Vec2(1.0f, 0.0f));
	_aSkipButton->setPosition(Vec2(m_szVisibleSize.width, 0.0f));

	Menu *_pMainMenu = Menu::create(_aSkipButton, NULL);
	_pMainMenu->setPosition(0, 0);
	this->addChild(_pMainMenu, 3);
}

void SceneIntro::initIntroScene() {
	//FacebookHelper::getInstance()->logout();
	SPHelper::getInstance()->signIn();
	AdmobHelper::getInstance()->showAd("top_banner");
	AudioManager::playMusic(MUSIC_MENU);

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
	
	auto _aGameName = TMXTiledMap::create("UI/UI_intro/config_gamename.tmx");
	_fTemp = _aGameName->getContentSize().height * m_szVisibleSize.width / _aGameName->getContentSize().width * 0.45f;
	if (_fTemp > m_szVisibleSize.height * 0.45f) {
		_aGameName->setScale(m_szVisibleSize.height / _aGameName->getContentSize().height * 0.45f);
	}
	else {
		_aGameName->setScale(m_szVisibleSize.width / _aGameName->getContentSize().width * 0.45f);
	}
	_aGameName->setAnchorPoint(Vec2(0.5f, 1.0f));
	_aGameName->setPosition(Vec2(m_szVisibleSize.width * 0.75f, m_szVisibleSize.height * 0.85f));
	this->addChild(_aGameName, 1);

	for (auto _aTwinkle : _aGameName->getObjectGroup("Twinkle")->getObjects()) {
		auto _aObjectTwinkle = _aTwinkle.asValueMap();
		Point _ptPositionTwinkle = Point(_aObjectTwinkle["x"].asFloat(), _aObjectTwinkle["y"].asFloat());
	
		Twinkle *_pTwinkle = Twinkle::create();
		_pTwinkle->setScale(_aGameName->getContentSize().height / _pTwinkle->getContentSize().height * 0.4f);
		_pTwinkle->setPosition(Vec2(_ptPositionTwinkle.x, _ptPositionTwinkle.y));
		_aGameName->addChild(_pTwinkle, 1);
	}

	auto _pStartNormal = Sprite::create("UI/UI_intro/btn_play.png");
	auto _pStartSelected = Sprite::create("UI/UI_intro/btn_play.png");
	_pStartSelected->setColor(Color3B(128, 128, 128));
	auto _aStartButton = MenuItemSprite::create(_pStartNormal, _pStartSelected, CC_CALLBACK_1(SceneIntro::goToMainMenuScene, this));
	_aStartButton->setScale(m_szVisibleSize.width / _aStartButton->getContentSize().width * 0.3f);
	_aStartButton->setAnchorPoint(Vec2(0.5f, 0.0f));
	_aStartButton->setPosition(Vec2(m_szVisibleSize.width * 0.75f, m_szVisibleSize.height * 0.15f));
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
}

void SceneIntro::hideStoryLayerAndInitIntroScene() {
	this->removeAllChildrenWithCleanup(true);
	initIntroScene();
}

void SceneIntro::goToMainMenuScene(Ref* p_pSender) {

	AdmobHelper::getInstance()->hide("top_banner");
	AudioManager::playSound(SOUND_BTCLICK);
	Layer *_pMenuScene = MenuLayer::create(false);
	auto _aMainMenuScene = Scene::create();
	_aMainMenuScene->addChild(_pMenuScene);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, _aMainMenuScene));

	/*auto mainScene = GameScene::create(1, 1, 0);
	mainScene->setName("gameLayer");
	mainScene->retain();
	auto hud = Hud::create();
	hud->retain();

	mainScene->setHud(hud);
	Director::getInstance()->replaceScene(TransitionFade::create(0.43f, GameScene::createScene(mainScene, hud)));*/
}