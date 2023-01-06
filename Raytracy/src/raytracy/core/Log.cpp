#include "raytracypch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace raytracy {

	shared_ptr<spdlog::logger> Log::base_logger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		base_logger = spdlog::stdout_color_mt("Raytracy");
		base_logger->set_level(spdlog::level::trace);
		auto raytracer_logger = spdlog::stdout_color_mt("Raytracer");
		raytracer_logger->set_level(spdlog::level::trace);
	}
}