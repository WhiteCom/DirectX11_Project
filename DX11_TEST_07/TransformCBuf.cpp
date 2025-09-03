#include "TransformCBuf.h"

std::unique_ptr< VertexConstantBuffer< DirectX::XMMATRIX>> TransformCBuf::m_pVcbuf;

TransformCBuf::TransformCBuf(Graphics& graphics, const Drawable& parent)
	: m_parent(parent)
{
	if (!m_pVcbuf)
	{
		m_pVcbuf = std::make_unique< VertexConstantBuffer< DirectX::XMMATRIX>>(graphics);
	}
}

void TransformCBuf::Bind(Graphics& graphics) noexcept
{
	m_pVcbuf->Update(
		graphics,
		DirectX::XMMatrixTranspose(m_parent.GetTransformXM() * graphics.GetProjection())
	);

	m_pVcbuf->Bind(graphics);
}