#include "dispatchengine.h"

#include <string>
#include <fstream>
#include <cassert>

using std::string;
using std::ofstream;

#include "menurenderer.h"


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

	menuRenderer_->setEngine(this);
	menuRenderer_->setMenuName("Dispatch selected file");
	menuRenderer_->setFastItemSelect(true);
	
	// set items as strings
	menuRenderer_->addMenuItem("v", "vim, VI-Improved v7.0 in new screen window");
	menuRenderer_->addMenuItem("s", "vim+sudo, VI-Improved v7.0 ,, + root priv.");
	menuRenderer_->addMenuItem("t", "tail, tail -f <file>");
	menuRenderer_->addMenuItem("p", "pico editor in new screen window");
	menuRenderer_->addMenuItem("c", "cat, show file contents, exit on keypress");
	menuRenderer_->addMenuItem("x", "urxvt + vim + desert256 outside non256color screen in windows using xming");

	menuRenderer_->Render();
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
	} else if (item == "s") {
		editorName = "sudo vim";
		editorParams = "-T xterm-256color";
	} else if (item == "t") {
		editorName = "tail";
		editorParams = "-f";
	} else if (item == "p") {
		editorName = "pico";
		editorParams = "";
	} else if (item == "c") {
		editorName = "cat";
		editorParams = "";
	} else if (item == "x") {
        editorName = "urxvt";
        editorParams = "-e vim";
    } else {
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
	// chmod script
	system("chmod +x /tmp/edit.sh");

	string cmd = "screen -t " + bufferName + " /tmp/edit.sh ";

	system(cmd.c_str());
}

void DispatchEngine::onMenuKeyPress(string key, string &selected)
{
	if (key == "v" || key == "p" || key == "c" || key == "t" ||
		key == "s" || key == "x"
	) {
		selected = key;
	} else { 
		selected = "";
	}
}
