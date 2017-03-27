#ifndef __HUD_H__
#define __HUD_H__

#include "Button.h"


class Hud : public Layer
{
public:

	virtual bool init();
	const Size SCREEN_SIZE = Director::getInstance()->getVisibleSize();	
	CREATE_FUNC(Hud);

protected:
	CC_SYNTHESIZE(Sprite*, avatar, AvatarSprite);

	CC_SYNTHESIZE(Sprite*, bloodBoard, BloodBoard);
	CC_SYNTHESIZE(CCArray*, listBlood, ListBlood);

	CC_SYNTHESIZE(Sprite*, moneyBoard, MoneyBoard);
	CC_SYNTHESIZE(Label*, lbMoney, LbMoney);
	CC_SYNTHESIZE(Sprite*, scoreBoard, ScoreBoard);
	CC_SYNTHESIZE(Label*, lbScore, LbScore);

	CC_SYNTHESIZE(Sprite*, distanceBar, DistanceBar);
	CC_SYNTHESIZE(Sprite*, characterPoint, CharacterPoint);


	CC_SYNTHESIZE(Button*, btnAttack, BtnAttack);

	CC_SYNTHESIZE(Button*, btnSkill_1, BtnSkill_1);
	CC_SYNTHESIZE(Button*, btnSkill_2, BtnSkill_2);
	CC_SYNTHESIZE(Button*, btnSkill_3, BtnSkill_3);

	CC_SYNTHESIZE(Button*, btnCalling, BtnCalling);

private:
	TMXTiledMap *tmxMap;
	void addProfile();
	void addButton();
	void createBloodBar();
};

#endif // __HUD_H__