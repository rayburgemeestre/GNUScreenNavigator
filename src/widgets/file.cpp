#include "file.h"

#include "../dispatchengine.h"

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
