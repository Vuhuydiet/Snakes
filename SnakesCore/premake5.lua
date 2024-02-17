project "SnakesCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "skpch.h"
	pchsource "src/skpch.cpp"

	defines
	{
		"SK_LIB",

		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	files
    {
        "src/**.h",
        "src/**.cpp",

		"vendor/glm/**.hpp",
		"vendor/glm/**.inl",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp"
    }

	includedirs
	{
		"vendor/spdlog/include",
		"vendor/SDL2/include",
		"vendor/SDL2_image/include",
		"vendor/glad/include",
		"vendor/ImGui",
		"vendor/glm",
		"vendor/stb_image",
		"vendor/yaml-cpp/include",

		"src"

	}

	libdirs
	{
		"vendor/SDL2/lib/x64",
		"vendor/SDL2_image/lib/x64"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"opengl32.lib",

		"glad",
		"ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS"
		}

		prebuildcommands
		{
			"{COPY} %{wks.location}/dlls/ %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
		}

	filter "configurations:Debug"
		defines "SK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SK_DIST"
		runtime "Release"
		optimize "on"
