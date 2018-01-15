
project "emp"
    kind "ConsoleApp"
    language "C++"
    buildoptions { '-std=c++11' }

    targetdir "../bin/%{cfg.buildcfg}"

    files { 
        "../main.cpp",
        "../src/**",    
        "../main.h"    
    }
    links {
        "tbb_debug",
        "Lua",
        "radiance",
        "raycalls",
        "rtrad"                                     
    }  

    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir,
        third_party_dir.."/Radiance/src/common",        
    }
    
    -- Add the platform specific
    if is_windows then
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers", 
        }    
    elseif is_macos then
        defines { "MACOS" }    
        
        buildoptions {
            "-F "..third_party_dir.."/SketchUp/MACOS/headers",            
        }
        links {
            third_party_dir.."/SketchUp/MACOS/headers/SketchUpAPI.framework",
        }
        linkoptions {
            --"-F "..third_party_dir.."/SketchUp/MACOS/headers", 
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb"
        }    
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {
            --third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

    filter "configurations:Debug"
    files {
        third_party_dir.."/nvwa/nvwa/debug_new.cpp", 
        third_party_dir.."/nvwa/nvwa/debug_new.h", 
        
    }
    includedirs{
        third_party_dir.."/nvwa/nvwa",     
    }

    buildoptions {
        "-Wl,-no_pie"            
    }

