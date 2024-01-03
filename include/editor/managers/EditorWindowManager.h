#ifndef PPMINT_EDITOR_WINDOW_MANAGER_H
#define PPMINT_EDITOR_WINDOW_MANAGER_H

#include <vector>
#include <string>

#include <editor/EditorWindow.h>

#include <GLFW/glfw3.h>

using namespace std;

namespace editor {
	namespace managers {
		class WindowManager {
		public:
			static int status;

			static vector<Window*> windows;

			static int defaultWidth;
			static int defaultHeight;

			static string defaultTitle;

			static GLFWmonitor* defaultMonitor;
			static GLFWwindow* defaultShare;

			static void destroy();

			static Window* createWindow();
			static Window* createWindow(int width, int height);
			static Window* createWindow(int width, int height, string title);
			static Window* createWindow(int width, int height, string title, GLFWmonitor* monitor, GLFWwindow* share);

			static void setCurrentWindow(Window* win);
			static Window* getWindow();
		private:
			static Window* currentWindow;
		};
	}
}

#endif