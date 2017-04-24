#ifndef __HUD_H__
#define __HUD_H__


#include "utils/MyButton.h"
#include <spine/spine-cocos2dx.h>

USING_NS_CC;
using namespace spine;


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

	void moveCallBirdToCenterScreen(Vec2 p_ptCenterScreen);
	void cooldownCallBird();
	bool callBirdCooldown();

	void updateMultiKills(int m_nCombo);
	void runnerSkillDuration(int skillWhat, float duration);
	void runnerItem(int counter);

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

	CC_SYNTHESIZE(MenuItemImage*, btnCalling, BtnCalling);
	CC_SYNTHESIZE(MenuItemImage*, btnMagnet, BtnMagnet);
	CC_SYNTHESIZE(MenuItemImage*, btnDouleGold, BtnDoubleGold);

	CC_SYNTHESIZE(ProgressTimer*, icon_Skill, Icon_Skill);
	CC_SYNTHESIZE(ProgressTimer*, icon_Item, Icon_Item);

	CC_SYNTHESIZE(bool, btnCallingHintDone, BtnCallingHintDone);

	CC_SYNTHESIZE(SkeletonAnimation *, multiKills, MultiKills);	//DuongPM Edited for multi kills

private:
	TMXTiledMap *tmxMap;
	list<Sprite*> g_lTemp;

	Sprite *coverSkill;
	Sprite *coverItemMagnet;
	float timerSkill;
	float durationSkill;

	float timerItem;
	float durationItem;
	bool isItemActive = false;

	Menu* menu;

	void addProfile();
	void addButton();
	void createBloodBar();


	void doSuctionCoin(Ref *pSender);
	void doDoublingCoin(Ref *pSender);
	void doCalling(Ref* pSender);
	void doPause(Ref* pSender);

	void showSpecialButton();
	void createButtonX(int index, Point position);
	vector<int> getListIndexOfTypeItemBuy();
};

#endif // __HUD_H__
