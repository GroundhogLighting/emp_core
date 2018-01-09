

project "emp_tests"
kind "ConsoleApp"

files { 
    "../main_test.cpp",
    "../main_test.h",
    "../src/**",
    "../tests/*.h"
}


 
filter "configurations:Debug"
    files {
        third_party_dir.."/nvwa/nvwa/debug_new.cpp", 
    }
    includedirs{
        third_party_dir.."/nvwa/nvwa",     
    }


filter "platforms:WIN*"

    defines { "WIN" }    

    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir,
        third_party_dir.."/Radiance/src/common", 
        google_test_dir.."/include",             
        third_party_dir.."/SketchUp/WIN/headers"
    }  

    links {
        third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",
        libs_dir.."/%{cfg.platform}/*",           
        third_party_dir.."/intelTBB/lib/intel64/vc14/*",
        libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}/*"
    }




