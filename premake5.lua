workspace "Internal"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "Internal/libs/glad/include"

include "Internal/libs/glad"

project "InternalEngine"
	location "Internal"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	defines
	{
	}

	files
	{
		"Internal/src/Internal/Core/**.h",
		"Internal/src/Internal/Events/**.h",
		"Internal/src/Internal/OpenGL/**.h",
		"Internal/src/Internal/Renderer/**.h",
		"Internal/src/Internal/Types/**.h",
		"Internal/src/Internal/Core/**.cpp",
		"Internal/src/Internal/Events/**.cpp",
		"Internal/src/Internal/OpenGL/**.cpp",
		"Internal/src/Internal/Renderer/**.cpp",
		"Internal/src/Internal/Types/**.cpp",
		"Internal/src/Internal/Vulkan/**.cpp",
		"Internal/src/Internal/Vulkan/**.h"
	}

	includedirs
	{
        "Internal/src",
		"%{IncludeDir.Glad}"
	}

	links
	{
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

		files
		{
		    "Internal/src/Internal/Windows/**.cpp",
            "Internal/src/Internal/Windows/**.h"
		}
		
		links
		{
			"opengl32"
		}

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

        files
        {
            "Internal/src/Internal/MacOS/**.cpp",
        	"Internal/src/Internal/MacOS/**.h"
        }

	filter "system:linux"	
		systemversion "latest"
		defines
		{
        }
        
		links
		{
		}

		files
		{
		    "Internal/src/Internal/Linux/**.cpp",
		    "Internal/src/Internal/Linux/**.h"
		}

		includedirs
		{
			"Internal/libs/vulkan/Linux/x86_64/include"
		}

		libdirs
		{
			"Internal/libs/vulkan/Linux/x86_64/lib"
		}

		links
		{
			"vulkan"
		}

    filter "configurations:Debug"
		defines "INTERNAL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
        defines "INTERNAL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "INTERNAL_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Internal/src",
		"%{IncludeDir.Glad}"
	}


	links
	{
        "InternalEngine",
		"Glad"
	}
		

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
		}
		
		links
		{
			"gdi32",
			"opengl32"
		}



	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
		}

		links
		{
		}

	filter "system:linux"	
		systemversion "latest"
		defines
		{
        }
        
		links
        {
            "X11",
            "GL",
			"GLU",
			"dl"
		}


		includedirs
		{
			"Internal/libs/vulkan/Linux/x86_64/include"
		}

		libdirs
		{
			"Internal/libs/vulkan/Linux/x86_64/lib"
		}

		links
		{
			"vulkan"
		}

    filter "configurations:Debug"
		defines "INTERNAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "INTERNAL_RELEASE"
		runtime "Release"		
		optimize "on"
		

	filter "configurations:Dist"
		defines "INTERNAL_DIST"
		runtime "Release"
		optimize "on"