#include <editor/windows/PreviewWindow.h>

#include <peppermint-preview/managers/EngineManager.h>

using namespace editor::gui::windows;

void PreviewWindow::render() {
	if (peppermint::managers::EngineManager::worldManagers.size() == 0) {
		const char* message = "* no world loaded *";

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - ImGui::CalcTextSize(message).x) / 2.0f, (this->lastSize.y - ImGui::CalcTextSize(message).y) / 2.0f));

		ImGui::Begin("preview", &this->active);

		ImGui::Text(message);

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) {
			this->imGuiWin = ImGui::FindWindowByName("preview");
		} else {
			this->lastSize = this->imGuiWin->InnerRect.GetSize();
		}
		return;
	} else if (peppermint::managers::EngineManager::getWM()->getFirstCamera() == nullptr) {
		const char* message = "* world has no camera *";

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - ImGui::CalcTextSize(message).x) / 2.0f, (this->lastSize.y - ImGui::CalcTextSize(message).y) / 2.0f));

		ImGui::Begin("preview", &this->active);

		ImGui::Text(message);

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) {
			this->imGuiWin = ImGui::FindWindowByName("preview");
		} else {
			this->lastSize = this->imGuiWin->InnerRect.GetSize();
		}
		return;
	} else {
		// wahey time to render a preview from the actual game camera

		peppermint::managers::EngineManager::getWM()->populateRenderQueue(peppermint::managers::EngineManager::windowManager->windows[0]);
		peppermint::Window* w = peppermint::managers::EngineManager::windowManager->windows[0];
		w->renderManager->bindFBO();
		w->width = this->lastSize.x;
		w->height = this->lastSize.y;
		w->renderManager->resizeFBOAttachments();
		int dims[2];
		w->getIntendedViewportSize(dims);

		w->renderFrame();
		peppermint::managers::RenderManager::unbindFBOs();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((this->lastSize.x - dims[0] - 2) / 2, (this->lastSize.y - dims[1] - 2) / 2));
		ImGui::Begin("preview", &this->active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (this->imGuiWin == nullptr) {
			this->lastSize = ImGui::GetContentRegionMax();
		} else {
			this->lastSize = this->imGuiWin->InnerRect.GetSize();
		}

		ImGui::Image((ImTextureID)w->renderManager->getColourTex(), ImVec2(dims[0], dims[1]), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImGui::GetStyleColorVec4(ImGuiCol_Border));

		ImGui::End();
		ImGui::PopStyleVar();

		if (this->imGuiWin == nullptr) this->imGuiWin = ImGui::FindWindowByName("preview");
	}
}