#pragma once
#include <string>

class VertexBuffer
{
	unsigned int rendererID;

public:
	VertexBuffer(int size, float data[]);
	~VertexBuffer();

	void bind() const;
	void unbind();
};