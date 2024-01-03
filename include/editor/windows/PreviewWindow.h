#ifndef PPMINT_EDITOR_PREVIEW_WINDOW_H
#define PPMINT_EDITOR_PREVIEW_WINDOW_H

#include <editor/windows/GUIWindow.h>

#include <imgui/imgui_internal.h>

namespace editor {
	namespace gui {
		namespace windows {
			class PreviewWindow : public GUIWindow {
			public:
				void render();

			private:
				ImVec2 lastSize;
				ImGuiWindow* imGuiWin = nullptr;
			};
		}
	}
}

#endif