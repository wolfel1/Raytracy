#pragma once

#include <spdlog/spdlog.h>

namespace raytracy {
	class Log {
	public:
		static void Init();
	};
}

#define RTY_RAYTRACER_TRACE(...) spdlog::get("Raytracer")->trace(__VA_ARGS__)
#define RTY_RAYTRACER_DEBUG(...) spdlog::get("Raytracer")->debug(__VA_ARGS__)
#define RTY_RAYTRACER_INFO(...) spdlog::get("Raytracer")->info(__VA_ARGS__)
#define RTY_RAYTRACER_WARN(...) spdlog::get("Raytracer")->warn(__VA_ARGS__)
#define RTY_RAYTRACER_ERROR(...) spdlog::get("Raytracer")->error(__VA_ARGS__)
#define RTY_RAYTRACER_CRITICAL(...) spdlog::get("Raytracer")->critical(__VA_ARGS__)

#define RTY_APP_TRACE(...) spdlog::get("Raytracy")->trace(__VA_ARGS__)
#define RTY_APP_DEBUG(...) spdlog::get("Raytracy")->debug(__VA_ARGS__)
#define RTY_APP_INFO(...) spdlog::get("Raytracy")->info(__VA_ARGS__)
#define RTY_APP_WARN(...) spdlog::get("Raytracy")->warn(__VA_ARGS__)
#define RTY_APP_ERROR(...) spdlog::get("Raytracy")->error(__VA_ARGS__)
#define RTY_APP_CRITICAL(...) spdlog::get("Raytracy")->critical(__VA_ARGS__)