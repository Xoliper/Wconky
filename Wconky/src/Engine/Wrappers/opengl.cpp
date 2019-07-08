#include "opengl.hpp"

OpenglManager::OpenglManager(int monitor) {

	window = NULL;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR,GLFW_RELEASE_BEHAVIOR_FLUSH);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);

	/*int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
*/
}

OpenglManager::~OpenglManager() {
	delete world;
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void OpenglManager::CreateGLWindow(std::string name, float fov, int width, int height){
	// Open a window and create its OpenGL context

	window = glfwCreateWindow( width, height, name.c_str(), NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
	}




	// Initialize GLEW
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	world = new World();
	world->SetupFov(fov, width, height);

}


void OpenglManager::SetupWindow(){
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);


	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED){
		std::cerr<<"Fail to load GLIMG"<<std::endl;
	}
}

void OpenglManager::Swap(){
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void OpenglManager::SetVSync(bool mode){
    glfwSwapInterval(mode); //0 - off, 1 - on
}

GLFWwindow* OpenglManager::GetHandle(){
	return window;
}

HWND OpenglManager::GetHWND(){
	return glfwGetWin32Window(window);
}

World * OpenglManager::GetWorldHandle(){
	return world;
}


bool OpenglManager::IfOpen(){
	return !glfwWindowShouldClose(GetHandle());
}
