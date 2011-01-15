#include <iostream>
#include <fstream>
#include <map>
#include <assert.h>

#include "widgets.h"
#include "menu_renderer.h"

using namespace std;

namespace Widgets {


IEngine::IEngine()
{
	_current = 0;
	menuRenderer = new MenuRenderer;
}

IEngine::~IEngine()
{
	delete menuRenderer;
}


Engine::Engine()
{
	activeMenu = NULL;
	activeMenuIndex = 0;
}

Engine::~Engine()
{
	activeMenu = NULL;
}
	
void Engine::addMenu(Menu menu)
{
	char num[64] = "";
	sprintf(num, "%d", _current++);
	
	menu.setEngine(this);
	menus[num] = menu;

	return;
}

void Engine::Run()
{
	assert(menus.size() > 0);
	assert(menus.size() > (unsigned int)activeMenuIndex);
	
	char buffer[64];
	sprintf(buffer, "%d", activeMenuIndex);
	activeMenu = &menus[buffer];

	menuRenderer->setEngine(this);
	menuRenderer->setMenuName(activeMenu->getName());
	menuRenderer->resetMenuItems();
	
	// set items as strings
	map <string, string> items = activeMenu->getItems();
	map <string, string>::iterator iter;
	for (
		iter = items.begin();
		iter != items.end();
		iter++
	) {
		menuRenderer->addMenuItem(iter->first, iter->second);
	}

	menuRenderer->Render();
}

void Engine::onNextMenuRequest()
{
	activeMenuIndex++;

	if (menus.size() == (unsigned int)activeMenuIndex) {
		activeMenuIndex = 0;
	}

	Run();
}

void Engine::onPreviousMenuRequest()
{
	activeMenuIndex--;

	if (activeMenuIndex < 0) {
		activeMenuIndex = menus.size() - 1;
	}

	Run();
}

void Engine::onItemSelect(string item)
{
	// Send execute to menu
	activeMenu->executeWidget(item);
}

void Engine::onMenuKeyPress(string key, string &selected)
{
	Widget *widget = activeMenu->getWidget(key);

	if (widget != NULL) {
		selected = widget->getParentKey();
	} else {
		selected = "";
	}
}

Json::Value Engine::toJson()
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



DispatchEngine::DispatchEngine(Widget *widget)
{
	activeWidget = widget;
}

DispatchEngine::~DispatchEngine()
{
}
void DispatchEngine::Run()
{
	assert(activeWidget != NULL);

	menuRenderer->setEngine(this);
	menuRenderer->setMenuName("Dispatch selected file");
	menuRenderer->setFastItemSelect(true);
	
	// set items as strings
	menuRenderer->addMenuItem("v", "vim, VI-Improved v7.0 in new screen window");
	menuRenderer->addMenuItem("s", "vim+sudo, VI-Improved v7.0 ,, + root priv.");
	menuRenderer->addMenuItem("t", "tail, tail -f <file>");
	menuRenderer->addMenuItem("p", "pico editor in new screen window");
	menuRenderer->addMenuItem("c", "cat, show file contents, exit on keypress");
	menuRenderer->addMenuItem("x", "urxvt + vim + desert256 outside non256color screen in windows using xming");
	menuRenderer->addMenuItem("m", "sendmail to ray@grecom.nl");
	menuRenderer->addMenuItem("n", "sendmail to marijn@grecom.nl");
	menuRenderer->addMenuItem("o", "sendmail to trigen_@hotmail.com");

	menuRenderer->Render();
}

void DispatchEngine::onNextMenuRequest()
{
}

void DispatchEngine::onPreviousMenuRequest()
{
}

void DispatchEngine::onItemSelect(string item)
{
	string editorName;
	string editorParams;

	if (item == "v") {
		editorName = "vim";
		editorParams = "-T xterm-256color";
	}
    else if (item == "s") {
		editorName = "sudo vim";
		editorParams = "-T xterm-256color";
	}
    else if (item == "t") {
		editorName = "tail";
		editorParams = "-f";
	}
	else if (item == "p") {
		editorName = "pico";
		editorParams = "";
	}
	else if (item == "c") {
		editorName = "cat";
		editorParams = "";
	}
    else if (item == "x") {
        editorName = "urxvt";
        editorParams = "-e vim";
    }
    else if (item == "m") {
        editorName = "sendFileTo.sh";
        editorParams = "  ray@grecom.nl";
    }
    else if (item == "n") {
        editorName = "sendFileTo.sh";
        editorParams = " marijn@grecom.nl";
    }
    else if (item == "o") {
        editorName = "sendFileTo.sh";
        editorParams = " trigen_@hotmail.com";
    }
	else {
		return;
	}

	ofstream myfile;
	string fileName = activeWidget->getValue();
	string bufferName = editorName;
	string directoryName = "~/";
	string::size_type pos = fileName.find_last_of('/', fileName.length());
	if (pos != string::npos) {
		bufferName = editorName + ":" + fileName.substr(pos + 1);
		directoryName = fileName.substr(0, pos);
	}

	myfile.open ("/tmp/edit.sh", ios::trunc);
	myfile << "#!/bin/bash\n";
	myfile << "cd " << directoryName << " && ";
	myfile << editorName + " " + editorParams + " " << fileName << "\n";
	myfile.close();
	// Chmod script
	system("chmod +x /tmp/edit.sh");

	string cmd = "screen -t " + bufferName + " /tmp/edit.sh ";

	system(cmd.c_str());
}

void DispatchEngine::onMenuKeyPress(string key, string &selected)
{
	if (key == "v" || 
		key == "p" ||
		key == "c" ||
		key == "t" ||
		key == "s" ||
		key == "x" ||
		key == "m" ||
		key == "n" ||
		key == "o"
	) {
		selected = key;

	}
	else { 
		selected = "";
	}
}







Widget::Widget()
{
	type = "Null";
	parentKey = "";
}

Widget::~Widget()
{
}


void Widget::setEngine(Engine *nEngine)
{
	engine = nEngine;
}


Engine * Widget::getEngine()
{
	return engine;
}



Menu::Menu()
{ 
	name = "Menu";

	_initialize();
}

Menu::Menu(string menuName)
{
	name = menuName;

	_initialize();
}

void Menu::_initialize()
{
	_current = 0; 
}

void Menu::addItem(Widget *widget)
{

	string availableKeys = "abcdefgimnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int numAvailableKeys = availableKeys.length();
	
	if (_current >= numAvailableKeys) {
		return;
	}

	char num[2];
	num[0] = availableKeys.at(_current);
	num[1] = '\0';

	_current++;

	//items[num] = widget;
	widget->setParentKey(num);
	widget->setEngine(engine);
	items.Add(num, widget);
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
	return items.Find(key);
}

void Menu::executeWidget(string key)
{
	Widget *widget = getWidget(key);
	assert(widget != NULL);
	assert(engine != NULL);

	widget->setEngine(engine);
	widget->execute();
}

map <string, string> Menu::getItems()
{
	map <string, string> strItems;

	for (
		itemIterator = items.begin();
		itemIterator != items.end();
		itemIterator++
	) {
		strItems[itemIterator->first] = ((itemIterator->second))->getName();
	}

	return strItems;
}

Json::Value Menu::toJson() 
{
	Json::Value root;

	Json::Value menu;

	for (
		itemIterator = items.begin(); 
		itemIterator != items.end(); 
		itemIterator++
	) {
		menu[itemIterator->first] = (itemIterator->second)->toJson();
	}

	root["type"] = Json::Value("Menu");
	root["name"] = Json::Value(getName());
	root["items"] = menu;

	return root; 
	
};

File::File() 
{
}

File::File(string nfileName) 
{
	name = "[file] " + nfileName;
	fileName = nfileName;
}

Json::Value File::toJson() 
{
	Json::Value root;
	root["type"] = Json::Value("File");
	root["name"] = Json::Value(getFileName());

	return root;
};

string File::getName() 
{
	return name;
}

string File::getFileName()
{
	return fileName;
}

string File::getValue()
{
	return getFileName();
}

void File::execute()
{
	DispatchEngine engine(this);
	engine.Run();
}

Dir::Dir() 
{
}

Dir::Dir(string ndirectoryName) 
{
	// Remove trailing slashes
	string dirName;
	if (ndirectoryName.at(ndirectoryName.length() - 1) == '/') {
		dirName = ndirectoryName.substr(0, ndirectoryName.length() - 1);
	} else {
		dirName = ndirectoryName;
	}
	
	name = "[dir]  " + dirName;
	directoryName = dirName;
}

Json::Value Dir::toJson() 
{
	Json::Value root;
	root["type"] = Json::Value("Dir");
	root["name"] = Json::Value(getDirectoryName());

	return root;
};

string Dir::getName() 
{
	return name;
}

string Dir::getDirectoryName()
{
	return directoryName;
}
string Dir::getValue()
{
	return getDirectoryName();
}

void Dir::execute()
{
	string dirName = getDirectoryName();

    // My screen doesn't support chdir, hence this workaround
    //cd /var/www/ray.grecom.lan
    ofstream myfile;
    myfile.open ("/tmp/chdir.sh", ios::trunc);
    myfile << "#!/bin/bash\n";
    myfile << "export IN_SCREEN=1 && " 
		"cd " << dirName << " && "
		"screen -X redisplay && "
		"screen -X select . && "
		"bash -l\n";
    myfile.close();
    // Chmod script
    system("chmod +x /tmp/chdir.sh");

	string name = "bash";
	string::size_type pos = dirName.find_last_of('/', dirName.length());
	if (pos != string::npos) {
		name = "d:" + dirName.substr(pos + 1);
	}
	
	string cmd = "screen -t " + name + " /tmp/chdir.sh ";
	system(cmd.c_str());
}

}
