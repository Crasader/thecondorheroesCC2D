#include "JSonQuestManager.h"

JSonQuestManager* JSonQuestManager::jsonQuestManager;

JSonQuestManager::JSonQuestManager() {
	auto menubuffer = FileUtils::getInstance()->getStringFromFile("Quest.json");
	this->jsonDoc.Parse(menubuffer.c_str());
}

JSonQuestManager::~JSonQuestManager() {
}
 
JSonQuestManager * JSonQuestManager::getInstance() {
	if (jsonQuestManager == nullptr)
		jsonQuestManager = new JSonQuestManager();
	return jsonQuestManager;
}

int JSonQuestManager::getNumberQuestTypeOne() {
	SizeType _bResult = jsonDoc["quests_type_1"].Size();
	return (int)_bResult;
}

void JSonQuestManager::readQuestTypeOne(int p_nIndexQuest) {
	this->m_sQuestName = jsonDoc["quests_type_1"][p_nIndexQuest]["questName"].GetString();
	this->m_sQuestDescription = jsonDoc["quests_type_1"][p_nIndexQuest]["questDescription"].GetString();
	this->m_nCompleteRequest = jsonDoc["quests_type_1"][p_nIndexQuest]["completeRequest"].GetInt();
	this->m_nGoldReward = jsonDoc["quests_type_1"][p_nIndexQuest]["goldReward"].GetInt();
	this->m_nDiamondReward = jsonDoc["quests_type_1"][p_nIndexQuest]["diamondReward"].GetInt();
}

//void JSonQuestManager::readFile(int firstIndex) {
//	this->m_sIconFilePath = jsonDoc["items"][firstIndex]["iconFilePath"].GetString();
//	this->m_nPrice = jsonDoc["items"][firstIndex]["priceItem"].GetInt();
//	this->m_sName = jsonDoc["items"][firstIndex]["nameItem"].GetString();
//	this->m_sDescription = jsonDoc["items"][firstIndex]["description"].GetString();
//}