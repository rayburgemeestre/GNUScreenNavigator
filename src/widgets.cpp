#include <iostream>
#include <fstream>
#include <map>
#include <assert.h>

#include "widgets.h"
#include "menurenderer.h"
#include "menuengine.h"
#include "dispatchengine.h"

using namespace std;


Widget::Widget()
{
	type = "Null";
	parentKey = "";
}

Widget::~Widget()
{
}

void Widget::setEngine(MenuEngine *engine)
{
	engine_ = engine;
}


MenuEngine * Widget::getEngine()
{
	return engine_;
}



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
	assert(engine_ != NULL);

	widget->setEngine(engine_);
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

