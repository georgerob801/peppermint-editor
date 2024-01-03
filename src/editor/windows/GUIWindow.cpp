#include <editor/windows/GUIWindow.h>

using namespace editor::gui;

#include <format>

void GUIWindow::render() {
	ImGui::Begin("default window");
	ImGui::TextWrapped("This is a window that shouldn't really be here.");
	ImGui::TextWrapped("This window is likely part of a feature that's yet to be implemented. Maybe check back here later?");

	ImGui::End();
}

std::string GUIWindow::getID() {
	return std::format("{}##{}", this->name, this->discriminator);
}