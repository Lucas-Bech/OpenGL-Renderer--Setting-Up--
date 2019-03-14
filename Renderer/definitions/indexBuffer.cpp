#include <indexBuffer.h>
#include <renderer.h>

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int& count) : m_count(count)
{
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
	glCall(glDeleteBuffers(1, &m_rendererID));
}
void IndexBuffer::bind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}
void IndexBuffer::unbind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}