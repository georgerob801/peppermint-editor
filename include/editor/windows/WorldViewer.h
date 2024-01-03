#ifndef PPMINT_EDITOR_WORLD_VIEWER_H
#define PPMINT_EDITOR_WORLD_VIEWER_H

#include <editor/windows/GUIWindow.h>

#include <peppermint-preview/classes/game/GameObject.h>
#include <peppermint-preview/classes/game/components/Camera.h>
#include <peppermint-preview/managers/RenderManager.h>
#include <imgui/imgui_internal.h>

using namespace peppermint::game;
using namespace peppermint::game::components;

namespace editor {
	namespace gui {
		namespace windows {
			class WorldViewer : public GUIWindow {
			public:
				WorldViewer();
				~WorldViewer();

				void render();

				float standardSpeed = 7.0f;
				float ctrlSpeed = 25.0f;
				float altSpeed = 3.0f;

				float lookSensitivity = 0.001f;

				ImVec2 widgetButtonSize = ImVec2(80.0f, 80.0f);
			private:
				ImVec2 lastSize;
				ImGuiWindow* imGuiWin = nullptr;

				peppermint::managers::RenderManager* rm = nullptr;

				GameObject* worldCamera = nullptr;
				Camera* camComponent = nullptr;
			};
		}
	}
}

#endif