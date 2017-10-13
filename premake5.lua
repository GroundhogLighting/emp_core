-- premake5.lua

libs_dir = "./libs"
third_party_dir = "./3rdparty"

workspace "Glare"
    architecture "x86_64"
    platforms { "WIN64", "MACOS", "LINUX" }
    configurations { "DEBUG", "RELEASE" }
    defines{ "GLARE" }
    
project "GoogleTest"
    google_test_dir = "googletest/googletest"
    kind "StaticLib"
    files { google_test_dir.."/src/gtest-all.cc" }
    includedirs { google_test_dir.."/include", google_test_dir }
    targetdir (libs_dir.."/%{cfg.platform}")        

project "Lua"
    lua_dir = "./3rdparty/Lua/src/"
    kind "StaticLib"
    language "C"
    targetdir (libs_dir.."/%{cfg.platform}")   
    files {
        lua_dir.."/lapi.c",
        lua_dir.."/lcode.c",
        lua_dir.."/lctype.c",
        lua_dir.."/ldebug.c",
        lua_dir.."/ldo.c",
        lua_dir.."/ldump.c",
        lua_dir.."/lfunc.c",
        lua_dir.."/lgc.c",
        lua_dir.."/llex.c",
        lua_dir.."/lmem.c",
        lua_dir.."/lobject.c",
        lua_dir.."/lopcodes.c",
        lua_dir.."/lparser.c",
        lua_dir.."/lstate.c",
        lua_dir.."/lstring.c",
        lua_dir.."/ltable.c",
        lua_dir.."/ltm.c",
        lua_dir.."/lundump.c",
        lua_dir.."/lvm.c",
        lua_dir.."/lzio.c",
        lua_dir.."/lauxlib.c",
        lua_dir.."/lbaselib.c",
        lua_dir.."/lbitlib.c",
        lua_dir.."/lcorolib.c",
        lua_dir.."/ldblib.c",
        lua_dir.."/liolib.c",
        lua_dir.."/lmathlib.c",
        lua_dir.."/loslib.c",
        lua_dir.."/lstrlib.c",
        lua_dir.."/ltablib.c",
        lua_dir.."/lutf8lib.c",
        lua_dir.."/loadlib.c",
        lua_dir.."/linit.c" 
    }


project "raycalls"
    radiance_dir = ""
    kind "StaticLib"
    files {third_party_dir.."/Radiance/src/rt/raycalls.c"}
    includedirs{third_party_dir.."/Radiance/src/**"}
    targetdir(third_party_dir.."/Radiance/build")
                   
project "glare"
   kind "ConsoleApp"
   language "C++"

   files { 
       "main.cpp",
       "main.h",
       "src/**"        
    }

    includedirs{
        "./src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir
    }
    
    filter "configurations:DEBUG"
        defines { "DEBUG" }
    
    filter "configurations:RELEASE"
        optimize "On"

    filter "platforms:WIN*"
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",
            libs_dir.."/%{cfg.platform}/*"
        }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers", -- this changes in different machines
        }     


project "glare_test"
    kind "ConsoleApp"

    filter "configurations:DEBUG"
        defines { "DEBUG" }
    
    filter "platforms:WIN*"
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*"
        }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers",      
            third_party_dir.."/intelTBB/include" -- this changes in different machines
        }     

    links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",
            libs_dir.."/%{cfg.platform}/*",           
            third_party_dir.."/intelTBB/lib/intel64/vc14/*"
    }

    includedirs {
        "./src/",
        third_party_dir,
        google_test_dir.."/include", 
        third_party_dir.."/SketchUp/WIN/headers" 
    }

    files { 
        "main_test.cpp",
        "main_test.h",
        "src/**",
        "tests/*.h"
    }
