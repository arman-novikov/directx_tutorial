#pragma once

#include "Windows.h"
#include "CustomException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include <memory>
#include <optional>

class Window
{
public:
	class Exception : public CustomException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::wstring_view title);
	Graphics& Gfx();
	static std::optional<int> ProcessMessages();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	HWND hWnd;
	Keyboard kbd;
	Mouse mouse;
	std::unique_ptr<Graphics> pGfx;
	int width;
	int height;
};


// error exception helper macro
#define CHWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )
