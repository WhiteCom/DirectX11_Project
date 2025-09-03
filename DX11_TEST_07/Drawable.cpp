#include "Drawable.h"
#include "IndexBuffer.h" // To do...
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& graphics) const noexcept
{
	for ( auto& b : m_binds )
	{
		b->Bind(graphics);
	}

	for ( auto& b : GetStaticBinds() )
	{
		b->Bind(graphics);
	}

	graphics.DrawIndexed( m_indexBuffer->GetCount() );
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
	m_binds.push_back( std::move(bind) );
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcept
{
	assert( "Attempting to add index buffer a second time" && m_indexBuffer == nullptr );
	m_indexBuffer = indexBuffer.get();
	m_binds.push_back( std::move(indexBuffer) );
}