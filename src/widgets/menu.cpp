#include "menu.h"

#include <cassert>

Menu::Menu()
{ 
	name = "Menu";

	initialize();
}

Menu::Menu(string menuName)
{
	name = menuName;

	initialize();
}

void Menu::initialize()
{
	current_ = 0; 
}

void Menu::addItem(Widget *widget)
{

	string availableKeys = "abcdefgimnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int numAvailableKeys = availableKeys.length();
	
	if (current_ >= numAvailableKeys) {
		return;
	}

	char num[2];
	num[0] = availableKeys.at(current_);
	num[1] = '\0';

	current_++;

	//items[num] = widget;
	widget->setParentKey(num);
	widget->setEngine(engine_);
	items[num] = WidgetPtr(widget);
}

string Menu::getName()
{
	return name;
}

void Menu::execute()
{
}

Widget * Menu::getWidget(string key)
{
	ItemsMapI i = items.find(key);
	if (i == items.end())
		return NULL;
	return (i->second).get();
}

void Menu::executeWidget(string key)
{
	Widget *widget = getWidget(key);
	assert(widget != NULL);
	assert(engine_ != NULL);

	widget->setEngine(engine_);
	widget->execute();
}

map <string, string> Menu::getItems()
{
	map <string, string> strItems;

	for (itemIterator = items.begin(); itemIterator != items.end(); ++itemIterator) {
		WidgetPtr ptr = itemIterator->second;
		strItems[itemIterator->first] = (ptr.get())->getName();
	}

	return strItems;
}

Json::Value Menu::toJson() 
{
	Json::Value root;
	Json::Value menu;
	int num = 0;
	for (itemIterator = items.begin(); itemIterator != items.end(); ++itemIterator) {
		menu[num++] = ((itemIterator->second).get())->toJson();
	}
	root[getName()] = menu;
	return root; 
	
};
