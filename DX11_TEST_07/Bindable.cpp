#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& graphics) noexcept
{
	return graphics.m_d3dContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& graphics) noexcept
{
	return graphics.m_d3dDevice.Get();
}