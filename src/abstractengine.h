#ifndef __MENU_RENDERER_H__
#define __MENU_RENDERER_H__
#include <string>

using std::string;

class MenuRenderer;

class AbstractEngine
{
public:
	AbstractEngine();
	virtual ~AbstractEngine();

	virtual void Run() = 0;
	virtual void onItemSelect(string item) = 0;
	virtual void onMenuKeyPress(string key, string &selected) = 0;
	virtual void onNextMenuRequest() = 0;
	virtual void onPreviousMenuRequest() = 0;

protected:
	MenuRenderer *menuRenderer_;
	int current_;
};
#endif // __MENU_RENDERER_H__
