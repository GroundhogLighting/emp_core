--add_executable(gendaylit gendaylit.c sun.c)
--target_link_libraries(gendaylit rtrad ${LIB_M})

project "gensky"
    kind "ConsoleApp"
    language "C"
    defines {
        "lint"
    }
    files {        
        rad_gen.."/gensky.c",               
        rad_gen.."/sun.c",               
    }
    targetdir "../bin/%{cfg.buildcfg}"
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }
    links{                        
        "rtrad",
    }