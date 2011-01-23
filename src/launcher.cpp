#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace std;

string get_config()
{
    ifstream::pos_type size;
    char * memblock;
    string contents = "";

	stringstream ss;
	ss << getenv("HOME") << "/.launcher";

    ifstream file (ss.str().c_str(), ios::in|ios::binary|ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();

        cout << "the complete file content is in memory";
        contents = memblock;

        delete[] memblock;
    }
    else 
        cout << "Unable to open file";

    return contents;
}

//apt-get install libncurses5-dev

#include "widgets/file.h"
#include "widgets/dir.h"
#include "menuengine.h"
#include "myjson.h"

int main(int argc, char *argv[])
{
	MenuEngine engine;

	// read json
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( get_config().c_str(), root );
	if ( !parsingSuccessful ) {
		// report to the user the failure and their locations in the document.
		std::cout  << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
		return 1;
	}

	// build menu from json
	for (unsigned int i=0; i<root.size(); ++i) {
		if (root[i].getMemberNames().empty())
			continue;
		string menuName = root[i].getMemberNames()[0];
		Menu m(menuName);

		Json::Value &menuItems = root[i][menuName];
		for (unsigned int j=0; j<menuItems.size(); ++j) {
			if (menuItems[j].getMemberNames().empty())
				continue;
			string type = menuItems[j].getMemberNames()[0];
			string param = menuItems[j][type].asString();
			if (type.compare("file") == 0) {
				m.addItem(new File(param));
			} else if (type.compare("dir") == 0) {
				m.addItem(new Dir(param));
			}
		}
		engine.addMenu(m);
	}

	// create json text
	Json::Value root2 = engine.toJson();
	Json::MyStyledWriter writer;
	std::string outputConfig = writer.write(root2);
	cout << " output config: \n\n" << outputConfig << endl;

	// save json
	ofstream myfile;
	stringstream ss;
	ss << getenv("HOME") << "/.launcher";
	myfile.open (ss.str().c_str(), ios::out |  ios::trunc);
	if (myfile.is_open()) {
		myfile << outputConfig << endl;
		myfile.close();
	} else {
		cout << "Failure writing config" << endl;
		exit(0);
	}

	engine.Run();
	return 0;
}
