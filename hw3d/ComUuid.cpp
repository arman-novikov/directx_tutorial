#include "ComUuid.h"

#include <Windows.h>
#include <WinInet.h>
#include <ShlObj.h>
#include <iostream>

int comuuid_example()
{
	auto res = CoInitialize(nullptr);
	if (res != S_OK)
	{
		std::cout << "initializing failed: " << res << std::endl;
		return res;
	}
	IActiveDesktop* pDesktop{ nullptr };
	WCHAR wszWallpaper[MAX_PATH];

	res = CoCreateInstance(
		CLSID_ActiveDesktop,
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IActiveDesktop),
		reinterpret_cast<void**>(&pDesktop)
	);
	if (res != S_OK)
	{
		std::cout << "cocreate failed: " << res << std::endl;
		return res;
	}
	pDesktop->GetWallpaper(wszWallpaper, MAX_PATH, 0);
	pDesktop->Release();

	IShellLink* pLink{ nullptr };
	CoCreateInstance(
		CLSID_ShellLink,
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IShellLink),
		reinterpret_cast<void**>(&pLink)
	);
	pLink->SetPath(wszWallpaper);

	IPersistFile* pPersist{ nullptr };
	pLink->QueryInterface(__uuidof(IPersistFile),
		reinterpret_cast<void**>(&pPersist));

	pPersist->Save(L"wallpaper.lnk", FALSE);
	pPersist->Release();
	pLink->Release();

	std::wcout << wszWallpaper;
	CoUninitialize();
	std::cin.get();
	return 0;
}