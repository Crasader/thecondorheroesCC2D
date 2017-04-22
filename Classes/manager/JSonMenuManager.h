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

private:
	Document jsonDoc;
	static JSonMenuManager *jsonMenuManager;

public:
	static JSonMenuManager * getInstance();

	void readFile(int p_nFirstIndex);
};

#define JSMENU JSonMenuManager::getInstance()

#endif // __JSON_MENU_MANAGER_H__