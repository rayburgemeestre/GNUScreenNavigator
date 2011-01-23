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

	// Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
	// such member.
	//std::string encoding = root.get("encoding", "UTF-8" ).asString();
	// Get the value of the member of root named 'encoding', return a 'null' value if
	// there is no such member.
	//const Json::Value plugins = root["plug-ins"];
	cout << "root size: " << root.size() << endl;
	for (unsigned int index = 0; index < root.size(); ++index ) {  // Iterates over the sequence elements.
		// Todo: use iterator here as well (obsoletes this stupid int->string conversion)
		char szIndex[64] = "";
		sprintf(szIndex, "%d", index);
		if (root[szIndex]["type"].asString().compare("Menu") == 0) {
			cout << "MENU!" << endl;
			Menu m(root[szIndex]["name"].asString());
			Json::Value items = root[szIndex]["items"];
			Json::Value::iterator iter;

			Json::Value::Members members( items.getMemberNames() );
			std::sort( members.begin(), members.end() );
			//std::string suffix = *(path.end()-1) == '.' ? "" : ".";
			for ( Json::Value::Members::iterator it = members.begin();
				  it != members.end();
				  ++it )
			{
				const std::string &name = *it;
				//printValueTree( fout, value[name], path + suffix + name );
				cout << "found: " << name << endl;
				cout << "found: " << items[name]["type"].asString() << endl;
				cout << "found: " << items[name]["name"].asString() << endl;
				if (items[name]["type"].asString().compare("File") == 0) {
					m.addItem(new File(items[name]["name"].asString()));
				}
				else if (items[name]["type"].asString().compare("Dir") == 0) {
					m.addItem(new Dir(items[name]["name"].asString()));
				}
			}
			engine.addMenu(m);
		}
	}

	//root["encoding"] = "overruled";
	//root["encoding2"] = new Json::Value;

	Json::Value root2 = engine.toJson()	;

	Json::StyledWriter writer;
	// Make a new JSON document for the configuration. Preserve original comments.
	std::string outputConfig = writer.write( root2 );
	cout << " output config: \n\n" << outputConfig << endl;

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
