#ifndef PPMINT_EDITOR_WINDOW_H
#define PPMINT_EDITOR_WINDOW_H

#include <string>

#include <GLFW/glfw3.h>

using namespace std;

namespace editor {
	class Window {
	public:
		Window(int width, int height, string title, GLFWmonitor* monitor, GLFWwindow* share);
		~Window();

		int status = 0;

		void makeContextCurrent();

		GLFWwindow* getGLFWwindow();

		void getSize(int* thing);

		void swapBuffers();

		bool shouldClose();

		void setName(string name);
	private:
		GLFWwindow* glfwWindow;
	};
}

#endif