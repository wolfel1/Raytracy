


project "Raytracy"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin/intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "raytracypch.h"
	pchsource "src/raytracypch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/stb/**.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"src",
		"../%{IncludeDir.glad}"
	}

	links {
		"glfw3",
		"glad",
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"RTY_PLATFORM_WINDOWS"
		}
		links {
			"opengl32.lib"
		}

	filter "system:linux"
        systemversion "latest"
        defines { "RTY_PLATFORM_LINUX" }
        links { "GL", "dl" }


	filter "configurations:Debug"
		defines "RTY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTY_RELEASE"
		runtime "Release"
		optimize "on"