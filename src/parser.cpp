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

#include "widgets.h"
using namespace Widgets;

int main(int argc, char *argv[])
{

	Engine engine;

	if (true) {
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
	}
    else {
        Menu m("C++ coding");
        m.addItem(new Dir("/home/ray/"));
        m.addItem(new Dir("/home/ray/cpp/jsoncpp/trunk/jsoncpp"));
        m.addItem(new File("/home/ray/cpp/jsoncpp/trunk/jsoncpp/parser.cpp"));
        m.addItem(new File("/home/ray/cpp/jsoncpp/trunk/jsoncpp/widgets.cpp"));

        Menu m4("Home/ Latex dir");
        m4.addItem(new Dir("/home/ray"));
        m4.addItem(new Dir("/home/ray/cp"));
        m4.addItem(new Dir("/var/www/ray.grecom.lan/latex/156"));
        m4.addItem(new Dir("/var/www/ray.grecom.lan/latex"));

        Menu m2("Host: ray.grecom.lan");
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/"));
        m2.addItem(new File("/var/www/ray.grecom.lan/benchmarkpunt/webframe/logs/error.log"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/modules/mod_meldingen/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/modules/mod_meldingen/include/TDP/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/modules/mod_benchmarkpunt/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/base/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/plugins/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/PEAR/"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/PEAR_modified/"));
        m2.addItem(new File("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/PEAR_modified/LiveUser.php"));
        m2.addItem(new File("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/PEAR/LiveUser.php"));
        m2.addItem(new Dir("/var/www/ray.grecom.lan/benchmarkpunt/webframe/include/plugins/DBTable/"));

        Menu m7("Host: julien.ims.grecom.lan");
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/modules/mod_meldingen/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/modules/mod_safer/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/modules/mod_sire/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/base/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/include/plugins/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/include/PEAR/"));
        m7.addItem(new Dir("/var/www/julien.ims.grecom.lan/webframe/include/PEAR_modified/"));

        Menu m3("Host: oscar.ims.grecom.lan");
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/modules/mod_meldingen/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/modules/mod_meldingen/include/TDP/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/base/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/include/plugins/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/include/PEAR/"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/include/PEAR_modified/"));
        m3.addItem(new File("/var/www/oscar.ims.grecom.lan/webframe/include/PEAR_modified/LiveUser.php"));
        m3.addItem(new File("/var/www/oscar.ims.grecom.lan/webframe/include/PEAR/LiveUser.php"));
        m3.addItem(new Dir("/var/www/oscar.ims.grecom.lan/webframe/include/plugins/DBTable/"));

        Menu m5("Host: marijn.grecom.lan");
        m5.addItem(new Dir("/var/www/marijn.grecom.lan/webframe/webframe/"));

        Menu m6("Apache conf, et.al.");
        m6.addItem(new Dir("/usr/local/apache/conf/"));
        m6.addItem(new Dir("/usr/local/apache/logs/"));
        m6.addItem(new File("/usr/local/apache/logs/access_log"));
        m6.addItem(new File("/usr/local/apache/conf/httpd.conf"));
        engine.addMenu(m4);
        engine.addMenu(m2);
        engine.addMenu(m3);
        engine.addMenu(m7);
        engine.addMenu(m4);
        engine.addMenu(m6);
        engine.addMenu(m5);
        engine.addMenu(m);
    }

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
	

/*
	root["tab"]["items"] = new Json::Value();
	root["tab"]["items"]["0"] = new Json::Value();
	root["tab"]["items"]["0"]["type"] = new Json::Value("File");
	root["tab"]["items"]["0"]["file"] = new Json::Value("/etc/webcheck.cfg");
	root["tab"]["items"]["1"] = new Json::Value();
	root["tab"]["items"]["1"]["type"] = new Json::Value("File");
	root["tab"]["items"]["1"]["file"] = new Json::Value("/etc/apache2/apache2.conf");

	Json::StyledWriter writer;
	// Make a new JSON document for the configuration. Preserve original comments.
	std::string outputConfig = writer.write( root );

	cout << " output config: \n\n" << outputConfig << endl;
*/
	
	return 0;
}
