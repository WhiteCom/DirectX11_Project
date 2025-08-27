#include "Topology.h"

Topology::Topology(Graphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
	: m_type(type)
{
}

void Topology::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetPrimitiveTopology(m_type);
}