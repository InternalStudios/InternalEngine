project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	files
	{
		"include/glad/glad.h",
		"src/glad.c"
	}

	includedirs
	{
		"include/"
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
	    files
	    {
	        "include/glad/glad_glx.h",
	        "src/glad_glx.c"
	    }

	filter "system:windows"
		files
		{
			"include/glad/glad_wgl.h",
			"src/glad_wgl.c"
		}

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"