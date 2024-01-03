#ifndef	PPMINT_EDITOR_HIERARCHY_VIEWER_H
#define PPMINT_EDITOR_HIERARCHY_VIEWER_H

#include <editor/windows/GUIWindow.h>

namespace editor {
	namespace gui {
		namespace windows {
			class HierarchyViewer : public GUIWindow {
			public:
				HierarchyViewer();

				void render();
			};
		}
	}
}

#endif