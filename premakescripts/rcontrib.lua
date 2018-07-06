--add_executable(rcontrib rcmain.c rcontrib.c rc2.c rc3.c)
--target_link_libraries(rcontrib radiance rtrad)


project "rcontrib"
    kind "ConsoleApp"
    language "C"
    defines {
        "lint"
    }
    files {
        rad_rt.."/rcmain.c", 
        rad_rt.."/rcontrib.c", 
        rad_rt.."/rc2.c", 
        rad_rt.."/rc3.c",         
    }
    targetdir "../bin/%{cfg.buildcfg}"
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }
    links{                
        "radiance",
        "rtrad",
    }