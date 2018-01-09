

project "emp_tests"
    kind "ConsoleApp"
    buildoptions { '-std=c++11' }
    language "C++" 

    files { 
        "../main_test.cpp",
        "../main_test.h",
        "../src/**",
        "../tests/*.h"
    }


    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir,
        third_party_dir.."/Radiance/src/common", 
        google_test_dir.."/include",             
        
    }  

    links {
        third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",
        libs_dir.."/%{cfg.platform}/*",           
        third_party_dir.."/intelTBB/lib/intel64/vc14/*"
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
        defines { "MACOS", "AVOID_SKP" }    
        links {
            --third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }  
        elseif is_linux then
            defines { "LINUX", "AVOID_SKP" }    
            links {
                --third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
                third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
            }

        end



