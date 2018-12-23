#pragma once
#include <string>

class IndexBuffer
{
	unsigned int m_rendererID;
	unsigned int m_count;

public:
	IndexBuffer(const unsigned int* data, const unsigned int& count);
	~IndexBuffer();

	void bind();
	void unbind();
};