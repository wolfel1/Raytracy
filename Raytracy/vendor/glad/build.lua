project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	warnings "Off"

	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../bin/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/**/*.h",
		"src/*.c"
	}

	includedirs {
		"include"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
