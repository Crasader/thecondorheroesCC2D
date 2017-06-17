#ifndef __JSON_MENU_MANAGER_H__
#define __JSON_MENU_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace rapidjson;
USING_NS_CC;

class JSonMenuManager {
public:
	JSonMenuManager();
	~JSonMenuManager();

protected:
	CC_SYNTHESIZE_READONLY(string, m_sItemIconFilePath, ItemIconFilePath);
	CC_SYNTHESIZE_READONLY(int, m_nItemPrice, ItemPrice);
	CC_SYNTHESIZE_READONLY(string, m_sItemName, ItemName);
	CC_SYNTHESIZE_READONLY(string, m_sItemDescription, ItemDescription);

	CC_SYNTHESIZE_READONLY(string, m_sIconGoldPackPath, IconGoldPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nCoinPackNumberGold, CoinPackNumberGold);
	CC_SYNTHESIZE_READONLY(int, m_nCoinPackDiamondPrice, CoinPackDiamondPrice);

	CC_SYNTHESIZE_READONLY(string, m_sIconDiamondPackPath, IconDiamondPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nDiamondPackNumberDiamond, DiamondPackNumberDiamond);
	CC_SYNTHESIZE_READONLY(string, m_sDiamondPackMoneyPrice, DiamondPackMoneyPrice);

	CC_SYNTHESIZE_READONLY(string, m_sIconEnergyPackPath, IconEnergyPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nEnergyPackNumberEnergy, EnergyPackNumberEnergy);
	CC_SYNTHESIZE_READONLY(int, m_nEnergyPackDiamondPrice, EnergyPackDiamondPrice);
private:
	Document jsonDoc;
	static JSonMenuManager *jsonMenuManager;

public:
	static JSonMenuManager * getInstance();

	string readMenuText(int p_nLanguage, int p_nIndex);

	void readItem(int p_nLanguage, int p_nFirstIndex);
	void readGoldPack(int p_nFirstIndex);
	void readDiamondPack(int p_nFirstIndex);
	void readEnergyPack(int p_nFirstIndex);

	int getNumberGoldPack();
	int getNumberDiamondPack();
	int getNumberEnergyPack();
};

#define JSMENU JSonMenuManager::getInstance()

#endif // __JSON_MENU_MANAGER_H__