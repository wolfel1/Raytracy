#include "raytracypch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
namespace raytracy {
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		auto raytracer_logger = spdlog::stdout_color_mt("Raytracer");
		raytracer_logger->set_level(spdlog::level::trace);
		auto raytracy_logger = spdlog::stdout_color_mt("Raytracy");
		raytracy_logger->set_level(spdlog::level::trace);
	}
}