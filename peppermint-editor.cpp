#include <iostream>

#include <editor/editor.h>

typedef editor::managers::EditorManager em;
typedef editor::managers::WindowManager wim;

int main() {
	em::initialise();

	while (wim::windows[0]->shouldClose() == false) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		wim::windows[0]->swapBuffers();

		glfwPollEvents();
	}

	em::destroy();

	return 0;
}