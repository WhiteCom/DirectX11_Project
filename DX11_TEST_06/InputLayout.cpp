#include "InputLayout.h"

InputLayout::InputLayout(
	Graphics& graphics,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* pVertexShaderByteCode)
{
	HRESULT hr;

	hr = GetDevice(graphics)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_pInputLayout
	);

	// To do...
	// Error Message - CreateInputLayout
}

void InputLayout::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetInputLayout(m_pInputLayout.Get());
}