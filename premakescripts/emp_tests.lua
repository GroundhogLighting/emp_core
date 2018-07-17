

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
        "../tests/*/*.h",        
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
        linkoptions {            
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb",  
            --"-Wl,-rpath,\\$ORIGIN"          
        }    
        buildoptions {
            "-F /Library/Frameworks",
            "-v"            
        }
        links {
            "SketchUpAPI.framework",
        }
        
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

    
    filter "configurations:RELEASE"    
    links {
        "tbb","tbbmalloc","tbbmalloc_proxy"
    }

    filter "configurations:DEBUG"    
    links {
        "tbb_debug","tbbmalloc_debug","tbbmalloc_proxy_debug"
    }
