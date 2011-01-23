#ifndef __WIDGET_FILE_H__
#define __WIDGET_FILE_H__

#include <string>
#include <json/value.h>

#include "../widget.h"

using std::string;

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

#endif // __WIDGET_FILE_H__
