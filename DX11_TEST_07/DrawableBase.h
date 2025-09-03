#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized() noexcept
	{
		return !m_staticBinds.empty();
	}

	static void AddStaticBind(std::unique_ptr< Bindable> pBind) noexcept
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*pBind) != typeid(IndexBuffer));
		m_staticBinds.push_back(std::move(pBind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		assert("Attempting to add index buffer a second time" && m_indexBuffer == nullptr);
		m_indexBuffer = ibuf.get();
		m_staticBinds.push_back(std::move(ibuf));
	}

	void SetIndexFromStatic() noexcept
	{
		assert("Attemping to add index buffer a second time" && m_indexBuffer == nullptr);
		for(const auto& b : m_staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				m_indexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && m_indexBuffer != nullptr);
	}

private:
	const std::vector< std::unique_ptr< Bindable>>& GetStaticBinds() const noexcept override
	{
		return m_staticBinds;
	}

private:
	static std::vector< std::unique_ptr<Bindable>> m_staticBinds;
};

//
// Static DrawableBase Class Variable
//
template<class T>
std::vector< std::unique_ptr<Bindable>> DrawableBase<T>::m_staticBinds;