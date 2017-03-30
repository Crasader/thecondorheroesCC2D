#ifndef __HUD_H__
#define __HUD_H__

#include "MyButton.h"


class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	
	CREATE_FUNC(Hud);

	void addEvents();

	void hideButton();
	void showButton();

	void pauseIfVisible();
	void resumeIfVisible();

	void refreshControl();

	void hintSpecial(Vec2 p_ptCenterScreen);
	void cooldownSpecial();
	bool specialCooldown();


protected:

	CC_SYNTHESIZE(MenuItemImage*, pauseItem, PauseItem);
	CC_SYNTHESIZE(Sprite*, avatar, AvatarSprite);

	CC_SYNTHESIZE(Sprite*, bloodBoard, BloodBoard);
	CC_SYNTHESIZE(CCArray*, listBlood, ListBlood);

	CC_SYNTHESIZE(Sprite*, moneyBoard, MoneyBoard);
	CC_SYNTHESIZE(Label*, lbMoney, LbMoney);
	CC_SYNTHESIZE(Sprite*, scoreBoard, ScoreBoard);
	CC_SYNTHESIZE(Label*, lbScore, LbScore);

	CC_SYNTHESIZE(Sprite*, distanceBar, DistanceBar);
	CC_SYNTHESIZE(Sprite*, characterPoint, CharacterPoint);


	CC_SYNTHESIZE(MyButton*, btnAttack, BtnAttack);

	CC_SYNTHESIZE(MyButton*, btnSkill_1, BtnSkill_1);
	CC_SYNTHESIZE(MyButton*, btnSkill_2, BtnSkill_2);
	CC_SYNTHESIZE(MyButton*, btnSkill_3, BtnSkill_3);

	CC_SYNTHESIZE(MenuItemImage*, btnSpecial, BtnSpecial);
	CC_SYNTHESIZE(bool, btnSpecialHintDone, BtnSpecialHintDone);

private:
	TMXTiledMap *tmxMap;
	void addProfile();
	void addButton();
	void createBloodBar();

	void doCalling(Ref* pSender);
	void doPause(Ref* pSender);
};

#endif // __HUD_H__