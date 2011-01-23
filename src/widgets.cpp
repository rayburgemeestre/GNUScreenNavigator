#include <iostream>
#include <fstream>
#include <map>
#include <assert.h>

#include "widget.h"

#include "menurenderer.h"
#include "menuengine.h"
#include "dispatchengine.h"

using namespace std;

Widget::Widget()
{
	type = "Null";
	parentKey = "";
}

Widget::~Widget()
{
}

void Widget::setEngine(MenuEngine *engine)
{
	engine_ = engine;
}


MenuEngine * Widget::getEngine()
{
	return engine_;
}




