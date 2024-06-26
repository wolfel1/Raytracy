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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
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

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
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

project "Test"
	location "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/test/**.h",
		"%{prj.name}/test/**.cpp",
        "%{prj.name}/vendor/googletest/googletest/**.h",
        "%{prj.name}/vendor/googletest/googletest/**.hpp",
        "%{prj.name}/vendor/googletest/googletest/src/gtest-all.cc",
        "%{prj.name}/vendor/googletest/googlemock/**.h",
        "%{prj.name}/vendor/googletest/googlemock/**.hpp",
        "%{prj.name}/vendor/googletest/googlemock/src/gmock-all.cc"
	}

	includedirs {
		"Raytracy/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.spdlog}",
		"%{prj.name}/vendor/googletest/googletest/include",
		"%{prj.name}/vendor/googletest/googletest",
		"%{prj.name}/vendor/googletest/googlemock/include",
		"%{prj.name}/vendor/googletest/googlemock",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
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