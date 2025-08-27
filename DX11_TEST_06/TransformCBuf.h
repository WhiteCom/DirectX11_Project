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
	VertexConstantBuffer<DirectX::XMMATRIX> m_vcbuf;
	const Drawable& m_parent;
};
