#ifndef PPMINT_EDITOR_EDITOR_MANAGER_H
#define PPMINT_EDITOR_EDITOR_MANAGER_H

namespace editor {
	namespace managers {
		class EditorManager {
		public:
			static void initialise();
			static void destroy();

			static void loop();

			static void updateDeltaTime();
			
			static int status;

			static double deltaTime;
			static double desiredFramerate;
		private:
			static double lastFrame;
		};
	}
}

#endif