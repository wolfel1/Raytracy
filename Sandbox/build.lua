project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin/intermediates/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"../Raytracy/src"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	postbuildcommands {
        "{COPYDIR} resources/ %{cfg.targetdir}"
    }

	links {
		"Raytracy",
		"glfw3",
		"glad",
		"fmt",
	}

	filter "system:linux"
        systemversion "latest"
        defines { "RTY_PLATFORM_LINUX" }
        targetname ("%{prj.name}.out")

	filter "system:windows"
		systemversion "latest"
        defines { "RTY_PLATFORM_WINDOWS" }
		

	filter "configurations:Debug"
		defines "RTY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTY_RELEASE"
		runtime "Release"
		optimize "on"