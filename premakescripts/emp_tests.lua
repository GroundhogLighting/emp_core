

project "emp_tests"

    kind "ConsoleApp"
    buildoptions { '-std=c++11','-stdlib=libc++' }
    language "C++" 
    runpathdirs { "." }

    targetdir "../bin/%{cfg.buildcfg}"

    files { 
        "../main_test.cpp",
        "../main_test.h",        
        "../tests/*.h",        
    }
   
    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",        
        google_test_dir.."/include",  
        rad_common,
        rad_rt                   
    }  

    links {                
        "GoogleTest",                
        "emp_core"
    }  


    -- Add the platform specific
    if is_windows then
        defines { "WIN" }               

    elseif is_macos then
        defines { "MACOS" }            
        --runpathdirs { "libs" }
        linkoptions {            
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb"
        }    
        buildoptions {
            "-F "..third_party_dir.."/SketchUp/MACOS/headers",
            --"-F/Users/german/Documents/emp_core/3rdparty/SketchUp/MACOS/headers/",
            "-v"            
        }
        links {
            third_party_dir.."/SketchUp/MACOS/headers/SketchUpAPI.framework",            
        }
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

    filter "configurations:Release"    
    links {
        "tbb"
    }

    filter "configurations:Debug"
    files {
        
    }
    includedirs{
        
    }
    links {
        "tbb_debug"
    }


