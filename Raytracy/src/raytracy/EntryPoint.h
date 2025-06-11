#pragma once

#include "core/Log.h"
#include "core/Core.h"
#include "Application.h"
#include "debug/Instrumentor.h"

extern raytracy::Application* raytracy::CreateApplication();

namespace raytracy {

	int Main(int argc, char** argv) {
		Log::Init();
		Random::Init();

		RTY_BASE_INFO("Starting Raytracy...");

		RTY_PROFILE_BEGIN_SESSION("Startup", "startup.json");
		auto app = CreateApplication();
		RTY_PROFILE_END_SESSION();

		RTY_PROFILE_BEGIN_SESSION("Runtime", "runtime.json");
		app->Run();
		RTY_PROFILE_END_SESSION();

		RTY_PROFILE_BEGIN_SESSION("Shutdown", "shutdown.json");
		delete app;
		RTY_PROFILE_END_SESSION();

		return 0;
	}
}

int main(int argc, char** argv) {
	return raytracy::Main(argc, argv);
}
