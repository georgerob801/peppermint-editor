#ifndef PPMINT_EDITOR_GUI_WINDOW_H
#define PPMINT_EDITOR_GUI_WINDOW_H

#include <imgui/imgui.h>

#include <string>

namespace editor {
	namespace gui {
		class GUIWindow {
		public:
			bool active = true;
			std::string name = "unnamed window";
			unsigned int discriminator = 0;

			virtual void render();
			std::string getID();
		};
	}
}

#endif