#include "JSonQuestManager.h"

JSonQuestManager* JSonQuestManager::jsonQuestManager;

JSonQuestManager::JSonQuestManager() {
	auto menubuffer = FileUtils::getInstance()->getStringFromFile("json/Quest.json");
	this->jsonDoc.Parse(menubuffer.c_str());
}

JSonQuestManager::~JSonQuestManager() {
}
 
JSonQuestManager * JSonQuestManager::getInstance() {
	if (jsonQuestManager == nullptr)
		jsonQuestManager = new JSonQuestManager();
	return jsonQuestManager;
}

int JSonQuestManager::getNumberQuest() {
	SizeType _bResult = jsonDoc["quests"].Size();
	return (int)_bResult;
}

void JSonQuestManager::readQuest(int p_nIndexQuest) {
	this->m_sQuestName = jsonDoc["quests"][p_nIndexQuest]["questName"].GetString();
	this->m_sQuestDescription = jsonDoc["quests"][p_nIndexQuest]["questDescription"].GetString();
	this->m_nCompleteRequest = jsonDoc["quests"][p_nIndexQuest]["completeRequest"].GetInt();
	this->m_nGoldReward = jsonDoc["quests"][p_nIndexQuest]["goldReward"].GetInt();
	this->m_nDiamondReward = jsonDoc["quests"][p_nIndexQuest]["diamondReward"].GetInt();
	this->m_nStepRequest = jsonDoc["quests"][p_nIndexQuest]["stepRequest"].GetInt();
	this->m_nLimitRequest = jsonDoc["quests"][p_nIndexQuest]["limitRequest"].GetInt();
}

void JSonQuestManager::readDailyReward(int p_nIndexDailyReward) {
	this->m_sDailyRewardType = jsonDoc["daily_reward"][p_nIndexDailyReward]["type"].GetString();
	this->m_nDailyRewardNumber = jsonDoc["daily_reward"][p_nIndexDailyReward]["number"].GetInt();
}