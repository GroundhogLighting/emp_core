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

    includedirs {
        "./src/3rdparty/SketchUp/win64/headers"
    }

    links { "GoogleTest" }

    filter "configurations:Debug*"
        defines { "DEBUG" }
    
    filter "configurations:*WIN*"
        defines { "WIN" }    
        links {
            "./src/3rdparty/SketchUp/win64/binaries/sketchup/x64/*"
        }

    filter "configurations:*MAC*"
        defines { "MACOS" }  
 
    filter "configurations:*LINUX*"
        defines { "LINUX" }       