#include "TransformCBuf.h"

TransformCBuf::TransformCBuf(Graphics& graphics, const Drawable& parent)
	: m_vcbuf(graphics)
	, m_parent(parent)
{
}

void TransformCBuf::Bind(Graphics& graphics) noexcept
{
	m_vcbuf.Update(
		graphics,
		DirectX::XMMatrixTranspose(m_parent.GetTransformXM() * graphics.GetProjection())
	);

	m_vcbuf.Bind(graphics);
}