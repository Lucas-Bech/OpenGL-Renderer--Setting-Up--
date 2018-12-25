#include <Shader.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

#include <glew.h>  // Cross-platform querying and loading of OpenGL extensions
#include <glfw3.h> // Cross-platform window creation

#include <renderer.h>

Shader::Shader(const std::string& filepath)
{
	m_filePath = filepath;
	ParseShader();
	std::cout << "\nVertex Shader:\n" << m_vertex << "\n\n"
		<< "Fragment Shader:\n" << m_vertex << "\n\n";
}

Shader::~Shader()
{

}

void Shader::ParseShader()
{
	enum class ShaderType : int
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::ifstream inStream(m_filePath);
	ShaderType type = ShaderType::NONE;
	std::string line;
	std::stringstream ss[2];

	while (getline(inStream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	m_vertex = ss[(int)ShaderType::VERTEX].str();
	m_fragment = ss[(int)ShaderType::FRAGMENT].str();
}

void Shader::setUniformLocation(const unsigned int& shaderProgram, const char* uniformName)
{
	glCall(m_uniformLocation = glGetUniformLocation(shaderProgram, "u_Color"));
	ASSERT(m_uniformLocation != -1);
}

void Shader::setUniform(const float RGBA[4])
{
	glCall(glUniform4f(m_uniformLocation, RGBA[0], RGBA[1], RGBA[2], RGBA[3]));
}

unsigned int Shader::CreateShader()
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, m_vertex);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, m_fragment);

	glCall(glAttachShader(program, vs));
	glCall(glAttachShader(program, fs));
	glCall(glLinkProgram(program));
	glCall(glValidateProgram(program));

	// Clean-up
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shaderID = glCreateShader(type);
	const char* shaderSrc = source.c_str();
	glCall(glShaderSource(shaderID, 1, &shaderSrc, nullptr));
	glCall(glCompileShader(shaderID));

	// Error handling
	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\nError: " << message << std::endl;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}