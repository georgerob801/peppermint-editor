#include <editor/windows/HierarchyViewer.h>

#include <peppermint-preview/managers/EngineManager.h>
#include <editor/managers/GUIManager.h>

#include <algorithm>

using namespace editor::gui::windows;
using namespace editor::managers;

typedef peppermint::game::GameObject GameObject;

unsigned int unnamedIndex = 0;

HierarchyViewer::HierarchyViewer() {
	this->name = "hierarchy";
}

void drawTree(GameObject* go, vector<GameObject*>* remaining) {
	if (go->displayName.length() == 0) go->displayName = format("Game Object {}", unnamedIndex++);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (GUIManager::currentlySelected == go) flags |= ImGuiTreeNodeFlags_Selected ;

	bool open = ImGui::TreeNodeEx((void*)go, flags, go->getDisplayName().c_str());
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) GUIManager::currentlySelected = (editor::gui::GUIInfo*)go;
	
	// drag and drop stuff
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("gameObject", go, sizeof(*go));
		ImGui::Text(go->getDisplayName().c_str());
		ImGui::EndDragDropSource();
	}

	if (open) {
		vector<GameObject*>::iterator pos = (*remaining).begin();
		while (pos != (*remaining).end()) {
			if ((*pos)->transform->parent == go->transform) {
				GameObject* val = *pos;
				drawTree(val, remaining);
			}
			pos++;
		}

		ImGui::TreePop();
	}
}

void HierarchyViewer::render() {
	ImGui::Begin(this->name.c_str(), &this->active);

	if (peppermint::managers::EngineManager::worldManagers.size() == 0) {
		ImGui::TextWrapped("* no world loaded *");
		ImGui::End();
		return;
	}

	peppermint::managers::WorldManager* wm = peppermint::managers::EngineManager::getWM();

	// ImGui::TextWrapped("GameObject count: %d", wm->gameObjects.size());

	// time to find top level objects
	if (wm->gameObjects.size() != 0) {
		vector<GameObject*> toSearch(wm->gameObjects);

		vector<GameObject*> topLevels;
		for (unsigned int i = 0; i < toSearch.size(); i++) {
			if (toSearch[i]->transform->parent == nullptr) {
				topLevels.push_back(toSearch[i]);
			}
		}

		for (unsigned int i = 0; i < topLevels.size(); i++) {
			drawTree(topLevels[i], &wm->gameObjects);
		}
	} else {
		// todo: centre this
		ImGui::TextWrapped("* no game objects in world *");
	}

	ImGui::End();
}