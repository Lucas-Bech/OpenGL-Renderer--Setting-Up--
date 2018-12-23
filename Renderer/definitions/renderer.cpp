#include <renderer.h>
#include <iostream>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

bool glCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error
			<< "\nFunction: " << function
			<< "\nFile: " << file
			<< "\nLine: " << line
			<< std::endl;
		return false;
	}
	return true;
}

void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}