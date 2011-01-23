#ifndef __MENU_H__
#define  __MENU_H__

#include <map>
#include <tr1/memory>

#include "../widget.h"
using namespace std::tr1;

class Widget;

typedef shared_ptr<Widget> WidgetPtr;
typedef map<string, WidgetPtr > ItemsMap;
typedef ItemsMap::iterator ItemsMapI;

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
	void initialize();

	string type;
	string name;
	
	ItemsMap items;
	ItemsMapI itemIterator;
	
	//CObjectMap<string, Widget> items;
	//CObjectMap<string, Widget>::iterator itemIterator;

	int current_;

};

#endif//   __MENU_H__
