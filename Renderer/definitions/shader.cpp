#include <Shader.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

#include <glew.h>  // Cross-platform querying and loading of OpenGL extensions
#include <glfw3.h> // Cross-platform window creation

#include <renderer.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x)\
	glClearErrors();\
	x;\
	ASSERT(glCheckError(#x, __FILE__, __LINE__));

Shader::Shader(const std::string& filepath)
{
	m_filePath = filepath;
	ParseShader();
	std::cout << "\nVertex Shader:\n" << Vertex << "\n\n"
		<< "Fragment Shader:\n" << Fragment << "\n\n";
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
	Vertex = ss[(int)ShaderType::VERTEX].str();
	Fragment = ss[(int)ShaderType::FRAGMENT].str();
}

void Shader::Bind() const
{

}

void Shader::Unbind() const
{

}

unsigned int Shader::CreateShader()
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, Vertex);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, Fragment);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Clean-up
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shaderID = glCreateShader(type);
	const char* shaderSrc = source.c_str();
	GLCall(glShaderSource(shaderID, 1, &shaderSrc, nullptr));
	GLCall(glCompileShader(shaderID));

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