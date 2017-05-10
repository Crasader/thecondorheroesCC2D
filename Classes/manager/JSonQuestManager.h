#ifndef __JSON_QUEST_MANAGER_H__
#define __JSON_QUEST_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace rapidjson;
USING_NS_CC;

class JSonQuestManager {
public:
	JSonQuestManager();
	~JSonQuestManager();

protected:
	CC_SYNTHESIZE_READONLY(string, m_sQuestName, QuestName);
	CC_SYNTHESIZE_READONLY(string, m_sQuestDescription, QuestDescription);
	CC_SYNTHESIZE_READONLY(int, m_nCompleteRequest, CompleteRequest);
	CC_SYNTHESIZE_READONLY(int, m_nGoldReward, GoldReward);
	CC_SYNTHESIZE_READONLY(int, m_nDiamondReward, DiamondReward);
	CC_SYNTHESIZE_READONLY(int, m_nStepRequest, StepRequest);
	CC_SYNTHESIZE_READONLY(int, m_nLimitRequest, LimitRequest);

	CC_SYNTHESIZE_READONLY(string, m_sDailyRewardType, DailyRewardType);
	CC_SYNTHESIZE_READONLY(int, m_nDailyRewardNumber, DailyRewardNumber);
private:
	Document jsonDoc;
	static JSonQuestManager *jsonQuestManager;

public:
	static JSonQuestManager * getInstance();

	int getNumberQuest();
	void readQuest(int p_nIndexQuest);
	void readDailyReward(int p_nIndexDailyReward);
};

#define JSQUEST JSonQuestManager::getInstance()

#endif // __JSON_QUEST_MANAGER_H__