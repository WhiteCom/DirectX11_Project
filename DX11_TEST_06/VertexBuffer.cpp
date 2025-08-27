#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics) noexcept
{
	const UINT offset = 0u;
	// 2025.08.26 - DX11 ����̽� �ʱ�ȭ �κе� �� ���غ���
	// ���������� ���õ��� �ʾƼ� �ͼ��� ���� �߻��� ���ɼ� �ſ� ����
	GetContext(graphics)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}