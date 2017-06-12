#include "JSonHeroManager.h"
#include "RefManager.h"

JSonHeroManager* JSonHeroManager::jsonHeroManager;

JSonHeroManager::JSonHeroManager()
{
	auto herobuffer = FileUtils::getInstance()->getStringFromFile("json/Hero.json");
	this->jsonDoc.Parse(herobuffer.c_str());
}

JSonHeroManager::~JSonHeroManager()
{
}

JSonHeroManager * JSonHeroManager::getInstance()
{
	if (jsonHeroManager == nullptr)
		jsonHeroManager = new JSonHeroManager();
	return jsonHeroManager;
}

void JSonHeroManager::readFile(int p_nLanguage, int indexHero)
{
	string _arLanguages[2] = { "en", "vi" };
	this->key = jsonDoc["hero"][indexHero]["key"].GetString();
	this->name = jsonDoc["hero"][indexHero]["name"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->infor = jsonDoc["hero"][indexHero]["inforHero"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->avatarPath = jsonDoc["hero"][indexHero]["avatarPath"].GetString();
	this->characterPointPath = jsonDoc["hero"][indexHero]["characterPointPath"].GetString();
	this->selectCharacterPoint = jsonDoc["hero"][indexHero]["selectCharacterPoint"].GetString();
	this->avatarLoadingPath = jsonDoc["hero"][indexHero]["avatarLoadingPath"].GetString();

	this->isLocked = jsonDoc["hero"][indexHero]["isLocked"].GetBool();

	this->pathButtonAttack = jsonDoc["hero"][indexHero]["pathButtonAttack"].GetString();

	this->skill_1Name = jsonDoc["hero"][indexHero]["skill1Name"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->coolDownSkill1 = jsonDoc["hero"][indexHero]["coolDownSkill1"].GetDouble();
	this->durationSkill1 = jsonDoc["hero"][indexHero]["durationSkill1"].GetDouble();
	this->numberOfUseSkill1 = jsonDoc["hero"][indexHero]["numberOfUseSkill1"].GetInt();
	this->pathMainImageSkill1 = jsonDoc["hero"][indexHero]["pathMainImageSkill1"].GetString();
	this->pathSubImageSkill1 = jsonDoc["hero"][indexHero]["pathSubImageSkill1"].GetString();

	this->skill_2Name = jsonDoc["hero"][indexHero]["skill2Name"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->coolDownSkill2 = jsonDoc["hero"][indexHero]["coolDownSkill2"].GetDouble();
	this->durationSkill2 = jsonDoc["hero"][indexHero]["durationSkill2"].GetDouble();
	this->numberOfUseSkill2 = jsonDoc["hero"][indexHero]["numberOfUseSkill2"].GetInt();
	this->pathMainImageSkill2 = jsonDoc["hero"][indexHero]["pathMainImageSkill2"].GetString();
	this->pathSubImageSkill2 = jsonDoc["hero"][indexHero]["pathSubImageSkill2"].GetString();

	this->skill_3Name = jsonDoc["hero"][indexHero]["skill3Name"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->coolDownSkill3 = jsonDoc["hero"][indexHero]["coolDownSkill3"].GetDouble();
	this->durationSkill3 = jsonDoc["hero"][indexHero]["durationSkill3"].GetDouble();
	this->numberOfUseSkill3 = jsonDoc["hero"][indexHero]["numberOfUseSkill3"].GetInt();
	this->pathMainImageSkill3 = jsonDoc["hero"][indexHero]["pathMainImageSkill3"].GetString();
	this->pathSubImageSkill3 = jsonDoc["hero"][indexHero]["pathSubImageSkill3"].GetString();

	this->baseHP = jsonDoc["hero"][indexHero]["baseHP"].GetInt();
	this->level = jsonDoc["hero"][indexHero]["level"].GetInt();
	this->maxLevel = jsonDoc["hero"][indexHero]["maxLevel"].GetInt();

	this->percentBonusCoin = jsonDoc["hero"][indexHero]["percentBonusScore"].GetInt();
	this->percentBonusScore = jsonDoc["hero"][indexHero]["percentBonusCoin"].GetInt();

	this->goldPrice = jsonDoc["hero"][indexHero]["goldPrice"].GetInt();
	this->diamondPrice = jsonDoc["hero"][indexHero]["diamondPrice"].GetInt();

	this->m_sIconUnlocked = jsonDoc["hero"][indexHero]["iconUnlocked"].GetString();
	this->m_sIconUnlockedSelected = jsonDoc["hero"][indexHero]["iconUnlockedSelected"].GetString();
	this->m_sIconLocked = jsonDoc["hero"][indexHero]["iconLocked"].GetString();
	this->m_sIconLockedSelected = jsonDoc["hero"][indexHero]["iconLockedSelected"].GetString();
}

int JSonHeroManager::getGoldUpgradeLevelX(int indexHero)
{
	return jsonDoc["goldUpgradeLevel"][0][("goldUpgradeLevel_" + StringUtils::format("%i", indexHero)).c_str()].GetInt();
}

string JSonHeroManager::getTipAtX(int index)
{
	assert(index > 0 && index <= 15);
	return jsonDoc["tip"][0][REF->getLanguage() == 0 ? "en" : "vi"][("tip_" + StringUtils::format("%i", index)).c_str()].GetString();
}

string JSonHeroManager::getNotifyAtX(int index)
{
	assert(index > 0 && index <= 11);
	return jsonDoc["notification"][0][REF->getLanguage() == 0 ? "en" : "vi"][("noti_" + StringUtils::format("%i", index)).c_str()].GetString();
}