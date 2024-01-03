#include <editor/windows/Inspector.h>

#include <peppermint-preview/peppermint.h>
#include <editor/managers/GUIManager.h>
#include <peppermint-preview/managers/EngineManager.h>
#include <peppermint-preview/classes/Asset.h>

#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

using namespace editor::gui::windows;
using namespace editor::managers;

typedef peppermint::game::GameObject GameObject;
typedef peppermint::managers::EngineManager EngineManager;

ImVec2 lastSize;
ImGuiWindow* imGuiWin = nullptr;

Inspector::Inspector() {
	this->name = "inspector";
}

void Inspector::showMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("options")) {
			ImGui::MenuItem("locked", "", &this->locked);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void Inspector::displayParameters(vector<editor::gui::ComponentParameterRegister::ParameterData> parameters) {
	if (parameters.size() != 0 && parameters[0].separator == false) ImGui::SeparatorText("parameters");

	for (unsigned int i = 0; i < parameters.size(); i++) {
		editor::gui::ComponentParameterRegister::ParameterData pd = parameters[i];
		if (pd.separator) {
			if (pd.separatorText.length() == 0) ImGui::Separator();
			else ImGui::SeparatorText(pd.separatorText.c_str());
		}

		std::string paramName = std::format("{}##{}", pd.name.c_str(), this->paramIndex++);

		if (pd.location == nullptr) {
			ImGui::BeginDisabled();
			ImGui::Text(format("parameter provided with invalid pointer: '{}'", pd.name).c_str());
			ImGui::EndDisabled();
			continue;
		}

		if (pd.disabled) ImGui::BeginDisabled();

		switch (pd.type) {
		case ComponentParameterRegister::INT:
		{
			int* val = (int*)pd.location;

			int in = *val;

			if (pd.useMapping) in = (int)pd.unmap((float)*val);

			int inMin = pd.clamp ? pd.mins[0] : -INT_MAX;
			int inMax = pd.clamp ? pd.maxes[0] : INT_MAX;
			if (inMin != -INT_MAX && inMax != INT_MAX) {
				ImGui::SliderInt(paramName.c_str(), &in, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			} else {
				int step = (int)fmin((inMax - inMin) / 100, 1);
				ImGui::DragInt(paramName.c_str(), &in, step, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				(*val) = (int)pd.map((float)in);
			} else {
				(*val) = in;
			}

			break;
		}
		case ComponentParameterRegister::FLOAT:
		{
			float* val = (float*)pd.location;

			float in = *val;

			if (pd.useMapping) in = pd.unmap(*val);

			float inMin = pd.clamp ? pd.mins[0] : -FLT_MAX;
			float inMax = pd.clamp ? pd.maxes[0] : FLT_MAX;

			if (inMin != -FLT_MAX && inMax != FLT_MAX) {
				ImGui::SliderFloat(paramName.c_str(), &in, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			} else {
				float step = fmin((inMax - inMin) / 100.0f, 0.5f);
				ImGui::DragFloat(paramName.c_str(), &in, step, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				(*val) = pd.map(in);
			} else {
				(*val) = in;
			}

			break;
		}
		case ComponentParameterRegister::INTV2:
		{
			ivec2* val = (ivec2*)pd.location;

			int in[2] = {
				val->x,
				val->y
			};

			if (pd.useMapping) {
				in[0] = (int)pd.unmap((float)(*val).x);
				in[1] = (int)pd.unmap((float)(*val).y);
			}

			int inMin = pd.clamp ? pd.mins[0] : -INT_MAX;
			int inMax = pd.clamp ? pd.maxes[0] : INT_MAX;

			if (inMin != -INT_MAX && inMax != INT_MAX) {
				ImGui::SliderInt2(paramName.c_str(), in, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			} else {
				int step = (int)fmin((inMax - inMin) / 100, 1);
				ImGui::DragInt2(paramName.c_str(), in, step, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				(*val).x = (int)pd.map((float)in[0]);
				(*val).y = (int)pd.map((float)in[1]);
			} else {
				(*val).x = in[0];
				(*val).y = in[1];
			}

			break;
		}
		case ComponentParameterRegister::FLOATV2:
		{
			vec3* val = (vec3*)pd.location;

			float in[2] = {
				val->x,
				val->y
			};

			if (pd.useMapping) {
				in[0] = pd.unmap(val->x);
				in[1] = pd.unmap(val->y);
			}

			float inMin = pd.clamp ? pd.mins[0] : -FLT_MAX;
			float inMax = pd.clamp ? pd.maxes[0] : FLT_MAX;

			if (inMin != -FLT_MAX && inMax != FLT_MAX) {
				ImGui::SliderFloat2(paramName.c_str(), in, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			} else {
				float step = fmin((inMax - inMin) / 100.0f, 0.5f);
				ImGui::DragFloat2(paramName.c_str(), in, step, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				val->x = pd.map(in[0]);
				val->y = pd.map(in[1]);
			} else {
				val->x = in[0];
				val->y = in[1];
			}

			break;
		}
		case ComponentParameterRegister::INTV3:
		{
			ivec3* val = (ivec3*)pd.location;

			int in[3] = {
				val->x,
				val->y,
				val->z
			};

			if (pd.useMapping) {
				in[0] = (int)pd.unmap((float)val->x);
				in[1] = (int)pd.unmap((float)val->y);
				in[2] = (int)pd.unmap((float)val->z);
			}

			int inMin = pd.clamp ? pd.mins[0] : -INT_MAX;
			int inMax = pd.clamp ? pd.maxes[0] : INT_MAX;

			if (inMin != -INT_MAX && inMax != INT_MAX) {
				ImGui::SliderInt3(paramName.c_str(), in, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			} else {
				int step = (int)fmin((inMax - inMin) / 100, 1);
				ImGui::DragInt3(paramName.c_str(), in, step, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);;
			}

			if (pd.useMapping) {
				val->x = (int)pd.map((float)in[0]);
				val->y = (int)pd.map((float)in[1]);
				val->z = (int)pd.map((float)in[2]);
			} else {
				val->x = in[0];
				val->y = in[1];
				val->z = in[2];
			}

			break;
		}
		case ComponentParameterRegister::FLOATV3:
		{
			vec3* val = (vec3*)pd.location;
			float in[3] = {
				val->x,
				val->y,
				val->z
			};

			if (pd.useMapping) {
				in[0] = pd.unmap(val->x);
				in[1] = pd.unmap(val->y);
				in[2] = pd.unmap(val->z);
			}

			float inMin = pd.clamp ? pd.mins[0] : -FLT_MAX;
			float inMax = pd.clamp ? pd.maxes[0] : FLT_MAX;

			float step = fmin((inMax - inMin) / 100.0f, 0.5f);

			ImGui::DragFloat3(paramName.c_str(), in, 0.5f, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);

			if (!pd.useMapping) {
				val->x = in[0];
				val->y = in[1];
				val->z = in[2];
			} else {
				val->x = pd.map(in[0]);
				val->y = pd.map(in[1]);
				val->z = pd.map(in[2]);
			}

			break;
		}
		case ComponentParameterRegister::INTV4:
		{
			ivec4* val = (ivec4*)pd.location;

			int in[4] = {
				val->x,
				val->y,
				val->z,
				val->w
			};

			if (pd.useMapping) {
				in[0] = (int)pd.unmap((float)val->x);
				in[1] = (int)pd.unmap((float)val->y);
				in[2] = (int)pd.unmap((float)val->z);
				in[3] = (int)pd.unmap((float)val->w);
			}


			int inMin = pd.clamp ? pd.mins[0] : -INT_MAX;
			int inMax = pd.clamp ? pd.maxes[0] : INT_MAX;

			if (inMin != -INT_MAX && inMax != INT_MAX) {
				ImGui::SliderInt4(paramName.c_str(), in, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			} else {
				int step = (int)fmin((inMax - inMin) / 100, 1);
				ImGui::DragInt4(paramName.c_str(), in, step, inMin, inMax, "%d", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				val->x = (int)pd.map((float)in[0]);
				val->y = (int)pd.map((float)in[1]);
				val->z = (int)pd.map((float)in[2]);
				val->w = (int)pd.map((float)in[3]);
			} else {
				val->x = in[0];
				val->y = in[1];
				val->z = in[2];
				val->w = in[3];
			}

			break;
		}
		case ComponentParameterRegister::FLOATV4:
		{
			vec4* val = (vec4*)pd.location;

			float in[4] = {
				val->x,
				val->y,
				val->z,
				val->w
			};

			if (pd.useMapping) {
				in[0] = pd.unmap(val->x);
				in[1] = pd.unmap(val->y);
				in[2] = pd.unmap(val->z);
				in[3] = pd.unmap(val->w);
			}

			float inMin = pd.clamp ? pd.mins[0] : -FLT_MAX;
			float inMax = pd.clamp ? pd.maxes[0] : FLT_MAX;

			if (inMin != -FLT_MAX && inMax != FLT_MAX) {
				ImGui::SliderFloat4(paramName.c_str(), in, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			} else {
				float step = fmin((inMax - inMin) / 100.0f, 0.5f);
				ImGui::DragFloat4(paramName.c_str(), in, step, inMin, inMax, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				val->x = pd.map(in[0]);
				val->y = pd.map(in[1]);
				val->z = pd.map(in[2]);
				val->w = pd.map(in[3]);
			} else {
				val->x = in[0];
				val->y = in[1];
				val->z = in[2];
				val->w = in[3];
			}

			break;
		}
		case ComponentParameterRegister::BOOL:
		{
			bool* val = (bool*)pd.location;

			bool in = *val;

			ImGui::Checkbox(paramName.c_str(), &in);

			(*val) = in;

			break;
		}
		case ComponentParameterRegister::VOID_P:
		{
			void* val = pd.location;

			std::string display = "error?";
			if (val == nullptr) {
				display = "none";
			} else display = std::format("{}", val);
			ImGui::InputText(paramName.c_str(), &display, ImGuiInputTextFlags_ReadOnly);

			break;
		}
		case ComponentParameterRegister::COMPONENT:
		{
			Component** val = (Component**)pd.location;

			std::string display = "error?";
			if (val == nullptr) {
				display = "none";
			} else display = std::format("{} ({})", (*val)->getDisplayName(), ((GameObject*)(*val)->getGameObject())->getDisplayName());

			ImGui::InputText(paramName.c_str(), &display, ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget()) {
				bool exists = false;
				Component* target = nullptr;

				if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
					if (std::string(payload->DataType) == std::string("gameObject")) {
						GameObject* drGO = (GameObject*)payload->Data;

						for (unsigned int k = 0; k < drGO->components.size(); k++) {
							if (drGO->components[k]->getDisplayTypeName() == pd.requiredTypeDisplayName) {
								exists = true;
								target = drGO->components[k];
							}
						}
					}
				}

				if (!exists) {
					ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
					ImGui::BeginTooltip();
					ImGui::Text(format("object must have a '{}' component", pd.requiredTypeDisplayName).c_str());
					ImGui::EndTooltip();
				} else {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("gameObject")) {
						(*val) = target;
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			if (ImGui::IsItemClicked()) {
				if (val != nullptr) GUIManager::currentlySelected = (GameObject*)(*val)->getGameObject();
			}

			break;
		}
		case ComponentParameterRegister::ASSET:
		{
			peppermint::Asset** val = (peppermint::Asset**)pd.location;

			std::string display = "error?";
			if (val == nullptr) {
				display = "none";
			} else display = (*val)->getDisplayName();

			ImGui::InputText(paramName.c_str(), &display, ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget()) {
				bool exists = false;
				peppermint::Asset* target = nullptr;

				if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
					if (std::string(payload->DataType) == std::string("asset")) {
						if (((peppermint::Asset*)payload->Data)->getDisplayTypeName() == pd.requiredTypeDisplayName) {
							if (!pd.requiresEnumValue || (pd.requiresEnumValue && ((peppermint::Asset*)payload->Data)->type == pd.requiredEnumValue)) {
								exists = true;
								target = (peppermint::Asset*)payload->Data;
							}
						}
					}
				}

				if (!exists) {
					ImGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
					ImGui::BeginTooltip();
					ImGui::Text(format("asset must be a '{}' asset", pd.requiredTypeDisplayName).c_str());
					ImGui::EndTooltip();
				} else {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("asset")) {
						(*val) = target;
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			if (ImGui::IsItemClicked()) {
				if (val != nullptr) GUIManager::currentlySelected = (peppermint::Asset*)(*val);
			}

			break;
		}
		case ComponentParameterRegister::ASSETV:
		{
			std::vector<peppermint::Asset*>* val = (std::vector<peppermint::Asset*>*)pd.location;

			int sizeIn = val->size();

			ImGui::InputInt(format("size##{}", paramName).c_str(), &sizeIn, 1, 5);
			sizeIn = sizeIn >= 0 ? sizeIn : 0;

			int oldSize = val->size();
			val->resize(sizeIn);

			peppermint::Asset* fallback = nullptr;

			if (val->size() > 0) {
				switch ((*val)[0]->type) {
				case peppermint::Asset::TEXTURE:
					fallback = GUIManager::fallbackTexture;
					break;
				case peppermint::Asset::TEXTURE_SET:
					fallback = GUIManager::fallbackTextureSet;
					break;
				default:
					fallback = GUIManager::fallbackAsset;
					break;
				}
			}

			if (val->size() > oldSize) {
				for (unsigned int k = oldSize; k < val->size(); k++) {
					(*val)[k] = fallback;
				}
			}

			for (unsigned int k = 0; k < val->size(); k++) {
				peppermint::Asset* item = (*val)[k];

				std::string display = "error?";
				if (item == nullptr) {
					display = "none";
				} else display = item->getDisplayName();

				ImGui::InputText(std::format("{} {}##{}", pd.name, k, this->paramIndex - 1).c_str(), &display, ImGuiInputTextFlags_ReadOnly);

				if (ImGui::BeginDragDropTarget()) {
					bool exists = false;
					peppermint::Asset* target;

					if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
						if (std::string(payload->DataType) == std::string("asset")) {
							if (((peppermint::Asset*)payload->Data)->getDisplayTypeName() == pd.requiredTypeDisplayName) {
								if (!pd.requiresEnumValue || (pd.requiresEnumValue && (peppermint::Assetz)))
							}
						}
					}
				}
			}

			break;
		}
		case ComponentParameterRegister::DOUBLE:
		{
			double* val = (double*)pd.location;

			double in = *val;

			if (pd.useMapping) in = pd.unmap(*val);

			double inMin = pd.clamp ? pd.mins[0] : -DBL_MAX;
			double inMax = pd.clamp ? pd.maxes[0] : DBL_MAX;

			if (inMin != -DBL_MAX && inMax != DBL_MAX) {
				ImGui::InputDouble(paramName.c_str(), &in, 0.5);
			} else {
				double step = fmin((inMax - inMin) / 100.0f, 0.5f);
				ImGui::InputDouble(paramName.c_str(), &in, step);
			}

			if (pd.useMapping) {
				(*val) = pd.map(in);
			} else {
				(*val) = in;
			}

			break;
		}
		case ComponentParameterRegister::UINT:
		{
			unsigned int* val = (unsigned int*)pd.location;

			unsigned int in = *val;

			if (pd.useMapping) in = (unsigned int)pd.unmap((float)*val);

			unsigned int inMin = pd.clamp ? pd.mins[0] >= 0 ? pd.mins[0] : 0 : 0;
			unsigned int inMax = pd.clamp ? pd.maxes[0] >= 0 ? pd.maxes[0] : UINT_MAX : UINT_MAX;

			if (inMin != 0 && inMax != UINT_MAX) {
				ImGui::SliderScalar(paramName.c_str(), ImGuiDataType_U32, &in, &inMin, &inMax, "%u", ImGuiSliderFlags_AlwaysClamp);
			} else {
				unsigned int step = (unsigned int)fmin((inMax - inMin) / 100, 1);
				ImGui::DragScalar(paramName.c_str(), ImGuiDataType_U32, &in, step, &inMin, &inMax, "%u", ImGuiSliderFlags_AlwaysClamp);
			}

			if (pd.useMapping) {
				(*val) = (unsigned int)pd.map((float)in);
			} else {
				(*val) = (unsigned int)in;
			}

			break;
		}
		case ComponentParameterRegister::ENUM:
		{
			unsigned int* val = (unsigned int*)pd.location;

			unsigned int in = *val;

			if (ImGui::BeginCombo(paramName.c_str(), pd.enumNames[*val].c_str())) {
				for (unsigned int k = 0; k < pd.enumValues.size(); k++) {
					if (ImGui::Selectable(std::format("{}##{}-{}", pd.enumNames[k], this->paramIndex, k).c_str(), *val == k)) {
						in = k;
					}

					if (in == k) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			(*val) = in;

			break;
		}
		case ComponentParameterRegister::STRING:
		{
			std::string* val = (std::string*)pd.location;

			std::string in = *val;

			ImGui::Text(pd.name.c_str());
			ImGui::InputTextMultiline(paramName.c_str(), &in, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 6), ImGuiInputTextFlags_AllowTabInput);

			(*val) = in;

			break;
		}
		
		default:
		{
			ImGui::TextWrapped("%s: this can't be edited from the inspector!", pd.name.c_str());

			break;
		}
		}
		
		if (pd.disabled) ImGui::EndDisabled();
		
		if (pd.tooltip.length() != 0) {
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
				ImGui::SetItemTooltip(pd.tooltip.c_str());
			}
		}
	}
}

void Inspector::render() {
	this->paramIndex = 0;
	if (!this->locked) {
		this->currentlyViewing = GUIManager::currentlySelected;
	}

	if (this->currentlyViewing == nullptr) {
		this->currentSelectedState = DESELECTED;

		const char* message = "* nothing selected *";
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((lastSize.x - ImGui::CalcTextSize(message).x) / 2.0f, (lastSize.y - ImGui::CalcTextSize(message).y) / 2.0f));

		ImGui::Begin(this->getID().c_str(), &this->active, ImGuiWindowFlags_MenuBar);
		// this->showMenu();

		ImGui::Text(message);

		ImGui::End();
		ImGui::PopStyleVar();

		if (imGuiWin == nullptr) imGuiWin = ImGui::FindWindowByName(this->getID().c_str());
		lastSize = imGuiWin->InnerRect.GetSize();

		return;
	} else {
		ImGui::Begin(this->getID().c_str(), &this->active, ImGuiWindowFlags_MenuBar);
		this->showMenu();

		switch (this->currentlyViewing->getDisplayType()) {
		case GUIInfo::GAME_OBJECT:
		{
			this->currentSelectedState = Inspector::GAME_OBJECT;
			GameObject* go = (GameObject*)this->currentlyViewing;

			ImGui::TextWrapped("Currently selected: %s", go->getDisplayName().c_str());

			for (unsigned int j = 0; j < go->components.size(); j++) {
				Component* co = go->components[j];

				if (co->getDisplayName().length() == 0) co->displayName = "unnamed component type";

				if (ImGui::CollapsingHeader(co->getDisplayName().c_str(), ImGuiTreeNodeFlags_None)) {
					if (co->iconIndex != 0) {
						ImGui::Image((ImTextureID)GUIManager::icons[co->iconIndex]->getGLTexLocation(), ImVec2(30, 30), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
						if (co->description.length() != 0) ImGui::SameLine();
					}
					if (co->description.length() != 0) ImGui::TextWrapped(co->description.c_str());

					this->displayParameters(co->parameters);
				}
			}

			break;
		}
		case GUIInfo::ASSET:
		{
			this->currentSelectedState = Inspector::ASSET;
			peppermint::Asset* as = (peppermint::Asset*)this->currentlyViewing;

			ImGui::TextWrapped("Currently selected: %s", as->getDisplayName().c_str());

			this->displayParameters(as->parameters);

			switch (as->type) {
			case peppermint::Asset::TEXTURE:
			{
				if (ImGui::Button(format("reload##{}", as->getDisplayName()).c_str())) {
					try {
						((peppermint::rendering::Texture*)as)->loadFromAsset();
					} catch (std::exception e) {
						// ((peppermint::rendering::Texture*)as)->setImageAsset(GUIManager::fallbackTexture);
						ImGui::OpenPopup("failed to reload texture");
					}
				}

				ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("failed to reload texture", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::Text("failed to load the texture using the image asset named");
					ImGui::TextWrapped(((peppermint::rendering::Texture*)as)->getImageAsset()->getDisplayName().c_str());

					ImGui::Text("please check its path is set correctly!");

					ImGui::Separator();
					if (ImGui::Button("ok")) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::CollapsingHeader("preview")) {
					float ratio = ImGui::GetContentRegionAvail().x / ((peppermint::rendering::Texture*)as)->getWidth();

					ImGui::Image((ImTextureID)((peppermint::rendering::Texture*)as)->getGLTexLocation(), ImVec2(((peppermint::rendering::Texture*)as)->getWidth() * ratio, ((peppermint::rendering::Texture*)as)->getHeight() * ratio), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				}

				break;
			}
			case peppermint::Asset::TEXTURE_SET:
			{
				ImGui::SeparatorText("previews");

				peppermint::rendering::TextureSet* ts = (peppermint::rendering::TextureSet*)as;

				for (unsigned int j = 0; j < ts->textures.size(); j++) {
					if (ImGui::CollapsingHeader(std::format("{}##{}", ts->textures[j]->getDisplayName(), j).c_str())) {
						float ratio = ImGui::GetContentRegionAvail().x / (ts->textures[j]->getWidth());

						ImGui::Image((ImTextureID)ts->textures[j]->getGLTexLocation(), ImVec2(ratio * ts->textures[j]->getWidth(), ratio * ts->textures[j]->getHeight()), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					}
				}

				break;
			}
			default:
				break;
			}

			break;
		}
		default:
		{
			ImGui::TextWrapped("Currently selected: %s", this->currentlyViewing->getDisplayName().c_str());
			ImGui::TextWrapped("You can't view this in the inspector at the moment. Apologies!");
			break;
		}
		}

		ImGui::End();
	}
}