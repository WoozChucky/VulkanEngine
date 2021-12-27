//
// Created by nunol on 12/22/2021.
//

#include "Display.hpp"

#include "Env/Environment.hpp"

Display::Display(S32 width,
				 S32 height,
				 const char *title,
				 const OnKeyCallback& keyCallback,
				 const OnMouseCallback& mouseCallback) {

	glfwSetErrorCallback([](int error, const char* description) -> void {
		LOGW("glfwErrorCallback", "Error(%d): %s", error, description)
	});

	this->onKeyCallback = keyCallback;
	this->mouseCallback = mouseCallback;

	//  Initialize GLFW and setup window config
	if (!glfwInit())
	{
		LOGE("glfwInit", "Initialization failed!")
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		LOGE("glfwCreateWindow", "Window or OpenGL context creation failed!")
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
		LOGD("onKeyCallback", "Key: %d", key)
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	});

	glfwSetMouseButtonCallback(window, nullptr);
	glfwSetCursorPosCallback(window, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		LOGD("glewInit", "Error: %s", glewGetErrorString(err))
		exit(EXIT_FAILURE);
	}
	LOGD("glewInit", "Using GLEW %s", glewGetString(GLEW_VERSION))

	// glfwGetFramebufferSize(window, &width, &height);

	// glViewport(0, 0, width, height);

	// Enable Vertical Sync
	glfwSwapInterval(1);

	isClosed = false;
}

Display::~Display() {
	glfwDestroyWindow(window);
	glfwTerminate();
}


bool Display::IsClosed() const {
	return isClosed;
}

void Display::Update(bool draw, bool pollEvents) {

	//  Update Display
	if (draw) {
		glfwSwapBuffers(window);
	}

	//  Check for key press etc.
	if (pollEvents) {
		glfwPollEvents();
	}

	//  Exit
	if (glfwWindowShouldClose(window)) {
		isClosed = true;
	}
}

void Display::Clear(R32 red, R32 green, R32 blue, R32 alpha) {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

std::shared_ptr<Display> Display::makeShared() {
	return std::make_shared<Display>(1024, 768, "Title", nullptr, nullptr);
}
void Display::Draw() {

	double t = glfwGetTime();
	glfwGetCursorPos(window, &x, nullptr);

	// Get window size (maybe different from the requested size)
	glfwGetWindowSize(window, &width, &height);

	// Special case: avoid division by zero below
	height = height > 0 ? height : 1;

	glViewport(0, 0, width, height);

	// Clear color buffer to black
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Select and set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.f, (GLfloat) width / (GLfloat) height, 1.f, 100.f);

	// Select and set up the model-view matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0.f, 1.f, 0.f,    // Eye-position
			  0.f, 20.f, 0.f,   // View-point
			  0.f, 0.f, 1.f);   // Up-vector

	// Draw a rotating colorful triangle
	glTranslatef(0.f, 14.f, 0.f);
	glRotatef(0.3f * (GLfloat) x + (GLfloat) t * 100.f, 0.f, 0.f, 1.f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-5.f, 0.f, -4.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(5.f, 0.f, -4.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 6.f);
	glEnd();

}

