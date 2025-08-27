#pragma once

#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
	Drawable() = default; // 기본 생성자 생성하도록 지시 (default 키워드)
	Drawable(const Drawable&) = delete; // 복사 생성자는 사용 금지 (컴파일 에러 발생, delete 키워드)
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void Update(float dt) noexcept = 0;

	void Draw(Graphics& graphics) const noexcept;

	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcept;

private:
	const IndexBuffer* m_indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};