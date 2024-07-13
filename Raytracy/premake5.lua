project "Raytracy"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "raytracypch.h"
	pchsource "src/raytracypch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"src",
		"../%{IncludeDir.spdlog}",
		"../%{IncludeDir.glm}",
		"../%{IncludeDir.glfw}",
		"../%{IncludeDir.glad}",
		"../%{IncludeDir.stb_image}"
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