#ifndef __WIDGETS_H
#define __WIDGETS_H
#include <string>
#include <map>

#include "json/value.h"

using namespace std;

class MenuRenderer;

namespace Widgets {

class IEngine
{
public:
	IEngine();
	virtual ~IEngine();

	virtual void Run() = 0;
	virtual void onItemSelect(string item) = 0;
	virtual void onMenuKeyPress(string key, string &selected) = 0;
	virtual void onNextMenuRequest() = 0;
	virtual void onPreviousMenuRequest() = 0;

protected:
	MenuRenderer *menuRenderer;
	int _current;
};

class Menu;
class Engine : IEngine
{
public:
	Engine();
	~Engine();
	void addMenu(Menu);
	Json::Value toJson();
	void Run();

	// events
	void onItemSelect(string item);
	/**
	 * Key press event in MenuRenderer is received in this handler.
	 *
	 *  @param &selected reference to the string the succesfully selected item's key 
	 *  can be stored in. the MenuRenderer will use it to update its menu.
	 */
	void onMenuKeyPress(string key, string &selected);
	void onNextMenuRequest();
	void onPreviousMenuRequest();

private:

	map <string, Menu> menus;
	map <string, Menu>::iterator menuIterator;

	int activeMenuIndex;
	Menu *activeMenu;
};

class Widget;
class DispatchEngine : public IEngine
{
public:
	DispatchEngine(Widget *);
	~DispatchEngine();
	void Run();

	void onItemSelect(string item);
	void onMenuKeyPress(string key, string &selected);
	void onNextMenuRequest();
	void onPreviousMenuRequest();

private:
	Widget *activeWidget;
};

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

	void setEngine(Engine *);
	Engine *getEngine();
	
private:

	string parentKey;
	string type;

protected:
	Engine *engine;
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
	
#include "CObjectMap.h"
typedef CObjectMap<string, Widget> ItemsMap;
typedef ItemsMap::ObjectMapI ItemsMapI;

class Menu : public Widget
{
public:

	Menu(); 
	Menu(string name); 

	void addItem(Widget *);
	Json::Value toJson();
	string getName();
	string getValue() { return "Menu::NullValue"; };
	map <string, string> getItems();
	Widget *getWidget(string key);
	void executeWidget(string key);
	void execute();

private:
	void _initialize();

	string type;
	string name;
	
	ItemsMap items;
	ItemsMapI itemIterator;
	
	//CObjectMap<string, Widget> items;
	//CObjectMap<string, Widget>::iterator itemIterator;

	int _current;

};

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


};
#endif
