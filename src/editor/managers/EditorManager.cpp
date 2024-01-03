#include <editor/managers/EditorManager.h>

#include <glad/glad.h>

#include <editor/managers/EditorWindowManager.h>
#include <editor/managers/GUIManager.h>
#include <editor/managers/EditorLogManager.h>

#include <peppermint-preview/peppermint.h>
#include <stb_image/stb_image.h>

#include <format>

using namespace std;

typedef editor::managers::EditorManager EditorManager;
typedef editor::managers::LogManager LogManager;
typedef editor::managers::WindowManager WindowManager;

double EditorManager::deltaTime = 0.0;
double EditorManager::desiredFramerate = 60.0;
double EditorManager::lastFrame = 0.0;
int EditorManager::status = 0;

void EditorManager::initialise() {
	stbi_set_flip_vertically_on_load(true);

	LogManager::info("Starting peppermint editor");
	LogManager::debug("Initialising GLFW");
	glfwInit();
	LogManager::debug("Initialised GLFW");

	LogManager::debug("Setting OpenGL version to 3.3");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	LogManager::debug("Set OpenGL version to 3.3");
	LogManager::debug("Setting OpenGL to core profile");
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	LogManager::debug("Set OpenGL to core profile");

	editor::Window* curWin = WindowManager::createWindow();
	glfwMaximizeWindow(curWin->getGLFWwindow());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LogManager::critical("Failed to initialise GLAD");
		EditorManager::status = -1;
		return;
	}

	LogManager::debug("Starting GUI manager");
	GUIManager::initialise();
	LogManager::debug("Started GUI manager");

	LogManager::info("Starting engine in preview mode");
	peppermint::managers::EngineManager::initialise();
	LogManager::info("Providing window link to engine input manager");
	peppermint::managers::InputManager::inputWindow = WindowManager::windows[0]->getGLFWwindow();
	LogManager::info("Disabling engine direct screen output");
	peppermint::managers::EngineManager::outputToScreen = false;

	LogManager::debug(format("Setting desired framerate to {}", 60.0));
	EditorManager::desiredFramerate = 60.0;
}

void EditorManager::destroy() {
	LogManager::info("Terminating peppermint editor");

	LogManager::debug("Terminating GUI manager");
	GUIManager::destroy();
	LogManager::debug("Terminated GUI manager");

	LogManager::debug("Terminating preview engine");
	peppermint::managers::EngineManager::destroy();
	LogManager::debug("Terminated preview engine");

	LogManager::debug("Terminating GLFW");
	glfwTerminate();
	LogManager::debug("Terminated GLFW");

	LogManager::debug("Destroying WindowManager");
	WindowManager::destroy();
	LogManager::debug("Destroyed WindowManager");

	LogManager::debug("Destroying EditorManager");
}

void EditorManager::updateDeltaTime() {
	double time = glfwGetTime();
	EditorManager::deltaTime = time - lastFrame;
	EditorManager::lastFrame = time;
}

void EditorManager::loop() {
	stbi_set_flip_vertically_on_load(false);
	GLFWimage images[2];
	images[0].pixels = stbi_load("resource/img/window-icon.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("resource/img/window-icon-small.png", &images[1].width, &images[1].height, 0, 4);

	for (unsigned int i = 0; i < WindowManager::windows.size(); i++) {
		glfwSetWindowIcon(WindowManager::windows[i]->getGLFWwindow(), 2, images);
	}

	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	while (WindowManager::windows.size() != 0) {
		EditorManager::updateDeltaTime();

		for (unsigned int i = 0; i < WindowManager::windows.size(); i++) {
			WindowManager::setCurrentWindow(WindowManager::windows[i]);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// wahey time to get stuff done™
			GUIManager::render();
		}

		if (peppermint::managers::EngineManager::worldManagers.size() != 0) {
			peppermint::managers::EngineManager::editorLoopIteration();
		}

		for (unsigned int i = 0; i < WindowManager::windows.size(); i++) WindowManager::windows[i]->swapBuffers();

		glfwPollEvents();

		for (unsigned int i = 0; i < WindowManager::windows.size(); i++) {
			if (WindowManager::windows[i]->shouldClose()) {
				editor::Window* winToDelete = WindowManager::windows[i];
				WindowManager::windows.erase(WindowManager::windows.begin() + i);
				delete winToDelete;
			}
		}

		// wait for frame time
		while (glfwGetTime() - EditorManager::lastFrame < 1.0 / EditorManager::desiredFramerate) { }
	}
}