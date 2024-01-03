#ifndef	PPMINT_EDITOR_ASSET_VIEWER_H
#define PPMINT_EDITOR_ASSET_VIEWER_H

#include <editor/windows/GUIWindow.h>
#include <imgui/imgui_internal.h>

namespace editor {
	namespace gui {
		namespace windows {
			class AssetViewer : public GUIWindow {
			public:
				AssetViewer();

				void render();

				ImVec2 iconSize = ImVec2(60.0f, 60.0f);
			private:
				ImVec2 lastSize;
				ImGuiWindow* imGuiWin = nullptr;

				unsigned int unnamedAssetIndex = 0;
			};
		}
	}
}

#endif