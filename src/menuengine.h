#ifndef __MENU_ENGINE_H__
#define __MENU_ENGINE_H__

#include <string>
#include <map>

using std::string;
using std::map;

#include "abstractengine.h"
#include "json/value.h"

#include "widgets/menu.h"

class MenuEngine : AbstractEngine
{
public:
	MenuEngine();
	~MenuEngine();
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

#endif // __MENU_ENGINE_H__
