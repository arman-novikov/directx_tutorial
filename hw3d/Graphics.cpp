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
		struct
		{
			float x, y;
		} pos;
		struct
		{
			unsigned char r, g, b, a;
		} color;
	};
	static const Vertex vertices[]
	{
		{ 0.0f,0.5f,255,0,0,0 },
		{ 0.5f,-0.5f,0,255,0,0 },
		{ -0.5f,-0.5f,0,0,255,0 },
		{ -0.3f,0.3f,0,255,0,0 },
		{ 0.3f,0.3f,0,0,255,0 },
		{ 0.0f,-0.8f,255,0,0,0 },
	};
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
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	const unsigned short indices[]
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer{nullptr};
	D3D11_BUFFER_DESC ibd{};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(indices[0]);
	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to create indeces buffer");
	}
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader{};
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to read pixel blob");
	}
	hr = pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);
	if (hr != S_OK)
	{
		throw std::runtime_error("failured to create pixel shader");
	}
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader{};
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

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout{};
	const D3D11_INPUT_ELEMENT_DESC ied[]
	{
		{
			"Position",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
		{
			"Color",
			0,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			0,
			sizeof(Vertex::pos),
			D3D11_INPUT_PER_VERTEX_DATA,
			0,
		},
	};
	hr = pDevice->CreateInputLayout(
		ied,
		static_cast<UINT>(std::size(ied)),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp{};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed(static_cast<UINT>(std::size(indices)), 0u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}
