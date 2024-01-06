workspace "Raytracy"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
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

project "Raytracy"
	location "Raytracy"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "raytracypch.h"
	pchsource "Raytracy/src/raytracypch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.stb_image}"
	}

	links {
		"glfw",
		"glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		defines "RTY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTY_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RTY_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Raytracy/src",
		"Raytracy/vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
	}

	links {
		"Raytracy"
	}

	filter "system:windows"
		systemversion "latest"
		

	filter "configurations:Debug"
		defines "RTY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTY_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RTY_DIST"
		runtime "Release"
		optimize "on"


