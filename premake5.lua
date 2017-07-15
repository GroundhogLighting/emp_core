-- premake5.lua
workspace "Glare"
    architecture "x86_64"

    configurations { "DebugWIN", "WIN", "DebugMACOS", "MACOS", "LINUX", "DebugLINUX" }

    includedirs {
        "./libs/SketchUp/win64/headers"
    }

    filter "configurations:Debug*"
        defines { "DEBUG" }
    
    filter "configurations:*WIN*"
        defines { "WIN" }    
        links {
            "./libs/SketchUp/win64/binaries/sketchup/x64/*"
        }

    filter "configurations:*MAC*"
        defines { "MACOS" }  
 
    filter "configurations:*LINUX*"
        defines { "LINUX" }                          

project "Glare"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
       "main.cpp",
       "main.h",
       "src/**"
    }

   
