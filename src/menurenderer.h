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

#define COLORS_MENU_ITEM_ACTIVE 1
#define COLORS_MENU_ITEM_INACTIVE 2
#define COLORS_MENU_HEADER_TEXT 3 
#define COLORS_MENU_HEADER_BACKGROUND 4

class AbstractEngine;

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
	void setEngine(AbstractEngine *engine);
	void setFastItemSelect(bool);
	bool isFastItemSelect();
	int Render();

private:
	// ncurses stuff
	void initNcurses();
	void initColors();
	void initWindowSizes();
	void initMenuItems();
	void initMenuWindow();
	void initWindow();
	void drawInformationText();
	void drawWindow();
	void runMenu();
	void unloadMenu();
	void unloadWindow();

	string menuName_;

	map <string, string> items_;
	map <string, string>::iterator itemIterator_;

	AbstractEngine *engine_;

	bool fastItemSelect_;

// ncurses stuff
	int windowMarginLeft_;
	int windowMarginRight_;
	int windowMarginTop_;
	int windowMarginBottom_;
	int windowWidth_;
	int windowHeight_;
	int subWindowWidth_;
	int subWindowHeight_;

	WINDOW *window1_;
	MENU *menu1_;
	ITEM **menu1items_;
	string selectedItem_;
	int state_;
};
#endif
