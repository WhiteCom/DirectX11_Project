#pragma once

#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCBuf : public Bindable
{
public:
	TransformCBuf(Graphics& graphics, const Drawable& parent);
	void Bind(Graphics& graphics) noexcept override;

private:
	static std::unique_ptr< VertexConstantBuffer< DirectX::XMMATRIX>> m_pVcbuf;
	const Drawable& m_parent;
};
