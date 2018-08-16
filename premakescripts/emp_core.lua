project "emp_core"   
    
    kind "StaticLib"

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
        buildoptions { '/std:c++17', '/permissive-' }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers"
        }
        
        filter "configurations:RELEASE"    
        links {
            libs_dir.."/%{cfg.buildcfg}/tbb/tbb.dll",                
            libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc.dll",                
            libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_proxy.dll",
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/sketchup.dll",   
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpAPI.dll",
        }
        
        filter "configurations:DEBUG"    
        links {
            libs_dir.."/%{cfg.buildcfg}/tbb/tbb_debug.dll",
            libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_debug.dll",
            libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_proxy_debug.dll",
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/sketchup.dll",   
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpAPI.dll",
        }
    
        
    elseif is_macos then
        defines { "MACOS" }       
        buildoptions { '-std=c++11','-stdlib=libc++' }              
        linkoptions {            
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb",                                                      
        }    
        buildoptions {
            "-F /Library/Frameworks",
            "-v",          
        }
        links {
            "SketchUpAPI.framework",
        }
        filter "configurations:RELEASE"    
            links {
                "tbb","tbbmalloc","tbbmalloc_proxy"
            }
    
        filter "configurations:DEBUG"    
            links {
                "tbb_debug","tbbmalloc_debug","tbbmalloc_proxy_debug"
            }
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

   


