#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

#include <glew.h>  // Cross-platform querying and loading of OpenGL extensions
#include <glfw3.h> // Cross-platform window creation

#include <renderer.h>
#include <indexBuffer.h>
#include <vertexBuffer.h>
#include <shader.h>

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

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

	Renderer renderer;

	unsigned int vao;
	glCall(glGenVertexArrays(1, &vao));
	glCall(glBindVertexArray(vao));

	VertexBuffer vbo(sizeof(positions), positions);
	IndexBuffer ibo(indices, 6);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	Shader shader("res/shaders/red.shader");
	unsigned int shaderProgram = shader.CreateShader();
	
	glUseProgram(shaderProgram);

	shader.setUniformLocation(shaderProgram, "u_Color");
	
	float r = 0.0f;
	float increment = 0.05f;
	float RGBA[] = { r, 0.2f, 0.6f, 1.0f };

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		RGBA[0] = r;

		shader.setUniform(RGBA);

		glCall(glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, nullptr));

		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}