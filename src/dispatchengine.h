#ifndef __DISPATCH_ENGINE_H__
#define __DISPATCH_ENGINE_H__

#include <string>
using std::string;

#include "abstractengine.h"

class Widget;
class DispatchEngine : public AbstractEngine
{
public:
	DispatchEngine(Widget *);
	~DispatchEngine();
	void Run();

	void onItemSelect(string item);
	void onMenuKeyPress(string key, string &selected);
	void onNextMenuRequest();
	void onPreviousMenuRequest();

private:
	Widget *activeWidget;
};

#endif // __DISPATCH_ENGINE_H__
