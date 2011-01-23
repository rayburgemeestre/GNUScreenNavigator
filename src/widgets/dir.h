#ifndef __WIDGET_DIR_H__
#define __WIDGET_DIR_H__

#include <string>

using std::string;

#include <json/value.h>

#include "../widget.h"

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

#endif // __WIDGET_DIR_H__
