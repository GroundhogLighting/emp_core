
project "raycalls"
radiance_dir = ""
kind "StaticLib"
language "C"
files {
    third_party_dir.."/Radiance/src/rt/raycalls.c",
    third_party_dir.."/Radiance/src/rt/raypwin.c", -- this is only for windows... otherwise it is raypcalls.c        
    third_party_dir.."/Radiance/src/rt/rayfifo.c", 
}
includedirs{
    third_party_dir.."/Radiance/src/**",
    third_party_dir
}
targetdir (libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}")   
