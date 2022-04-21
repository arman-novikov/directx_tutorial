#include "App.h"
#include "WStringConvert.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try
	{
		return App{}.Go();
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