#include "JSonMenuManager.h"
#include "RefManager.h"

JSonMenuManager* JSonMenuManager::jsonMenuManager;

JSonMenuManager::JSonMenuManager() {
	auto menubuffer = FileUtils::getInstance()->getStringFromFile(String::createWithFormat("json/Menu.json")->getCString());
	this->jsonDoc.Parse(menubuffer.c_str());
}

JSonMenuManager::~JSonMenuManager() {
}

JSonMenuManager * JSonMenuManager::getInstance() {
	if (jsonMenuManager == nullptr)
		jsonMenuManager = new JSonMenuManager();
	return jsonMenuManager;
}

int JSonMenuManager::getNumberGoldPack() {
	SizeType _bResult = jsonDoc["gold_packs"].Size();
	return (int)_bResult;
}

int JSonMenuManager::getNumberDiamondPack() {
	SizeType _bResult = jsonDoc["diamond_packs"].Size();
	return (int)_bResult;
}

int JSonMenuManager::getNumberEnergyPack() {
	SizeType _bResult = jsonDoc["energy_packs"].Size();
	return (int)_bResult;
}

void JSonMenuManager::readItem(int p_nLanguage, int firstIndex) {
	string _arLanguages[2] = { "en", "vi" };
	this->m_sItemIconFilePath = jsonDoc["items"][firstIndex]["iconFilePath"].GetString();
	this->m_nItemPrice = jsonDoc["items"][firstIndex]["priceItem"].GetInt();
	this->m_sItemName = jsonDoc["items"][firstIndex]["nameItem"][_arLanguages[p_nLanguage].c_str()].GetString();
	this->m_sItemDescription = jsonDoc["items"][firstIndex]["description"][_arLanguages[p_nLanguage].c_str()].GetString();
}

void JSonMenuManager::readGoldPack(int firstIndex) {
	this->m_sIconGoldPackPath = jsonDoc["gold_packs"][firstIndex]["iconGoldPackPath"].GetString();
	this->m_nCoinPackNumberGold = jsonDoc["gold_packs"][firstIndex]["goldNumber"].GetInt();
	this->m_nCoinPackDiamondPrice = jsonDoc["gold_packs"][firstIndex]["diamondPrice"].GetInt();
}

void JSonMenuManager::readDiamondPack(int firstIndex) {
	this->m_sIconDiamondPackPath = jsonDoc["diamond_packs"][firstIndex]["iconDiamondPackPath"].GetString();
	this->m_nDiamondPackNumberDiamond = jsonDoc["diamond_packs"][firstIndex]["diamondNumber"].GetInt();
	this->m_sDiamondPackMoneyPrice = jsonDoc["diamond_packs"][firstIndex]["moneyPrice"].GetString();
}

void JSonMenuManager::readEnergyPack(int firstIndex) {
	this->m_sIconEnergyPackPath = jsonDoc["energy_packs"][firstIndex]["iconEnergyPackPath"].GetString();
	this->m_nEnergyPackNumberEnergy = jsonDoc["energy_packs"][firstIndex]["energyNumber"].GetInt();
	this->m_nEnergyPackDiamondPrice = jsonDoc["energy_packs"][firstIndex]["diamondPrice"].GetInt();
}