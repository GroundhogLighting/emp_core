-- premake5.lua

workspace "Glare"
    architecture "x86_64"
    platforms { "WIN64", "MACOS", "LINUX" }
    configurations { "DEBUG", "RELEASE" }

project "GoogleTest"
    kind "StaticLib"
    files { "googletest/googletest/src/gtest-all.cc" }
    includedirs { "googletest/googletest/include", "googletest/googletest" }
    targetdir "googletest/build/%{cfg.buildcfg}"        


project "raycalls"
    kind "StaticLib"
    files {"./src/3rdparty/Radiance/src/rt/raycalls.c"}
    includedirs{"./3rdparty/Radiance/src/**"}
    targetdir "./src/3rdparty/Radiance/build"

                   
project "glare"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
       "main.cpp",
       "main.h",
       "src/**"        
    }

    includedirs{
        "./src/",
        "./3rdparty"
    }
    
    filter "configurations:DEBUG"
        defines { "DEBUG" }
    
    filter "platforms:WIN*"
        defines { "WIN" }    
        links {
            "./3rdparty/SketchUp/WIN/binaries/sketchup/x64/*",
            "./3rdparty/Radiance/build/*"
        }
        includedirs {
            "./3rdparty/SketchUp/WIN/headers", -- this changes in different machines
        }     


project "glare_test"
    kind "ConsoleApp"

    links {
            "./3rdparty/SketchUp/WIN/binaries/sketchup/x64/*",
            "./googletest/build/%{cfg.buildcfg}/*"
    }

    includedirs { 
        "googletest/googletest/include", 
        "./3rdparty/SketchUp/WIN/headers" 
    }

    files { 
        "main_test.cpp",
        "main_test.h",
        "src/**",
        "tests/*.h"
    }
