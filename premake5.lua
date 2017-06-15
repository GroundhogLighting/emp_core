-- premake5.lua
workspace "Glare"
    configurations { "Debug64" }

    filter "configurations:*32"
        architecture "x86"

    filter "configurations:*64"
        architecture "x86_64"
    
    links {
        "./libs/SketchUp/binaries/sketchup/x64/*"
    }

    includedirs {
        "./libs/SketchUp/headers"
    }

project "Glare"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { 
       "main.cpp"
    }

   filter "configurations:Debug64"
      defines { "DEBUG" }
      symbols "Off"
