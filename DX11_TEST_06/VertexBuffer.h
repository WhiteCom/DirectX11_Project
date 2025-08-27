#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& graphics, const std::vector<V>& vertices)
		: m_stride(sizeof(V))
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GetDevice(graphics)->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	}

	void Bind(Graphics& graphics) noexcept override;

protected:
	UINT m_stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
};