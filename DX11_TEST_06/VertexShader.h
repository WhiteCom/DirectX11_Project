#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& graphics, const std::wstring& path);
	void Bind(Graphics& graphics) noexcept override;
	ID3DBlob* GetByteCode() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
};