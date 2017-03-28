#include "DialogGameOver.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"


bool DialogGameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//this->setVisible(false);

	auto blurLayer = LayerColor::create(Color4B(0, 0, 0, 170));
	addChild(blurLayer);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto win_size = Director::getInstance()->getVisibleSize();


	background = Sprite::create("send/DialogGameOver.png");
	background->setScale(win_size.width / 2 / background->getContentSize().width);
	background->setPosition(origin + win_size / 2);
	addChild(background);





	return true;
}

DialogGameOver* DialogGameOver::create(int myScore, int myCoin)
{
	DialogGameOver* dialog = new DialogGameOver();
	dialog->score = myScore;
	dialog->coin = myCoin;

	if (dialog && dialog->init())
	{
		dialog->autorelease();
		return dialog;
	}
	else
	{
		delete dialog;
		dialog = nullptr;
		return nullptr;
	}
}





