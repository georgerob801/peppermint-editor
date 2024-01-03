#include <editor/managers/EditorWindowManager.h>

using namespace editor;

typedef editor::managers::WindowManager wm;

int wm::status = 0;
int wm::defaultWidth = 800;
int wm::defaultHeight = 450;

string wm::defaultTitle = "peppermint editor";

GLFWmonitor* wm::defaultMonitor = nullptr;
GLFWwindow* wm::defaultShare = nullptr;

editor::Window* wm::currentWindow = nullptr;

vector<Window*> wm::windows;

void wm::destroy() {
	for (unsigned int i = 0; i < wm::windows.size(); i++) {
		delete wm::windows[i];
	}
}

Window* wm::createWindow() {
	return wm::createWindow(wm::defaultWidth, wm::defaultHeight, wm::defaultTitle, wm::defaultMonitor, wm::defaultShare);
}

Window* wm::createWindow(int width, int height) {
	return wm::createWindow(width, height, wm::defaultTitle, wm::defaultMonitor, wm::defaultShare);
}

Window* wm::createWindow(int width, int height, string title) {
	return wm::createWindow(width, height, title, wm::defaultMonitor, wm::defaultShare);
}

Window* wm::createWindow(int width, int height, string title, GLFWmonitor* monitor, GLFWwindow* share) {
	Window* window = new Window(width, height, title, monitor, share);
	if (window->status == -1) wm::status = -1;

	wm::windows.push_back(window);

	return window;
}

void wm::setCurrentWindow(Window* win) {
	win->makeContextCurrent();
	wm::currentWindow = win;
}

Window* wm::getWindow() {
	return wm::currentWindow;
}