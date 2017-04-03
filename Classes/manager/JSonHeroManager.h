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
	CC_SYNTHESIZE_READONLY(int, stt, STT);
	CC_SYNTHESIZE_READONLY(string, key, Key);
	CC_SYNTHESIZE_READONLY(string, name, Name);
	CC_SYNTHESIZE_READONLY(string, avatarPath, avatarPath);
	CC_SYNTHESIZE_READONLY(string, characterPointPath, CharacterPointPath);
	CC_SYNTHESIZE_READONLY(bool, isLocked, IsLocked);
	
	CC_SYNTHESIZE_READONLY(float, coolDownSkill1, CoolDownSkill1);
	CC_SYNTHESIZE_READONLY(float, durationSkill1, DurationSkill1);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill1, PathMainImageSkill1);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill1, PathSubImageSkill1);

	CC_SYNTHESIZE_READONLY(float, coolDownSkill2, CoolDownSkill2);
	CC_SYNTHESIZE_READONLY(float, durationSkill2, DurationSkill2);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill2, PathMainImageSkill2);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill2, PathSubImageSkill2);

	CC_SYNTHESIZE_READONLY(float, coolDownSkill3, CoolDownSkill3);
	CC_SYNTHESIZE_READONLY(float, durationSkill3, DurationSkill3);
	CC_SYNTHESIZE_READONLY(string, pathMainImageSkill3, PathMainImageSkill3);
	CC_SYNTHESIZE_READONLY(string, pathSubImageSkill3, PathSubImageSkill3);

	CC_SYNTHESIZE_READONLY(int, baseHP, BaseHP);
	CC_SYNTHESIZE_READONLY(int, level, Level);
	


private:
	Document jsonDoc;
	static JSonHeroManager *jsonHeroManager;

public:
	static JSonHeroManager* getInstance();

	int getSelectedHero();
	void readFile(int indexHero);
	void writerString(int index, const char* key, string valueString);
	void writerFloat(int index, string key, float valueFloat);
	void writerInteger(int index, string key, int valueInteger);


	
};

#define JSHERO JSonHeroManager::getInstance()

#endif // __JSON_MANAGER_H__