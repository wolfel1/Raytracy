#pragma once

#include <iostream>
#include "core/Core.h"
#include "Application.h"

#ifdef RTY_PLATFORM_WINDOWS

extern raytracy::Application* raytracy::CreateApplication();

namespace raytracy {

	int Main(int argc, char** argv) {
		std::cout << "Starting App...";

		auto app = CreateApplication();

		app->Run();


		return 0;
	}
}

int main(int argc, char** argv) {
	return raytracy::Main(argc, argv);
}
#else
#error This app only supports Windows for now
#endif // RTY_PLATFORM_WINDOWS
