workspace "AwesomeSolution"
	configurations { "Debug", "Release"}
	platforms "x64"
	systemversion "10"
	staticruntime "on"

filter { "platforms:x64" }
	defines { "PLATFORM_WINDOWS" }
	disablewarnings 
	{
		"4006",
		"4221"
	}
	includedirs 
	{
		"Source",
		"Depen/DX12",
		"Depen/JSON",
		"Depen/TinyObj",
		"Depen/GLM",
		"Depen/STB",
		"Assets/Shaders",
		"Depen/DirectXTex/DirectXTex",
		"Depen/assimp/include",
		"Depen/PhysX/physx/include",
		"Depen/PhysX/pxshared/include",
	}
	files
	{
		"Assets/Shaders/**.h"
	}

filter {"configurations:Debug"}
	libdirs
	{
		"Depen/DirectXTex/DirectXTex/Bin/Windows10_2019/x64/Debug",
		"Depen/assimp/lib/Debug",
		"Depen/PhysX/physx/bin/win.x86_64.vc142.mt/debug"
	}
	links
	{
		"DirectXTex",
		"assimp-vc142-mtd",
		"IrrXMLd",
		"zlibstaticd",
		"PhysXCommon_64",
		"PhysX_64",
		"PhysXFoundation_64",
		"PhysXExtensions_static_64",
		"PhysXPvdSDK_static_64"
	}

filter {"configurations:Release"}
	libdirs
	{
		"Depen/DirectXTex/DirectXTex/Bin/Windows10_2019/x64/Release",
		"Depen/assimp/lib/Release",
		"Depen/PhysX/physx/bin/win.x86_64.vc142.mt/release"
	}
	links
	{
		"DirectXTex",
		"assimp-vc142-mt",
		"IrrXML",
		"zlibstatic",
		"PhysXCommon_64",
		"PhysX_64",
		"PhysXFoundation_64",
		"PhysXExtensions_static_64",
		"PhysXPvdSDK_static_64"
	}
	defines
	{
		"NDEBUG"
	}

project "Core"
	kind "StaticLib"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Core/**.h",
		"Source/Core/**.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
	
	filter "configurations:Release"
		optimize "On"

project "Graphics"
	kind "StaticLib"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Graphics/**.h",
		"Source/Graphics/**.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Core"
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Core"
		}

project "AwesomeTriangle"
	kind "WindowedApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/TriangleSample.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Graphics", "Core"
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Graphics", "Core"
		}

project "AwesomeAdvanced"
	kind "WindowedApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/AdvancedSample.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
			"copy %{wks.location}Depen\\assimp\\bin\\%{cfg.buildcfg}\\assimp-vc141-mtd.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\assimp-vc141-mtd.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\debug\\PhysXCommon_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysXCommon_64.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\debug\\PhysX_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysX_64.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\debug\\PhysXFoundation_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysXFoundation_64.dll"
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
			"copy %{wks.location}Depen\\assimp\\bin\\%{cfg.buildcfg}\\assimp-vc141-mt.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\assimp-vc141-mt.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\release\\PhysXCommon_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysXCommon_64.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\release\\PhysX_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysX_64.dll",
			"copy %{wks.location}Depen\\PhysX\\physX\\bin\\win.x86_64.vc141.mt\\release\\PhysXFoundation_64.dll %{wks.location}Build\\%{cfg.platform}\\%{cfg.buildcfg}\\PhysXFoundation_64.dll",
		}