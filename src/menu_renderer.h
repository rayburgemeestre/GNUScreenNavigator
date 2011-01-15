#ifndef __MENU_RENDERER_H
#define  __MENU_RENDERER_H
#include <stdlib.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

#include <iostream>
#include <string>
#include <map>
#include <assert.h>

#include "widgets.h"

using namespace std;
using namespace Widgets;

#define COLORS_MENU_ITEM_ACTIVE 1
#define COLORS_MENU_ITEM_INACTIVE 2
#define COLORS_MENU_HEADER_TEXT 3 
#define COLORS_MENU_HEADER_BACKGROUND 4

class MenuRenderer
{
public:
	MenuRenderer();
	void setMenuName(string menuName);
	void addMenuItem(string key, string itemName);
	void resetMenuItems();
	/**
	 * Engine is used for sending menuItemSelect events to.
	 */
	void setEngine(IEngine *engine);
	void setFastItemSelect(bool);
	bool isFastItemSelect();
	int Render();

private:
	// ncurses stuff
	void _initNcurses();
	void _initColors();
	void _initWindowSizes();
	void _initMenuItems();
	void _initMenuWindow();
	void _initWindow();
	void _drawInformationText();
	void _drawWindow();
	void _runMenu();
	void _unloadMenu();
	void _unloadWindow();

	string menuName;

	map <string, string> items;
	map <string, string>::iterator itemIterator;

	IEngine *engine;

	bool fastItemSelect;

// ncurses stuff
	int windowMarginLeft;
	int windowMarginRight;
	int windowMarginTop;
	int windowMarginBottom;
	int windowWidth;
	int windowHeight;
	int subWindowWidth;
	int subWindowHeight;

	WINDOW *window1;
	MENU *menu1;
	ITEM **menu1items;
	string selectedItem;
	int state;
};
#endif
