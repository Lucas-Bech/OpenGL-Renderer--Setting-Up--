#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

#include <glew.h>  // Cross-platform querying and loading of OpenGL extensions
#include <glfw3.h> // Cross-platform window creation

static GLuint CompileShader(GLuint type, const std::string& source)
{
	GLuint shaderID = glCreateShader(type);
	const char* shaderSrc = source.c_str();
	glShaderSource(shaderID, 1, &shaderSrc, nullptr);
	glCompileShader(shaderID);

	// Error handling
	int result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex": "fragment") << " shader!\nError: " << message << std::endl;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();

	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Clean-up
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{

	enum class ShaderType : int
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::ifstream inStream(filePath);
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

	return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cerr << "Error!\n";
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao, vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(positions) / sizeof(*positions)) * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(indices) / sizeof(*indices)) * sizeof(float), indices, GL_STATIC_DRAW);

	ShaderProgramSource source = ParseShader("res/shaders/red.shader");
	std::cout << "Vertex Shader Source: " << source.VertexSource << std::endl;
	std::cout << "Fragment Shader Source: " << source.FragmentSource << std::endl;

	GLuint shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, nullptr);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}