#include "dir.h"

#include <fstream>
#include <cstdlib> // system()

using std::ofstream;
using std::ios;

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

