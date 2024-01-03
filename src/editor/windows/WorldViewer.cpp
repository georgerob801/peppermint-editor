#include <editor/windows/WorldViewer.h>

#include <peppermint-preview/managers/EngineManager.h>
#include <editor/managers/EditorManager.h>
#include <editor/managers/EditorWindowManager.h>

#include <editor/managers/GUIManager.h>

using namespace editor::gui::windows;

typedef peppermint::managers::EngineManager EngineManager;

ImVec2 lastCursorPos;
unsigned int lastCursorPosFrame = 0;

unsigned int buttonIndex = 0;

WorldViewer::WorldViewer() {
	this->name = "world";
}

WorldViewer::~WorldViewer() {
	if (this->rm != nullptr) delete this->rm;
}

void WorldViewer::render() {
	if (EngineManager::worldManagers.size() == 0) {
		const char* message = "* no world loaded *";

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - ImGui::CalcTextSize(message).x) / 2.0f, (this->lastSize.y - ImGui::CalcTextSize(message).y) / 2.0f));

		ImGui::Begin(this->name.c_str(), &this->active);

		ImGui::Text(message);

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) {
			this->imGuiWin = ImGui::FindWindowByName(this->name.c_str());
		} else {
			this->lastSize = this->imGuiWin->InnerRect.GetSize();
		}
		return;
	} else {
		buttonIndex = 0;
		// wahey time to attempt to render something from an added camera
		if (this->rm == nullptr) {
			this->rm = new peppermint::managers::RenderManager();

			this->worldCamera = new GameObject();
			this->camComponent = new Camera();
			this->worldCamera->addComponent(this->camComponent);
			this->rm->activeRenderQueue = EngineManager::windowManager->windows[0]->renderManager->activeRenderQueue;
			this->rm->activeCamera = this->camComponent;

			this->rm->setupFBO();

			this->rm->backgroundColour = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		}


		if (GameObject* selectedGo = dynamic_cast<GameObject*>(editor::managers::GUIManager::currentlySelected)) {
			this->rm->toHighlight = selectedGo;
		} else {
			this->rm->toHighlight = nullptr;
		}

		EngineManager::getWM()->populateRenderQueue(EngineManager::windowManager->windows[0]);
		peppermint::Window* w = EngineManager::windowManager->windows[0];

		this->rm->bindFBO();
		w->width = this->lastSize.x;
		w->height = this->lastSize.y;
		this->rm->resizeFBOAttachments();

		int dims[2];
		w->getIntendedViewportSize(dims);
		
		this->widgetButtonSize.x = dims[1] / 10.0f;
		this->widgetButtonSize.y = dims[1] / 10.0f;

		mat4 proj = perspective(glm::quarter_pi<float>(), 16.0f / 9.0f, 0.1f, 100.0f);
		this->rm->renderFrame(true, proj);
		peppermint::managers::RenderManager::unbindFBOs();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - dims[0] - 2) / 2, (this->lastSize.y - dims[1] - 2) / 2));
		ImGui::Begin(this->name.c_str(), &this->active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (this->imGuiWin == nullptr) {
			this->lastSize = ImGui::GetContentRegionMax();
		} else {
			this->lastSize = this->imGuiWin->InnerRect.GetSize();
		}

		ImVec2 pos = ImGui::GetCursorPos();

		ImGui::Image((ImTextureID)this->rm->getColourTex(), ImVec2(dims[0], dims[1]), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImGui::GetStyleColorVec4(ImGuiCol_Border));

		// now attempt to draw icons (death)
		for (unsigned int i = 0; i < EngineManager::getWM()->gameObjects.size(); i++) {

			// empty cases
			if (EngineManager::getWM()->gameObjects[i]->components.size() <= 1) {
				// wahey find location
				vec4 clipPos = proj * this->camComponent->getViewMatrix() * EngineManager::getWM()->gameObjects[i]->transform->getMatrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f); // vec4(EngineManager::getWM()->gameObjects[i]->transform->position, 1.0f);

				if (abs(clipPos.x) > clipPos.w || abs(clipPos.y) > clipPos.w || abs(clipPos.z) > clipPos.w) break;

				// ok it's actually on the screen
				vec3 ndcPosition = vec3(clipPos) / clipPos.w;

				ImGui::SetCursorPos(ImVec2(pos.x + (dims[0] * ((1 + ndcPosition.x) / 2.0f) - (this->widgetButtonSize.x / 2)), (pos.y + dims[1]) - (dims[1] * ((1 + ndcPosition.y) / 2.0f)) - (this->widgetButtonSize.y / 2)));
				// ImGui::Text("camera!");

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

				ImGui::PushID(std::format("worldViewerSelector##{}", buttonIndex).c_str());
				if (ImGui::ImageButton("", (ImTextureID)editor::managers::GUIManager::icons[8]->getGLTexLocation(), this->widgetButtonSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), editor::managers::GUIManager::currentlySelected == EngineManager::getWM()->gameObjects[i] ? ImVec4(this->rm->selectionTintColour.r, this->rm->selectionTintColour.g, this->rm->selectionTintColour.b, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 0.5f)));

				// using the if statement didn't work for some reason, no idea why
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					editor::managers::GUIManager::currentlySelected = EngineManager::getWM()->gameObjects[i];
				}

				ImGui::PopID();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
				// and go to next object
				continue;
			}

			for (unsigned int j = 0; j < EngineManager::getWM()->gameObjects[i]->components.size(); j++) {
				peppermint::game::Component* co = EngineManager::getWM()->gameObjects[i]->components[j];
				switch ((peppermint::game::Component::ComponentType)co->getType()) {
				case peppermint::game::Component::SOUND_SOURCE:
				case peppermint::game::Component::SOUND_LISTENER:
				case peppermint::game::Component::CAMERA:
				case peppermint::game::Component::WARP_TILE:
				case peppermint::game::Component::NAVIGABLE_MAP:
				case peppermint::game::Component::PLAYER_CONTROLLER:
				{
					// wahey find location
					vec4 clipPos = proj * this->camComponent->getViewMatrix() * EngineManager::getWM()->gameObjects[i]->transform->getMatrix() * vec4(0.0f, 0.0f, 0.0f, 1.0f); // vec4(EngineManager::getWM()->gameObjects[i]->transform->position, 1.0f);

					if (clipPos.x > clipPos.w || clipPos.y > clipPos.w || clipPos.z > clipPos.w) break;

					// ok it's actually on the screen
					vec3 ndcPosition = vec3(clipPos) / clipPos.w;

					ImGui::SetCursorPos(ImVec2(pos.x + (dims[0] * ((1 + ndcPosition.x) / 2.0f) - (this->widgetButtonSize.x / 2)), (pos.y + dims[1]) - (dims[1] * ((1 + ndcPosition.y) / 2.0f)) - (this->widgetButtonSize.y / 2)));
					// ImGui::Text("camera!");
					
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					unsigned int iconIndex = 0;

					switch ((peppermint::game::Component::ComponentType)co->getType()) {
					case peppermint::game::Component::CAMERA:
						iconIndex = 1;
						break;
					case peppermint::game::Component::SOUND_SOURCE:
						iconIndex = 2;
						break;
					case peppermint::game::Component::SOUND_LISTENER:
						iconIndex = 3;
						break;
					case peppermint::game::Component::WARP_TILE:
						iconIndex = 4;
						break;
					case peppermint::game::Component::NAVIGABLE_MAP:
						iconIndex = 5;
						break;
					case peppermint::game::Component::PLAYER_CONTROLLER:
						iconIndex = 6;
						break;
					}

					ImGui::PushID(std::format("worldViewerSelector##{}", buttonIndex).c_str());
					if (ImGui::ImageButton("", (ImTextureID)editor::managers::GUIManager::icons[iconIndex]->getGLTexLocation(), this->widgetButtonSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), editor::managers::GUIManager::currentlySelected == EngineManager::getWM()->gameObjects[i] ? ImVec4(this->rm->selectionTintColour.r, this->rm->selectionTintColour.g, this->rm->selectionTintColour.b, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 0.5f)));
					
					// using the if statement didn't work for some reason, no idea why
					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
						editor::managers::GUIManager::currentlySelected = EngineManager::getWM()->gameObjects[i];
					}

					ImGui::PopID();
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();

					break;
				}
				default:
					break;
				}
			}
		}


		// wahey it's moving time
		if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered()) {
			// look
			if (ImGui::IsKeyDown(ImGuiKey_MouseRight)) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_None);
				if (ImGui::GetFrameCount() - 1 != lastCursorPosFrame) {
					// new movement
					ImVec2 cursorCentre(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2));
					glfwSetCursorPos(editor::managers::WindowManager::windows[0]->getGLFWwindow(), cursorCentre.x, cursorCentre.y);
					lastCursorPos = cursorCentre;
					lastCursorPosFrame = ImGui::GetFrameCount();
				} else {
					// continue movement
					lastCursorPosFrame = ImGui::GetFrameCount();
					double pos[2];
					glfwGetCursorPos(editor::managers::WindowManager::windows[0]->getGLFWwindow(), &pos[0], &pos[1]);
					ImVec2 delta(pos[0] - lastCursorPos.x, pos[1] - lastCursorPos.y);
					delta.x = round(delta.x);
					delta.y = round(delta.y);
					ImVec2 cursorCentre(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2));
					glfwSetCursorPos(editor::managers::WindowManager::windows[0]->getGLFWwindow(), cursorCentre.x, cursorCentre.y);
					glfwGetCursorPos(editor::managers::WindowManager::windows[0]->getGLFWwindow(), &pos[0], &pos[1]);
					lastCursorPos = ImVec2(pos[0], pos[1]);

					// time to actually move
					delta.x *= this->lookSensitivity;
					delta.y *= this->lookSensitivity;


					this->worldCamera->transform->rotation.x -= delta.y;
					if (this->worldCamera->transform->rotation.x > half_pi<float>()) this->worldCamera->transform->rotation.x = half_pi<float>() - 0.01f;
					if (this->worldCamera->transform->rotation.x < -half_pi<float>()) this->worldCamera->transform->rotation.x = -half_pi<float>() + 0.01f;

					this->worldCamera->transform->rotation.y += delta.x;

					while (this->worldCamera->transform->rotation.y >= two_pi<float>()) this->worldCamera->transform->rotation.y -= two_pi<float>();
					while (this->worldCamera->transform->rotation.y < 0.0f) this->worldCamera->transform->rotation.y += two_pi<float>();
				}

				// move
				vec3 direction(0.0f, 0.0f, 0.0f);

				float speed = this->standardSpeed;
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) speed = this->ctrlSpeed;
				else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) speed = this->altSpeed;

				if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow)) {
					// move forwards
					direction += this->camComponent->front;
				}
				if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
					// move left
					direction -= this->camComponent->right;
				}
				if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow)) {
					// move back
					direction -= this->camComponent->front;
				}
				if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
					// move right
					direction += this->camComponent->right;
				}
				if (ImGui::IsKeyDown(ImGuiKey_E) || ImGui::IsKeyDown(ImGuiKey_Space)) {
					// move up
					direction += vec3(0.0f, 1.0f, 0.0f);
				}
				if (ImGui::IsKeyDown(ImGuiKey_Q) || ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
					// move down
					direction -= vec3(0.0f, 1.0f, 0.0f);
				}

				if (length(direction) != 0) {
					direction = normalize(direction);
					direction *= editor::managers::EditorManager::deltaTime * speed;
					this->worldCamera->transform->position += direction;
				}
			}
		} else {
			if (ImGui::IsKeyDown(ImGuiKey_MouseRight) && ImGui::IsWindowHovered()) {
				ImGui::SetWindowFocus(this->name.c_str());
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) this->imGuiWin = ImGui::FindWindowByName(this->name.c_str());
	}
}