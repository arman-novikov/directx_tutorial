#include "Graphics.h"

#include <stdexcept>

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hWnd):
	pDevice{nullptr},
	pSwap{nullptr},
	pContext{nullptr},
	pTarget{nullptr}
{
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	if (res != S_OK)
	{
		throw std::runtime_error("D3D11CreateDeviceAndSwapChain failed");
	}
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer{nullptr};
	res = pSwap->GetBuffer( 0u, __uuidof(ID3D11Resource), &pBackBuffer);
	if (res != S_OK)
	{
		throw std::runtime_error("D3D11CreateDeviceAndSwapChain failed");
	}
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}
