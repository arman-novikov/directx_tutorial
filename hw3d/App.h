#pragma once

#include "Window.h"
#include "ChiliTimer.h"

class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
	Window wnd;
	ChiliTimer timer;
};

