

--add_executable(rtrace rtmain.c rtrace.c duphead.c persist.c)
--target_link_libraries(rtrace raycalls radiance rtrad)

project "rtrace"
    kind "ConsoleApp"
    language "C"
    defines {
        "lint"
    }
    files {
        rad_rt.."/rtmain.c", 
        rad_rt.."/rtrace.c", 
        rad_rt.."/duphead.c", 
        rad_rt.."/persist.c",         
    }
    targetdir "../bin/%{cfg.buildcfg}"
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }
    links{
        --"rtrace",
        "rtrad",
        "radiance",
        "raycalls",
    }