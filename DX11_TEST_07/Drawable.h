#pragma once

#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
private:
	template<class T>
	friend class DrawableBase;

public:
	Drawable() = default; // �⺻ ������ �����ϵ��� ���� (default Ű����)
	Drawable(const Drawable&) = delete; // ���� �����ڴ� ��� ���� (������ ���� �߻�, delete Ű����)
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void Update(float dt) noexcept = 0;

	void Draw(Graphics& graphics) const noexcept;

protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcept;

private:
	virtual const std::vector< std::unique_ptr< Bindable>>& GetStaticBinds() const noexcept = 0;

private:
	const IndexBuffer* m_indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};