#include "menuengine.h"
#include "menurenderer.h"
#include <cassert>

MenuEngine::MenuEngine()
{
	activeMenu = NULL;
	activeMenuIndex = 0;
}

MenuEngine::~MenuEngine()
{
	activeMenu = NULL;
}
	
void MenuEngine::addMenu(Menu menu)
{
	char num[64] = "";
	sprintf(num, "%d", current_++);
	
	menu.setEngine(this);
	menus[num] = menu;

	return;
}

void MenuEngine::Run()
{
	assert(menus.size() > 0);
	assert(menus.size() > (unsigned int)activeMenuIndex);
	
	char buffer[64];
	sprintf(buffer, "%d", activeMenuIndex);
	activeMenu = &menus[buffer];

	menuRenderer_->setEngine(this);
	menuRenderer_->setMenuName(activeMenu->getName());
	menuRenderer_->resetMenuItems();
	
	// set items as strings
	map <string, string> items = activeMenu->getItems();
	map <string, string>::iterator iter;
	for (iter = items.begin(); iter != items.end(); ++iter) {
		menuRenderer_->addMenuItem(iter->first, iter->second);
	}

	menuRenderer_->Render();
}

void MenuEngine::onNextMenuRequest()
{
	activeMenuIndex++;

	if (menus.size() == (unsigned int)activeMenuIndex) {
		activeMenuIndex = 0;
	}

	Run();
}

void MenuEngine::onPreviousMenuRequest()
{
	activeMenuIndex--;

	if (activeMenuIndex < 0) {
		activeMenuIndex = menus.size() - 1;
	}

	Run();
}

void MenuEngine::onItemSelect(string item)
{
	// Send execute to menu
	activeMenu->executeWidget(item);
}

void MenuEngine::onMenuKeyPress(string key, string &selected)
{
	Widget *widget = activeMenu->getWidget(key);

	if (widget != NULL) {
		selected = widget->getParentKey();
	} else {
		selected = "";
	}
}

Json::Value MenuEngine::toJson()
{
	Json::Value root;

	for (
		menuIterator = menus.begin(); 
		menuIterator != menus.end(); 
		menuIterator++
	) {
		root[menuIterator->first] = (menuIterator->second).toJson();
	}

	return root;
}

