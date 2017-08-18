-- premake5.lua

workspace "Glare"
    architecture "x86_64"

    configurations { "DebugWIN", "WIN", "DebugMACOS", "MACOS", "LINUX", "DebugLINUX" }

project "GoogleTest"
    kind "StaticLib"
    files { "googletest/googletest/src/gtest-all.cc" }
    includedirs { "googletest/googletest/include", "googletest/googletest" }
    targetdir "googletest/build/%{cfg.buildcfg}"        


                   
project "glare"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
       "main.cpp",
       "main.h",
       "src/**"
    }
    
    filter "configurations:Debug*"
        defines { "DEBUG" }
    
    filter "configurations:*WIN*"
        defines { "WIN" }    
        links {
            "./src/3rdparty/SketchUp/WIN/binaries/sketchup/x64/*"
        }
        includedirs {
            "./src/3rdparty/SketchUp/WIN/headers"
        }

    filter "configurations:*MAC*"
        defines { "MACOS" }  
        includedirs {
            "./src/3rdparty/SketchUp/MACOS/headers"
        }
 
    filter "configurations:*LINUX*"
        defines { "LINUX" }       

project "glare_test"
    kind "ConsoleApp"

    links {
            "./src/3rdparty/SketchUp/WIN/binaries/sketchup/x64/*",
            "./googletest/build/%{cfg.buildcfg}/*"
    }

    includedirs { 
        "googletest/googletest/include", 
        "./src/3rdparty/SketchUp/WIN/headers" 
    }

    files { 
        "main_test.cpp",
        "main_test.h",
        "src/**",
        "tests/*.h"
    }