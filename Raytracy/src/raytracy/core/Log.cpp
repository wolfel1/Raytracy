#include "raytracypch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
namespace raytracy {
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		auto logger = spdlog::stdout_color_mt("Raytracy");
		logger->set_level(spdlog::level::trace);
	}
}