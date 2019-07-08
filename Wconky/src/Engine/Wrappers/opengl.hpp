#ifndef WRAPPERS_OPENGL_HPP_
#define WRAPPERS_OPENGL_HPP_

//-----------------------------------------------------
// Defines & Includes
//-----------------------------------------------------

// Definitions
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "../Common/world.hpp"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

// Include GLEW & GLFW & GLM & GLIMG
#include <glload/gl_load.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Windows specific headers
#include <windows.h>

// Namespaces
using namespace glm;


//-----------------------------------------------------
// Classes
//-----------------------------------------------------

class OpenglManager {

public:
	OpenglManager(int monitor);
	virtual ~OpenglManager();

	void CreateGLWindow(std::string name, float fov, int width, int height);
	void SetupWindow();
	void Swap();
	void SetVSync(bool mode);
	bool IfOpen();

	GLFWwindow * GetHandle();
	HWND GetHWND();
	World * GetWorldHandle();

private:
	GLFWwindow* window;
	World * world;


};

#endif /* WRAPPERS_OPENGL_HPP_ */
