#include "VertexShader.h"

VertexShader::VertexShader(Graphics& graphics, const std::wstring& path)
{
	HRESULT hr;
	hr = D3DReadFileToBlob(path.c_str(), &m_pByteCodeBlob);
	// To do...
	// Error Message - Fail to Read VertexShader

	hr = GetDevice(graphics)->CreateVertexShader(
		m_pByteCodeBlob->GetBufferPointer(),
		m_pByteCodeBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	);
	// To do...
	// Error Message - Fail to Create VertexShader
}

void VertexShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return m_pByteCodeBlob.Get();
}