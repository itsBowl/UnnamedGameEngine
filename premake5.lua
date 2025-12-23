workspace "UnnamedGameEngine"
    architecture "x64"
    startproject "GameEngine"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories
IncludeDir = {}
IncludeDir["SDL"] = "3rdParty/SDL3/include"

