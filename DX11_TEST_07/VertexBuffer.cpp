#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics) noexcept
{
	const UINT offset = 0u;
	// 2025.08.26 - DX11 디바이스 초기화 부분들 다 비교해보기
	// 정상적으로 세팅되지 않아서 익셉션 에러 발생할 가능성 매우 높음
	GetContext(graphics)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}