project "emp_core"   
    
    kind "StaticLib"

    buildoptions { '-std=c++11','-stdlib=libc++' }
    language "C++" 
    runpathdirs { "." }

    targetdir(libs_dir.."/%{cfg.buildcfg}")

    files {                 
        "../src/**.cpp",
        "../src/**.h", 
        "../include/**.h"       
    }
   
    includedirs{        
        third_party_dir,
        third_party_dir.."/intelTBB/include",                
        rad_common,
        rad_rt                   
    }  

    links {                            
        "rtrad"
    }  


    -- Add the platform specific
    if is_windows then
        defines { "WIN" }               

    elseif is_macos then
        defines { "MACOS" }                    
        linkoptions {            
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb"
        }    
        buildoptions {
            "-F /Library/Frameworks",
            "-v",          
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

    filter "configurations:Release"    
    links {
        "tbb"
    }

    filter "configurations:Debug"    
    links {
        "tbb_debug"
    }


