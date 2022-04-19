#include "Window.h"
#include "WStringConvert.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try
	{
		Window wnd(800, 300, L"Donkey Fart Box");
		while (true)
		{
			if (const auto ecode = wnd.ProcessMessages())
			{
				return ecode.value();
			}
		}
	}
	catch (const CustomException& e)
	{
		const auto what = wstring_convert::to_wstring(e.what());
		const auto type = wstring_convert::to_wstring(e.GetType());
		MessageBox(nullptr, what.c_str(), type.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		const auto what = wstring_convert::to_wstring(e.what());
		MessageBox(nullptr, what.c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}