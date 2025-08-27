#include "PixelShader.h"

PixelShader::PixelShader(Graphics& graphics, const std::wstring& path)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(path.c_str(), &pBlob);

	hr = GetDevice(graphics)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_pPixelShader);
}

void PixelShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}