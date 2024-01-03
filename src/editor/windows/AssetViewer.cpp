#include <editor/windows/AssetViewer.h>

#include <peppermint-preview/managers/EngineManager.h>
#include <editor/managers/GUIManager.h>

using namespace editor::gui::windows;
using namespace peppermint::managers;

typedef editor::managers::GUIManager GUIManager;

AssetViewer::AssetViewer() {
	this->name = "assets";
}

void AssetViewer::render() {
	if (EngineManager::worldManagers.size() == 0) {
		const char* message = "* no game loaded *";
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - ImGui::CalcTextSize(message).x) / 2.0f, (this->lastSize.y - ImGui::CalcTextSize(message).y) / 2.0f));

		ImGui::Begin(this->getID().c_str(), &this->active);

		ImGui::Text(message);

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) this->imGuiWin = ImGui::FindWindowByName(this->getID().c_str());
		this->lastSize = this->imGuiWin->InnerRect.GetSize();

		return;
	} else {
		this->unnamedAssetIndex = 0;
		// show assets
		ImGui::Begin(this->getID().c_str(), &this->active);

		unsigned int numCols = 5;

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 5.0f));
		if (ImGui::BeginTable(format("assets-{}", this->discriminator).c_str(), numCols)) {
			for (unsigned int row = 0; row * numCols < EngineManager::assetManager->assets.size(); row++) {
				ImGui::TableNextRow();
				for (unsigned int col = 0; col < numCols && (row * numCols) + col < EngineManager::assetManager->assets.size(); col++) {
					ImGui::TableSetColumnIndex(col);
					if (EngineManager::assetManager->assets[(row * numCols) + col]->displayName.length() == 0) EngineManager::assetManager->assets[(row * numCols) + col]->displayName = format("unnamed asset {}", this->unnamedAssetIndex++);
					//ImGui::Text(EngineManager::assetManager->assets[(row * numCols) + col]->getDisplayName().c_str());

					unsigned int iconIndex = 0;
					switch (EngineManager::assetManager->assets[(row * numCols) + col]->type) {
					case peppermint::Asset::TEXTURE:
						iconIndex = 12;
						break;
					case peppermint::Asset::TEXTURE_SET:
						iconIndex = 13;
						break;
					default:
						iconIndex = EngineManager::assetManager->assets[(row * numCols) + col]->iconIndex;
						break;
					}
					

					ImGui::ImageButton((ImTextureID)GUIManager::icons[iconIndex]->getGLTexLocation(), this->iconSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					
					if (ImGui::BeginDragDropSource()) {
						ImGui::SetDragDropPayload("asset", EngineManager::assetManager->assets[(row * numCols) + col], sizeof(*EngineManager::assetManager->assets[(row * numCols) + col]));
						ImGui::Text(EngineManager::assetManager->assets[(row * numCols) + col]->getDisplayName().c_str());
						ImGui::EndDragDropSource();
					}
					
					bool result = ImGui::IsItemClicked();
					result = result || ImGui::Selectable(EngineManager::assetManager->assets[(row * numCols) + col]->getDisplayName().c_str());

					if (ImGui::BeginDragDropSource()) {
						ImGui::SetDragDropPayload("asset", EngineManager::assetManager->assets[(row * numCols) + col], sizeof(*EngineManager::assetManager->assets[(row * numCols) + col]));
						ImGui::Text(EngineManager::assetManager->assets[(row * numCols) + col]->getDisplayName().c_str());
						ImGui::EndDragDropSource();
					}

					if (result) {
						GUIManager::currentlySelected = EngineManager::assetManager->assets[(row * numCols) + col];
					}
				}
			}

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();

		ImGui::End();
	}
}