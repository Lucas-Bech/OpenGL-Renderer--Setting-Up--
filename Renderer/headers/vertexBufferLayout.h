#include <vector>
#include <glew.h>

struct VertexBufferElement
{
	unsigned type;
	unsigned count;
	char normalized;
};

class VertexBufferLayout
{
	std::vector<VertexBufferElement> m_elements;
	unsigned m_stride;
public:
	VertexBufferLayout()
		: m_stride(0) {};

	template<typename T>
	void push(unsigned count)
	{
		static_assert(false);
	}
	template<>
	void push<float>(unsigned count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * sizeof(GL_FLOAT);
	}
	template<>
	void push<unsigned>(unsigned count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * sizeof(GL_UNSIGNED_INT);
	}
	template<>
	void push<unsigned char>(unsigned count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * sizeof(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> getElements() const
	{
		return m_elements;
	}

	inline unsigned getStride() const
	{
		return m_stride;
	}

};