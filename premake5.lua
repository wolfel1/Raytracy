workspace "Raytracy"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "Raytracy/vendor/spdlog/include"
IncludeDir["glm"] = "Raytracy/vendor/glm"
IncludeDir["glfw"] = "Raytracy/vendor/glfw/include"
IncludeDir["glad"] = "Raytracy/vendor/glad/include"
IncludeDir["stb_image"] = "Raytracy/vendor/stb_image"
IncludeDir["taskflow"] = "Raytracy/vendor/taskflow"


group "Dependencies"
	include "Raytracy/vendor/glad"

	externalproject "glfw"
		location "Raytracy/vendor/glfw/build/src"
		uuid "ee1748e0-155b-4b7f-aea8-afa1cb3b5326"
		kind "StaticLib"
		language "C"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

group ""

include "Raytracy"
include "Sandbox"
include "Test"