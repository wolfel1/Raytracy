project "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"test/**.h",
		"test/**.cpp",
        "vendor/googletest/googletest/**.h",
        "vendor/googletest/googletest/**.hpp",
        "vendor/googletest/googletest/src/gtest-all.cc",
        "vendor/googletest/googlemock/**.h",
        "vendor/googletest/googlemock/**.hpp",
        "vendor/googletest/googlemock/src/gmock-all.cc"
	}

	includedirs {
		"../Raytracy/src",
		"../%{IncludeDir.glm}",
		"../%{IncludeDir.glfw}",
		"../%{IncludeDir.spdlog}",
		"vendor/googletest/googletest/include",
		"vendor/googletest/googletest",
		"vendor/googletest/googlemock/include",
		"vendor/googletest/googlemock",
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