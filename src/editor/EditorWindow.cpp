#include <editor/EditorWindow.h>

#include <editor/managers/EditorLogManager.h>

#include <format>

typedef editor::Window w;
typedef editor::managers::LogManager LogManager;

void onResize(GLFWwindow* win, int width, int height) {
	glfwMakeContextCurrent(win);

	glViewport(0, 0, width, height);
}

w::Window(int width, int height, string title, GLFWmonitor* monitor, GLFWwindow* share) {
	this->status = 0;

	LogManager::debug(format("Attempting to create {}x{} window titled '{}'", width, height, title));

	this->glfwWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share);

	if (this->glfwWindow == nullptr) {
		LogManager::critical("Failed to create GLFW window");
		this->status = -1;
	} else {
		LogManager::info(format("Created '{}' window successfully at {} for editor::Window at {}", title, (void*)this->glfwWindow, (void*)this));

		LogManager::debug(format("Setting resize handler for {}", (void*)this));
		int size[2];
		this->getSize(size);
		onResize(this->glfwWindow, size[0], size[1]);
		glfwSetFramebufferSizeCallback(this->glfwWindow, onResize);
	}
}

w::~Window() {
	glfwDestroyWindow(this->glfwWindow);
}

void w::makeContextCurrent() {
	glfwMakeContextCurrent(this->getGLFWwindow());
}

GLFWwindow* w::getGLFWwindow() {
	return this->glfwWindow;
}

void w::getSize(int* thing) {
	glfwGetWindowSize(this->getGLFWwindow(), &thing[0], &thing[1]);
}

void w::swapBuffers() {
	glfwSwapBuffers(this->getGLFWwindow());
}

bool w::shouldClose() {
	return glfwWindowShouldClose(this->getGLFWwindow());
}

void w::setName(string name) {
	glfwSetWindowTitle(this->getGLFWwindow(), name.c_str());
}