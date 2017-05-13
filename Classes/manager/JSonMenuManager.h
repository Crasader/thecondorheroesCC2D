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
	CC_SYNTHESIZE_READONLY(string, m_sIconFilePath, IconFilePath);
	CC_SYNTHESIZE_READONLY(int, m_nPrice, Price);
	CC_SYNTHESIZE_READONLY(string, m_sName, Name);
	CC_SYNTHESIZE_READONLY(string, m_sDescription, Description);

	CC_SYNTHESIZE_READONLY(string, m_sGoldPackName, GoldPackName);
	CC_SYNTHESIZE_READONLY(string, m_sIconGoldPackPath, IconGoldPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nCoinPackNumberGold, CoinPackNumberGold);
	CC_SYNTHESIZE_READONLY(int, m_nCoinPackDiamondPrice, CoinPackDiamondPrice);

	CC_SYNTHESIZE_READONLY(string, m_sDiamondPackName, DiamondPackName);
	CC_SYNTHESIZE_READONLY(string, m_sIconDiamondPackPath, IconDiamondPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nDiamondPackNumberDiamond, DiamondPackNumberDiamond);
	CC_SYNTHESIZE_READONLY(int, m_nDiamondPackMoneyPrice, DiamondPackMoneyPrice);

	CC_SYNTHESIZE_READONLY(string, m_sEnergyPackName, EnergyPackName);
	CC_SYNTHESIZE_READONLY(string, m_sIconEnergyPackPath, IconEnergyPackPath);
	CC_SYNTHESIZE_READONLY(int, m_nEnergyPackNumberEnergy, EnergyPackNumberEnergy);
	CC_SYNTHESIZE_READONLY(int, m_nEnergyPackDiamondPrice, EnergyPackDiamondPrice);
private:
	Document jsonDoc;
	static JSonMenuManager *jsonMenuManager;

public:
	static JSonMenuManager * getInstance();

	void readFile(int p_nFirstIndex);
	void readGoldPack(int p_nFirstIndex);
	void readDiamondPack(int p_nFirstIndex);
	void readEnergyPack(int p_nFirstIndex);
};

#define JSMENU JSonMenuManager::getInstance()

#endif // __JSON_MENU_MANAGER_H__