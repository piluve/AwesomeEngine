workspace "AwesomeSolution"
	configurations { "Debug", "Release"}
	platforms "x64"
	systemversion "10.0.16299.0"
	
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
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/JSON",
			"Depen/TinyObj",
			"Depen/GLM",
			"Depen/STB",

			"../DirectXTex/DirectXTex",

			"../assimp/include"
		}	
		libdirs
		{
			"../DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Debug",
			"../assimp/lib/Debug"
		}
		links
		{
			"DirectXTex",
			"assimp-vc140-mt",
			"IrrXML",
			"zlibstaticd"
		}
	filter "configurations:Release"
		optimize "On"
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/JSON",
			"Depen/TinyObj",
			"Depen/GLM",
			"Depen/STB",

			"../DirectXTex/DirectXTex",

			"../assimp/include"	
		}
		libdirs
		{
			"../DirectXTex/DirectXTex/Bin/Desktop_2017_Win10/x64/Release",
			"../assimp/lib/Release"
		}
		links
		{
			"DirectXTex",
			"assimp-vc140-mt"
		}

project "AwesomeTriangle"
	kind "ConsoleApp"
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
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}	
		libdirs
		{
		}
		links
		{
			"Graphics"
		}
	filter "configurations:Release"
		optimize "On"
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}
		libdirs
		{
		}
		links
		{
			"Graphics"
		}

project "AwesomeAdvanced"
	kind "ConsoleApp"
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
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}	
		libdirs
		{
		}
		links
		{
			"Graphics"
		}
	filter "configurations:Release"
		optimize "On"
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}
		libdirs
		{
		}
		links
		{
			"Graphics"
		}

project "AwesomeShowcase"
	kind "ConsoleApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/Showcase.cpp",
		"Source/Samples/ShowcaseScene.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}	
		libdirs
		{
			"../assimp/lib/Debug"
		}
		links
		{
			"Graphics",
			"assimp-vc140-mt"
		}
	filter "configurations:Release"
		optimize "On"
		includedirs 
		{
			"Source",
			"Depen/DX12",
			"Depen/GLM"
		}
		libdirs
		{
			"../assimp/lib/Release"
		}
		links
		{
			"Graphics",
			"assimp-vc140-mt"
		}