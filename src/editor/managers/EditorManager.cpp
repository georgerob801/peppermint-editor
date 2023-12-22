#include <editor/managers/EditorManager.h>
#include <editor/managers/LogManager.h>

#include <glad/glad.h>
#include <editor/managers/WindowManager.h>

#include <format>

using namespace std;

typedef editor::managers::EditorManager em;
typedef editor::managers::LogManager lm;
typedef editor::managers::WindowManager wim;

void em::initialise() {
	lm::info("Starting peppermint editor");
	lm::debug("Initialising GLFW");
	glfwInit();
	lm::debug("Initialised GLFW");

	lm::debug("Setting OpenGL version to 3.3");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	lm::debug("Set OpenGL version to 3.3");
	lm::debug("Setting OpenGL to core profile");
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	lm::debug("Set OpenGL to core profile");

	wim::createWindow();
}

void em::destroy() {
	lm::info("Terminating peppermint editor");
	lm::debug("Terminating GLFW");
	glfwTerminate();
	lm::debug("Terminated GLFW");
	lm::debug("Destroying WindowManager");
	wim::destroy();
	lm::debug("Destroyed WindowManager");
	lm::debug("Destroying EditorManager");
}