#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <iostream>
#include <string>
#include <map>
using std::cout;
using std::endl;
using std::string;
using std::map;

#include "json/value.h"

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
	

#endif //__WIDGET_H__
