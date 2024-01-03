#include <iostream>

#include <editor/editor.h>

#include <peppermint-preview/peppermint.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stb_image/stb_image.h>


typedef editor::managers::EditorManager EditorManager;
typedef editor::managers::WindowManager WindowManager;

typedef editor::managers::LogManager LogManager;


// testing stuff goes below here
#include <editor/windows/HierarchyViewer.h>
#include <editor/windows/PreviewWindow.h>
#include <editor/windows/Inspector.h>
#include <editor/windows/WorldViewer.h>
#include <editor/windows/AssetViewer.h>
#include <peppermint-preview/classes/game/components/renderers/SpriteRenderer.h>


#include <format>

int main() {
	// try {
		EditorManager::initialise();
	// } catch (std::exception e) {
// 		LogManager::critical(e.what());
	// 	return -1;
	// }


	//float backColour[4] = {
	//	0.0f,
	//	0.0f,
	//	0.0f,
	//	1.0f
	//};

	peppermint::Asset* gameFileAsset = new peppermint::Asset(peppermint::Asset::PPMINT_GAME_FILE, (char*)"peppermint/pokemon.pmint");
	peppermint::managers::EngineManager::setGameFile(gameFileAsset);

	peppermint::managers::EngineManager::loadFromGameFile();

	// peppermint::managers::EngineManager::getWM()->createGameObject();

	// peppermint::managers::EngineManager::goToWorld(0);


	// visualise camera for testing
	//peppermint::game::components::SpriteRenderer* sr = new peppermint::game::components::SpriteRenderer();
	//peppermint::Asset* a = new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/camera.png");
	//peppermint::rendering::Texture* te = new peppermint::rendering::Texture(a);
	//peppermint::rendering::TextureSet* ts = new peppermint::rendering::TextureSet();
	//ts->addTexture(te);
	//sr->textureSet = ts;

	//((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->addComponent(sr);
	//sr->generateTextures();

	//sr->size = vec2(ts->atlas->getWidth(), ts->atlas->getHeight());
	//sr->position = vec2(64, 64);

	//sr->generateVertices();


	//float fl3[3] = { 
	//	((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.x, 
	//	((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.y, 
	//	((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.z
	//};

	//peppermint::managers::EngineManager::windowManager->windows[0]->renderManager->bindFBO();
	//int dims[2];
	//peppermint::managers::EngineManager::windowManager->windows[0]->getSize(dims);
	//glViewport(0, 0, dims[0], dims[1]);
	//peppermint::managers::RenderManager::unbindFBOs();

	//ImVec2 lastSize = ImVec2(0, 0);

	//double lastTime = glfwGetTime();
	//double fps = 60.0f;

	//ImGuiWindow* test = nullptr;

	/*
	while (WindowManager::windows.size() != 0) {
		for (unsigned int i = 0; i < WindowManager::windows.size(); i++) {
			WindowManager::setCurrentWindow(WindowManager::windows[0]);
			
			// game frame rendering happens here

			peppermint::managers::EngineManager::getWM()->populateRenderQueue(peppermint::managers::EngineManager::windowManager->windows[0]);
			peppermint::managers::EngineManager::windowManager->windows[0]->renderManager->bindFBO();
			peppermint::managers::EngineManager::windowManager->windows[0]->width = lastSize.x;
			peppermint::managers::EngineManager::windowManager->windows[0]->height = lastSize.y;
			peppermint::managers::EngineManager::windowManager->windows[0]->renderManager->resizeFBOAttachments();
			peppermint::managers::EngineManager::windowManager->windows[0]->getIntendedViewportSize(dims);
			glViewport(0, 0, dims[0], dims[1]);
			peppermint::managers::RenderManager::unbindFBOs();

			peppermint::managers::EngineManager::windowManager->windows[0]->renderFrame();

			peppermint::managers::RenderManager::unbindFBOs();


			// ui frame rendering happens here

			glClearColor(backColour[0], backColour[1], backColour[2], backColour[3]);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			


			ImGui::Begin("wahey this is a frame");

			ImGui::Text(format("X: {}, Y: {}", ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y).c_str());

			ImGui::Text("ooh look at this");
			ImGui::ColorEdit4("background colour", backColour);
			ImGui::DragFloat3("position test", fl3, 0.1f, -10.0f, 10.0f);

			((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.x = fl3[0];
			((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.y = fl3[1];
			((peppermint::game::GameObject*)peppermint::managers::EngineManager::getWM()->getFirstCamera()->getGameObject())->transform->position.z = fl3[2];


			ImGui::End();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(((float)(lastSize.x - dims[0] - 2)) / 2.0f, ((float)(lastSize.y - dims[1] - 2)) / 2.0f));
			
			// ImGui::Begin("Preview", false, );

			ImGui::Begin("Preview", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			
			ImVec2 vMax;
			if (test == nullptr) {
				vMax = ImGui::GetContentRegionMax();
			} else {
				vMax = test->InnerRect.GetSize();
			}

			lastSize = vMax;

			ImVec2 imgSize(dims[0], dims[1]);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

			ImGui::GetItemRectSize();
			
			ImGui::Image((ImTextureID)peppermint::managers::EngineManager::windowManager->windows[0]->renderManager->getColourTex(), imgSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImGui::GetStyleColorVec4(ImGuiCol_Border));
			ImGui::PopStyleVar();

			ImGui::End();
			ImGui::PopStyleVar();

			if (test == nullptr) test = ImGui::FindWindowByName("Preview");

			// ImGui::ShowFontSelector("eeee");
			// ImGui::ShowStyleEditor();

			ImGui::ShowDemoWindow();

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			WindowManager::windows[0]->swapBuffers();


			if (WindowManager::getWindow()->shouldClose()) {
				editor::Window* winToDelete = WindowManager::windows[i];
				WindowManager::windows.erase(WindowManager::windows.begin() + i);
				delete winToDelete;
			}
		}

		glfwPollEvents();

		while (glfwGetTime() - lastTime < 1 / fps) { }
		lastTime = glfwGetTime();
	}

	*/

	// peppermint::managers::EngineManager::createWorldManager();
	
	editor::gui::windows::HierarchyViewer* test = new editor::gui::windows::HierarchyViewer();
	editor::managers::GUIManager::windows.push_back(test);
	editor::gui::windows::PreviewWindow* e = new editor::gui::windows::PreviewWindow();
	editor::managers::GUIManager::windows.push_back(e);
	editor::gui::windows::Inspector* s = new editor::gui::windows::Inspector();
	editor::managers::GUIManager::windows.push_back(s);
	editor::gui::windows::WorldViewer* wv = new editor::gui::windows::WorldViewer();
	editor::managers::GUIManager::windows.push_back(wv);
	editor::gui::windows::AssetViewer* av = new editor::gui::windows::AssetViewer();
	editor::managers::GUIManager::windows.push_back(av);

	//try {
		EditorManager::loop();
	//} catch (std::exception e) {
	//	LogManager::critical(e.what());
	//	EditorManager::destroy();
	//	return -1;
	//}

	EditorManager::destroy();

	return 0;
}