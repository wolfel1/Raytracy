IncludeDir = {}
IncludeDir["glad"] = "Raytracy/vendor/glad/include"

LibDir = {}

workspace "Raytracy"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release"
	}

	filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

   filter "system:linux"
	  IncludeDir["all"] = "vcpkg_installed/x64-linux/include"
	  LibDir["all"] = "vcpkg_installed/x64-linux/lib"
      includedirs { "%{IncludeDir.all}" }
      libdirs { "%{LibDir.all}" }
	  toolset "clang"

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

-- Include directories relative to root folder (solution directory)


group "Dependencies"
	include "Raytracy/vendor/glad/build.lua"

group ""

include "Raytracy/build.lua"
include "Sandbox/build.lua"
include "Test/build.lua"