
workspace "Snakes"
architecture "x64"
startproject "Snakes"

configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "SnakesCore/vendor/glad"
	include "SnakesCore/vendor/ImGui"
	include "SnakesCore/vendor/yaml-cpp"
group ""

group "Core"
	include "SnakesCore"
	include "Snakes"
group ""
