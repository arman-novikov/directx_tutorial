#include "Window.h"


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Window wnd(800, 300, L"Donkey Fart Box");
	Window wnd_chebur(800, 300, L"Cheburek");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// check if GetMessage call itself worked
	if (gResult == -1)
	{
		return -1;
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}