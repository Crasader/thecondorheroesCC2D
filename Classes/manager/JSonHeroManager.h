#ifndef __JSON_HERO_MANAGER_H__
#define __JSON_HERO_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace rapidjson;
USING_NS_CC;

class JSonHeroManager
{
public:

	JSonHeroManager();
	~JSonHeroManager();
protected:
	CC_SYNTHESIZE_READONLY(string, key, Key);
	CC_SYNTHESIZE_READONLY(string, name, Name);
	CC_SYNTHESIZE_READONLY(string, infor, Infor);
	CC_SYNTHESIZE_READONLY(string, avatarPath, avatarPath);
	CC_SYNTHESIZE_READONLY(string, characterPointPath, CharacterPointPath);
	CC_SYNTHESIZE_READONLY(string, avatarLoadingPath, AvatarLoadingPath);
	CC_SYNTHESIZE_READONLY(bool, isLocked, IsLocked);

	CC_SYNTHESIZE_READONLY(string, skill_1Name, NameOfSkill_1);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill1, CoolDownSkill1);
	CC_SYNTHESIZE_READONLY(float, durationSkill1, DurationSkill1);
	CC_SYNTHESIZE_READONLY(int, numberOfUseSkill1, NumberOfUseSkill1);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill1, PathMainImageSkill1);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill1, PathSubImageSkill1);

	CC_SYNTHESIZE_READONLY(string, skill_2Name, NameOfSkill_2);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill2, CoolDownSkill2);
	CC_SYNTHESIZE_READONLY(float, durationSkill2, DurationSkill2);
	CC_SYNTHESIZE_READONLY(int, numberOfUseSkill2, NumberOfUseSkill2);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill2, PathMainImageSkill2);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill2, PathSubImageSkill2);

	CC_SYNTHESIZE_READONLY(string, skill_3Name, NameOfSkill_3);
	CC_SYNTHESIZE_READONLY(float, coolDownSkill3, CoolDownSkill3);
	CC_SYNTHESIZE_READONLY(float, durationSkill3, DurationSkill3);
	CC_SYNTHESIZE_READONLY(int, numberOfUseSkill3, NumberOfUseSkill3);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill3, PathMainImageSkill3);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill3, PathSubImageSkill3);

	CC_SYNTHESIZE_READONLY(int, baseHP, BaseHP);
	CC_SYNTHESIZE_READONLY(int, level, Level);
	CC_SYNTHESIZE_READONLY(int, maxLevel, MaxLevel);
	CC_SYNTHESIZE_READONLY(int, percentBonusScore, BonusScore);
	CC_SYNTHESIZE_READONLY(int, percentBonusCoin, BonusCoin);

	CC_SYNTHESIZE_READONLY(int, goldPrice, GoldPrice);
	CC_SYNTHESIZE_READONLY(int, diamondPrice, DiamondPrice);

	CC_SYNTHESIZE_READONLY(string, m_sIconUnlocked, IconUnlocked);
	CC_SYNTHESIZE_READONLY(string, m_sIconUnlockedSelected, IconUnlockedSelected);
	CC_SYNTHESIZE_READONLY(string, m_sIconLocked, IconLocked);
	CC_SYNTHESIZE_READONLY(string, m_sIconLockedSelected, IconLockedSelected);

private:
	Document jsonDoc;
	static JSonHeroManager *jsonHeroManager;

public:
	static JSonHeroManager* getInstance();

	int getSelectedHero();
	void readFile(int indexHero);
	int getMaxScoreLevelX(int level);

	// index must be around 1-10
	string getTipAtX(int index);
};

#define JSHERO JSonHeroManager::getInstance()

#endif // __JSON_MANAGER_H__