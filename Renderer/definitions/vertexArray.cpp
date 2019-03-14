#include <vertexArray.h>
#include <renderer.h>

VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::bind() const
{
	glCall(glBindVertexArray(m_rendererID));
}
void VertexArray::unbind() const
{
	glCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned offset = 0;
	for (unsigned i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * sizeof element.type;
	}
	
}