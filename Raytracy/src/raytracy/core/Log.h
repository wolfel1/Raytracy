#pragma once

#include "spdlog/spdlog.h"

namespace raytracy {
	class Log {
	public:
		static void Init();
	};
}

#define RTY_CORE_TRACE(...) spdlog::get("Raytracy")->trace(__VA_ARGS__)
#define RTY_CORE_DEBUG(...) spdlog::get("Raytracy")->debug(__VA_ARGS__)
#define RTY_CORE_INFO(...) spdlog::get("Raytracy")->info(__VA_ARGS__)
#define RTY_CORE_WARN(...) spdlog::get("Raytracy")->warn(__VA_ARGS__)
#define RTY_CORE_ERROR(...) spdlog::get("Raytracy")->error(__VA_ARGS__)
#define RTY_CORE_CRITICAL(...) spdlog::get("Raytracy")->critical(__VA_ARGS__)