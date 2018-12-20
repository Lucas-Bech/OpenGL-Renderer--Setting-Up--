#pragma once
#include <string>

class Shader {
	std::string m_filePath;
	std::string Vertex;
	std::string Fragment;
public:
	unsigned int CreateShader();
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	unsigned CompileShader(unsigned int type, const std::string& source);

private:
	void ParseShader();
};