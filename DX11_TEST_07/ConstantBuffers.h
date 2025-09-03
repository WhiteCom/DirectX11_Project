#pragma once

#include "Bindable.h"

// 
// class ConstantBuffer (Base)
// 

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& graphics, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		HRESULT hr;
		hr = GetContext(graphics)->Map(
			m_pConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		);

		// To do...
		// Error Message - Map Failed

		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(graphics)->Unmap(m_pConstantBuffer.Get(), 0u);
	}

	ConstantBuffer(Graphics& graphics, const C& consts)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		
		HRESULT hr;
		hr = GetDevice(graphics)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);

		// To do...
		// Error Message - Create Buffer
	}

	ConstantBuffer(Graphics& graphics)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		HRESULT hr;
		hr = GetDevice(graphics)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);

		// To do...
		// Error Message - Create Buffer
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

// 
// class Vertex ConstantBuffer
// 

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

// 
// class Pixel ConstantBuffer
// 

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		GetContext(graphics)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};