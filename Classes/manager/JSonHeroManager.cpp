#include "JSonHeroManager.h"

JSonHeroManager* JSonHeroManager::jsonHeroManager;

JSonHeroManager::JSonHeroManager()
{
	auto herobuffer = FileUtils::getInstance()->getStringFromFile("Hero.json");
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

int JSonHeroManager::getSelectedHero()
{
	return 0;
}

void JSonHeroManager::readFile(int indexHero)
{
	this->key = jsonDoc["hero"][indexHero]["key"].GetString();
	this->name = jsonDoc["hero"][indexHero]["name"].GetString();
	this->infor = jsonDoc["hero"][indexHero]["inforHero"].GetString();
	
	this->avatarPath = jsonDoc["hero"][indexHero]["avatarPath"].GetString();
	this->characterPointPath = jsonDoc["hero"][indexHero]["characterPointPath"].GetString();
	this->avatarLoadingPath = jsonDoc["hero"][indexHero]["avatarLoadingPath"].GetString();

	this->isLocked = jsonDoc["hero"][indexHero]["isLocked"].GetBool();
	
	this->skill_1Name = jsonDoc["hero"][indexHero]["skill1Name"].GetString();
	this->coolDownSkill1 = jsonDoc["hero"][indexHero]["coolDownSkill1"].GetDouble();
	this->durationSkill1 = jsonDoc["hero"][indexHero]["durationSkill1"].GetDouble();
	this->numberOfUseSkill1 = jsonDoc["hero"][indexHero]["numberOfUseSkill1"].GetInt();
	this->pathMainImageSkill1 = jsonDoc["hero"][indexHero]["pathMainImageSkill1"].GetString();
	this->pathSubImageSkill1 = jsonDoc["hero"][indexHero]["pathSubImageSkill1"].GetString();

	this->skill_2Name = jsonDoc["hero"][indexHero]["skill2Name"].GetString();
	this->coolDownSkill2 = jsonDoc["hero"][indexHero]["coolDownSkill2"].GetDouble();
	this->durationSkill2 = jsonDoc["hero"][indexHero]["durationSkill2"].GetDouble();
	this->numberOfUseSkill2 = jsonDoc["hero"][indexHero]["numberOfUseSkill2"].GetInt();
	this->pathMainImageSkill2 = jsonDoc["hero"][indexHero]["pathMainImageSkill2"].GetString();
	this->pathSubImageSkill2 = jsonDoc["hero"][indexHero]["pathSubImageSkill2"].GetString();

	this->skill_3Name = jsonDoc["hero"][indexHero]["skill3Name"].GetString();
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
}

int JSonHeroManager::getMaxScoreLevelX(int level)
{
	assert(level > 0 && level < 20);
	return jsonDoc["maxScoreLevel"][0][("maxScoreLevel_" + StringUtils::format("%i", level)).c_str()].GetInt();
}


