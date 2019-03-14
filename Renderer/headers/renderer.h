#pragma once

#include <glew.h>
#include <glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x)\
	glClearErrors();\
	x;\
	ASSERT(glCheckError(#x, __FILE__, __LINE__));

void glClearErrors();
bool glCheckError(const char* function, const char* file, int line);

class Renderer
{

public:
	Renderer();
	~Renderer();
	void Draw();
};