#include <editor/managers/GUIManager.h>

#include <editor/managers/EditorLogManager.h>
#include <editor/managers/EditorWindowManager.h>

#include <peppermint-preview/managers/EngineManager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <format>

using namespace editor::managers;
using namespace std;

vector<editor::gui::GUIWindow*> GUIManager::windows;
vector<ImFont*> GUIManager::fonts;
editor::gui::GUIInfo* GUIManager::currentlySelected;
unsigned int GUIManager::lastWindowDiscriminator = 0;
vector<peppermint::rendering::Texture*> GUIManager::icons;

peppermint::rendering::Texture* GUIManager::fallbackTexture;
peppermint::rendering::TextureSet* GUIManager::fallbackTextureSet;
peppermint::Asset* GUIManager::fallbackAsset;

void GUIManager::initialise() {
	LogManager::info("Initialising GUI");
	IMGUI_CHECKVERSION();
	LogManager::debug(format("Using ImGUI version {}", IMGUI_VERSION));

	LogManager::debug("Creating ImGUI context");
	ImGui::CreateContext();
	LogManager::debug("Setting theme");
	ImGui::StyleColorsDark();
	LogManager::debug("Initialising ImGUI for GLFW");
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::windows[0]->getGLFWwindow(), true);
	LogManager::debug("Initialising ImGUI for OpenGL 3.3");
	ImGui_ImplOpenGL3_Init("#version 330");

	LogManager::debug("Enabling docking mode");
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	LogManager::debug("Loading fonts");
	GUIManager::fonts.push_back(ImGui::GetIO().Fonts->AddFontFromFileTTF("resource/font/poppins/medium.ttf", 18));
	LogManager::debug("Loaded fonts");

	LogManager::debug("Loading icons");
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/unknown.png")));				// 0
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/camera.png")));				// 1
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/audioSource.png")));			// 2
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/audioListener.png")));		// 3
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/warpTile.png")));			// 4
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/navMap.png")));				// 5
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/playerController.png")));	// 6
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/transform.png")));			// 7
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/empty.png")));				// 8
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/renderer.png")));			// 9
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/world/spriteRenderer.png")));		// 10
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/asset/asset.png")));				// 11
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/asset/texture.png")));				// 12
	GUIManager::icons.push_back(new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/asset/textureSet.png")));			// 13
	LogManager::debug("Loaded icons");

	LogManager::debug("Creating fallbacks");
	GUIManager::fallbackAsset = new peppermint::Asset(peppermint::Asset::NONE);
	GUIManager::fallbackAsset->displayName = "generic fallback asset";

	GUIManager::fallbackTexture = new peppermint::rendering::Texture(new peppermint::Asset(peppermint::Asset::IMAGE, (char*)"resource/img/icon/generic/missing.png"));
	GUIManager::fallbackTexture->displayName = "fallback texture";
	
	GUIManager::fallbackTextureSet = new peppermint::rendering::TextureSet();
	GUIManager::fallbackTextureSet->addTexture(GUIManager::fallbackTexture);
	GUIManager::fallbackTextureSet->generateAtlas();
	GUIManager::fallbackTextureSet->displayName = "fallback texture set";
	LogManager::debug("Created fallbacks");
}

void GUIManager::destroy() {
	LogManager::debug("Deleting GUI windows");
	for (unsigned int i = 0; i < GUIManager::windows.size(); i++) {
		delete GUIManager::windows[i];
	}
	LogManager::debug("Deleted all GUI windows");
	LogManager::debug("Unloading icons");
	for (unsigned int i = 0; i < GUIManager::icons.size(); i++) {
		delete GUIManager::icons[i];
	}
	LogManager::debug("Unloaded icons");
	LogManager::debug("Terminating ImGUI");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUIManager::newFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUIManager::endFrame() {
	ImGui::EndFrame();
}

void GUIManager::render() {
	GUIManager::newFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("file")) {
			if (ImGui::MenuItem("new", "ctrl+n"));
			if (ImGui::MenuItem("open", "ctrl+o"));
			ImGui::Separator();
			if (ImGui::MenuItem("save world", "ctrl+s")) {
				LogManager::debug(format("Saving world file '{}' and asset file '{}'", peppermint::managers::EngineManager::getWM()->worldAsset->path, peppermint::managers::EngineManager::assetManager->assetFileAsset->path));

				peppermint::managers::EngineManager::getWM()->saveWorldFile();
				peppermint::managers::EngineManager::assetManager->saveAssetFile();

				LogManager::info(format("Saved world file '{}' and asset file '{}' successfully", peppermint::managers::EngineManager::getWM()->worldAsset->path, peppermint::managers::EngineManager::assetManager->assetFileAsset->path));
			}
			if (ImGui::MenuItem("save world as", "ctrl+shift+s"));
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("windows")) {
			for (unsigned int i = 0; i < GUIManager::windows.size(); i++) {
				if (ImGui::MenuItem(GUIManager::windows[i]->name.c_str(), NULL, &GUIManager::windows[i]->active));
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (unsigned int i = 0; i < GUIManager::windows.size(); i++) {
		if (GUIManager::windows[i]->active) {
			if (GUIManager::windows[i]->discriminator = 0) GUIManager::windows[i]->discriminator = ++GUIManager::lastWindowDiscriminator;
			GUIManager::windows[i]->render();
		}
	}

#ifdef SHOW_IMGUI_DEBUG_WINDOW
	ImGui::ShowDemoWindow();
#endif

	GUIManager::endFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}