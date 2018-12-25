#pragma once
#include <string>

class Shader {

	std::string m_filePath; // Shader source file path (Vertex + Fragment)

	// Shaders
	std::string m_vertex;
	std::string m_fragment;

	int m_uniformLocation;

public:
	unsigned int CreateShader();
	Shader(const std::string& filepath);
	~Shader();

	void setUniformLocation(const unsigned int& shaderProgram, const char* uniformName);
	void setUniform(const float RGBA[4]);

	void Bind() const;
	void Unbind() const;
	unsigned CompileShader(unsigned int type, const std::string& source);

private:
	void ParseShader();
};