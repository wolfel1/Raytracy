#pragma once

namespace raytracy {
	class Application {
	private:
		bool running;

		static Application* instance;
	public:
		Application();
		virtual ~Application();

		void Run();

		inline static Application& Get() {
			return *instance;
		}
	};

	Application* CreateApplication();
}

