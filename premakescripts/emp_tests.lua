

project "emp_tests"

    kind "ConsoleApp"
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
    
    filter "system:windows"
        defines { "WIN" }                                    

        includedirs {
            third_party_dir.."/SketchUp/WIN/headers"
        }
        filter {"configurations:RELEASE","system:windows"}    
            links {
                libs_dir.."/%{cfg.buildcfg}/tbb/tbb.dll",                
                libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc.dll",                
                libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_proxy.dll",
                third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpCommonPreferences.dll",   
                third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpAPI.dll",
            }

        filter {"configurations:DEBUG","system:windows"}
            links {
                libs_dir.."/%{cfg.buildcfg}/tbb/tbb_debug.dll",
                libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_debug.dll",
                libs_dir.."/%{cfg.buildcfg}/tbb/tbbmalloc_proxy_debug.dll",
                third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpCommonPreferences.dll",   
                third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/SketchUpAPI.dll",
            }
        
    
    filter {"system:macosx"}
        defines { "MACOS" }        
        linkoptions {            
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb",              
        }   
        buildoptions {
            "-F /Library/Frameworks",
            "-v"            
        }
        links {
            "SketchUpAPI.framework",
        } 
        filter {"configurations:RELEASE", "system:macosx"}    
            links {
                "tbb","tbbmalloc","tbbmalloc_proxy"
            }

        filter {"configurations:DEBUG","system:macosx"}    
            links {
                "tbb_debug","tbbmalloc_debug","tbbmalloc_proxy_debug"
            }
        
        
    
    filter "system:linux"
        defines { "LINUX", "AVOID_SKP" }    
        links {            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    

    
    
