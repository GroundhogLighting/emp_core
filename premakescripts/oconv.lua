project "oconv"
    kind "ConsoleApp"
    language "C"
    defines {
        "lint"
    }
    files {        
        rad_ot.."/bbox.c", 
        rad_ot.."/initotypes.c", 
        rad_ot.."/o_cone.c", 
        rad_ot.."/o_face.c", 
        rad_ot.."/o_instance.c", 
        rad_ot.."/oconv.c", 
        rad_ot.."/sphere.c", 
        rad_ot.."/writeoct.c",        
    }
    targetdir "../bin/%{cfg.buildcfg}"
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }
    links{                        
        "rtrad",
    }