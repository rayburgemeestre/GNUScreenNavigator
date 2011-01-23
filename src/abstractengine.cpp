#include "abstractengine.h"

#include "abstractengine.h"
#include "menurenderer.h"

AbstractEngine::AbstractEngine()
{
	current_ = 0;
	menuRenderer_ = new MenuRenderer;
}

AbstractEngine::~AbstractEngine()
{
	delete menuRenderer_;
}

