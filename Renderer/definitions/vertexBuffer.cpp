#include <vertexBuffer.h>

#include <renderer.h>

VertexBuffer::VertexBuffer(int size, float data[])
{
	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}