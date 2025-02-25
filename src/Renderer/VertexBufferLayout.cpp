#include "VertexBufferLayout.h"


namespace RenderEngine
{
	VertexBufferLayout::VertexBufferLayout()
		: m_stride(0)
	{}

	void VertexBufferLayout::reserve(const size_t count)
	{
		m_layoutElements.reserve(count);
	}

	void VertexBufferLayout::addElementLayoutFloat(const unsigned int count, const bool normalized)
	{
		m_layoutElements.push_back({ count, GL_FLOAT, normalized,  static_cast<unsigned int>(count * sizeof(GLfloat)) });
		m_stride += m_layoutElements.back().size;
	}
}
