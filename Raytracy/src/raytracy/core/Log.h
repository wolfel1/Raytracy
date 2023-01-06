#pragma once

#include <spdlog/spdlog.h>

#include "Core.h"

namespace raytracy {
	class Log {
	private:
		static shared_ptr<spdlog::logger> base_logger;
	public:
		static void Init();

		inline static shared_ptr<spdlog::logger>& GetBaseLogger() {
			return base_logger;
		}
	};
}

#define RTY_RAYTRACER_TRACE(...)        spdlog::get("Raytracer")->trace(__VA_ARGS__)
#define RTY_RAYTRACER_DEBUG(...)        spdlog::get("Raytracer")->debug(__VA_ARGS__)
#define RTY_RAYTRACER_INFO(...)         spdlog::get("Raytracer")->info(__VA_ARGS__)
#define RTY_RAYTRACER_WARN(...)         spdlog::get("Raytracer")->warn(__VA_ARGS__)
#define RTY_RAYTRACER_ERROR(...)        spdlog::get("Raytracer")->error(__VA_ARGS__)
#define RTY_RAYTRACER_CRITICAL(...)     spdlog::get("Raytracer")->critical(__VA_ARGS__)

#define RTY_BASE_TRACE(...)             raytracy::Log::GetBaseLogger()->trace(__VA_ARGS__)
#define RTY_BASE_DEBUG(...)             raytracy::Log::GetBaseLogger()->debug(__VA_ARGS__)
#define RTY_BASE_INFO(...)              raytracy::Log::GetBaseLogger()->info(__VA_ARGS__)
#define RTY_BASE_WARN(...)              raytracy::Log::GetBaseLogger()->warn(__VA_ARGS__)
#define RTY_BASE_ERROR(...)             raytracy::Log::GetBaseLogger()->error(__VA_ARGS__)
#define RTY_BASE_CRITICAL(...)          raytracy::Log::GetBaseLogger()->critical(__VA_ARGS__)