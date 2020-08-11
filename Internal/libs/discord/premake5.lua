project "Discord"
	kind "SharedLib"
	language "C"
	staticruntime "on"

	files
	{
		"cpp/**.h",
		"cpp/**.cpp"
	}

	includedirs
	{
		"cpp"
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
