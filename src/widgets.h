#ifndef __WIDGETS_H__
#define __WIDGETS_H__
#include <string>
#include <map>

#include <iostream>
using std::cout;
using std::endl;

#include "json/value.h"

using std::string;
using std::map;

class MenuRenderer;

//#include "menuengine.h"

//#include "abstractengine.h"
//#include "dispatchengine.h"

class MenuEngine;
class Widget
{
public:
	Widget();
	virtual ~Widget();
	virtual Json::Value toJson() = 0;
	virtual string getName() = 0;
	virtual string getValue() = 0;

	/**
	 * Widgets know their place in their parent container
	 */
	void setParentKey(string key)
	{
		parentKey = key;
	}

	string getParentKey()
	{
		return parentKey;
	}

	virtual void execute() = 0;

	void setEngine(MenuEngine *);
	MenuEngine *getEngine();
	
private:

	string parentKey;
	string type;

protected:
	MenuEngine *engine_;
};

class Null : public Widget
{
public:
	Null() { }

	Json::Value toJson() 
	{
		return Json::Value("Null!"); 
	}

	string getName()
	{
		return "Widget::Null";
	}

	string getValue()
	{
		return "Widget::NullValue";
	}

	void execute()
	{
		cout << "Widget::null does not execute" << endl;
	}

};
	
#include "menu.h"

class File : public Widget
{
public: 
	File();
	File(string);
	Json::Value toJson();

	string getName();
	string getFileName();
	string getValue();
	void execute();

private:

	string name;
	string fileName;
	
};

class Dir : public Widget
{
public: 
	Dir();
	Dir(string);
	Json::Value toJson();

	string getName();
	string getDirectoryName();
	string getValue();
	void execute();

private:

	string name;
	string directoryName;
	
};

#endif //__WIDGETS_H__
