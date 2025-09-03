#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices);
	void Bind(Graphics& graphics) noexcept override;
	UINT GetCount() const noexcept;

protected:
	UINT m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
};