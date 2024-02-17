
project "SKServer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		"SK_SERVER",
		"_CRT_SECURE_NO_WARNINGS",
		
		"YAML_CPP_STATIC_DEFINE"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/SnakesCore/vendor/spdlog/include",
		"%{wks.location}/SnakesCore/vendor/ImGui",
		"%{wks.location}/SnakesCore/vendor/glm",
		"%{wks.location}/SnakesCore/vendor/yaml-cpp/include",

		"src",
		"%{wks.location}/SnakesCore/src"
	}

	links
	{
		"yaml-cpp",

		"SnakesCore"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SK_PLATFORM_WINDOWS"
		}
		
		prebuildcommands
		{
			"{COPY} ../dlls/ ../bin/" .. outputdir .. "/%{prj.name}"
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