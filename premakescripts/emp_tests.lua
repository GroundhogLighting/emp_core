

project "emp_tests"

    kind "ConsoleApp"
    buildoptions { '-std=c++11','-stdlib=libc++' }
    language "C++" 
    runpathdirs { "." }

    targetdir "../bin/%{cfg.buildcfg}"

    files { 
        "../main_test.cpp",
        "../main_test.h",
        "../src/**",
        "../tests/*.h",
        -- third_party_dir.."/Radiance/src/gen/sun.c" -- For generating sky vectors    
    }
   
    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir,
        google_test_dir.."/include",  
        rad_common,
        rad_rt                   
    }  

    links {                
        "GoogleTest",
        "Lua",  
        --"radiance",
        --"raycalls",
        "rtrad"
    }  


    -- Add the platform specific
    if is_windows then
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*"                                  
        }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers", 
        }    

    elseif is_macos then
        defines { "MACOS" }    
        links {
            third_party_dir.."/SketchUp/MACOS/headers/SketchUpAPI.framework",
        }
        buildoptions {
            "-F "..third_party_dir.."/SketchUp/MACOS/headers"            
        }
        runpathdirs { "libs" }
        linkoptions {
            "-F "..third_party_dir.."/SketchUp/MACOS/headers", 
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb"
        }    
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {
            --third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

    filter "configurations:Release"    
    links {
        "tbb"
    }

    filter "configurations:Debug"
    files {
        third_party_dir.."/nvwa/nvwa/debug_new.cpp", 
    }
    includedirs{
        third_party_dir.."/nvwa/nvwa",     
    }
    links {
        "tbb_debug"
    }
    --buildoptions {
    --    "-Wl,-no_pie"            
    --}


