#include <vertexBuffer.h>
#include <vertexBufferLayout.h>

class VertexArray
{
	unsigned m_rendererID;
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unbind() const;
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};