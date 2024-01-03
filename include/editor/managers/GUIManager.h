#ifndef PPMINT_EDITOR_GUI_MANAGER_H
#define PPMINT_EDITOR_GUI_MANAGER_H

#define SHOW_IMGUI_DEBUG_WINDOW
#include <editor/windows/GUIWindow.h>
#include <editor/serialisation/GUIInfo.hpp>
#include <vector>

#include <peppermint-preview/classes/rendering/TextureSet.h>
#include <peppermint-preview/classes/rendering/Texture.h>

using namespace std;

namespace editor {
	namespace managers {
		class GUIManager {
		public:
			static vector<editor::gui::GUIWindow*> windows;

			static vector<ImFont*> fonts;

			static editor::gui::GUIInfo* currentlySelected;

			static void initialise();
			static void destroy();

			static void newFrame();
			static void endFrame();

			static void render();

			static vector<peppermint::rendering::Texture*> icons;

			static peppermint::Asset* fallbackAsset;
			static peppermint::rendering::Texture* fallbackTexture;
			static peppermint::rendering::TextureSet* fallbackTextureSet;
		private:
			static unsigned int lastWindowDiscriminator;
		};
	}
}

#endif