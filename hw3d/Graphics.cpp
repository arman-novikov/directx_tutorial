#include "Graphics.h"

#include <stdexcept>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		float x, y;
	};
	static const Vertex vertices[]{
		{ 0.0f,0.5f },
		{ 0.5f,-0.5f },
		{ -0.5f,-0.5f },
	};
	static const size_t nVertices = sizeof(vertices) / sizeof(vertices[0]);
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer{nullptr};
	D3D11_BUFFER_DESC bd{};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertices;
	HRESULT hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to allocate vertex buffer");
	}
	const UINT stride = sizeof(Vertex);
	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, 0);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader{ nullptr };
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob{ nullptr };
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to read vertex blob");
	}
	hr = pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to create vertex shader");
	}
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pContext->Draw(static_cast<UINT>(sizeof(vertices)), 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}
