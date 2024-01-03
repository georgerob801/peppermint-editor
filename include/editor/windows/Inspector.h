#ifndef PPMINT_EDITOR_INSPECTOR_H
#define PPMINT_EDITOR_INSPECTOR_H

#include <editor/windows/GUIWindow.h>

#include <editor/serialisation/GUIInfo.hpp>
#include <editor/serialisation/ComponentParameterRegister.h>

namespace editor {
	namespace gui {
		namespace windows {
			class Inspector : public GUIWindow {
			public:
				Inspector();
				void render();

				enum SelectedState {
					DESELECTED,
					UNKNOWN,
					GAME_OBJECT,
					ASSET
				};

				SelectedState currentSelectedState = DESELECTED;

			private:
				bool locked = false;
				GUIInfo* currentlyViewing;

				void showMenu();

				void displayParameters(std::vector<editor::gui::ComponentParameterRegister::ParameterData> parameters);

				unsigned int paramIndex = 0;
			};
		}
	}
}

#endif