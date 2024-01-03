#ifndef PPMINT_EDITOR_GUI_INFO_H
#define PPMINT_EDITOR_GUI_INFO_H

#include <string>

namespace editor {
	namespace gui {
		class GUIInfo {
		public:
			std::string displayName = "";
			unsigned int iconIndex = 0;
			std::string description = "";

			enum GUIType {
				UNSET,
				GAME_OBJECT,
				COMPONENT,
				ASSET
			};

			virtual GUIType getDisplayType() {
				return this->displayType;
			}

			virtual std::string getDisplayTypeName() {
				return this->displayTypeName;
			}

			virtual std::string getDisplayName() {
				return this->displayName.length() == 0 ? this->displayTypeName : this->displayName;
			}

		protected:
			GUIType displayType;
			std::string displayTypeName = "";
		};
	}
}

#endif