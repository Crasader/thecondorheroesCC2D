#include "JSonMenuManager.h"

JSonMenuManager* JSonMenuManager::jsonMenuManager;

JSonMenuManager::JSonMenuManager() {
	auto menubuffer = FileUtils::getInstance()->getStringFromFile("Menu.json");
	this->jsonDoc.Parse(menubuffer.c_str());
}

JSonMenuManager::~JSonMenuManager() {
}
 
JSonMenuManager * JSonMenuManager::getInstance() {
	if (jsonMenuManager == nullptr)
		jsonMenuManager = new JSonMenuManager();
	return jsonMenuManager;
}

void JSonMenuManager::readFile(int firstIndex) {
	this->m_sIconFilePath = jsonDoc["items"][firstIndex]["iconFilePath"].GetString();
	this->m_nPrice = jsonDoc["items"][firstIndex]["priceItem"].GetInt();
	this->m_sName = jsonDoc["items"][firstIndex]["nameItem"].GetString();
	this->m_sDescription = jsonDoc["items"][firstIndex]["description"].GetString();
}


