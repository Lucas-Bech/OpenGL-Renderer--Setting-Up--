#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

#include <glew.h>  // Cross-platform querying and loading of OpenGL extensions
#include <glfw3.h> // Cross-platform window creation

#include <shader.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x)\
	glClearErrors();\
	x;\
	ASSERT(glCheckError(#x, __FILE__, __LINE__));

static void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool glCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error
				  << "\nFunction: "	   << function
				  << "\nFile: "		   << file
				  << "\nLine: "		   << line
				  << std::endl;
		return false;
	}
	return true;
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

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(positions) / sizeof(*positions)) * sizeof(float), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(indices) / sizeof(*indices)) * sizeof(float), indices, GL_STATIC_DRAW);

	Shader shader("res/shaders/red.shader");
	unsigned int shaderProgram = shader.CreateShader();
	
	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, nullptr));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}