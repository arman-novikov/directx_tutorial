#include <Windows.h>

#include <string>
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(69);
        break;
    case WM_KEYDOWN:
        if (wParam == 'F')
        {
            SetWindowText(hWnd, L"Respect");
        }
        break;
    case WM_KEYUP:
        if (wParam == 'F')
        {
            SetWindowText(hWnd, L"Dangerfield");
            break;
        }

    case WM_CHAR:
        {
            static std::wstring buf;
            buf.push_back(wParam);
            SetWindowText(hWnd, buf.c_str());
        }
        break;
    case WM_LBUTTONDOWN:
        {
            POINTS pt = MAKEPOINTS(lParam);
            std::wstringstream wss;
            wss << L"(" << pt.x << L", " << pt.y << L")";
            SetWindowText(hWnd, wss.str().c_str());
        }
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    const auto pClassName = L"hw3d";
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
    HWND hWind = CreateWindowEx(
        0, pClassName, L"Happy Hard Window",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 640, 480,
        nullptr, nullptr, hInstance, nullptr
    );
    ShowWindow(hWind, SW_SHOW);

    MSG msg;
    BOOL gres;
    while ((gres = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gres == -1)
    {
        return gres;
    }
    else
    {
        return msg.wParam;
    }
}