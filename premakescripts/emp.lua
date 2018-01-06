
project "emp"
kind "ConsoleApp"
language "C++"

files { 
    "main.cpp",
    "main.h",
    "src/**",        
 }

 includedirs{
     "./src/",
     third_party_dir,
     third_party_dir.."/intelTBB/include",
     lua_dir,
     third_party_dir.."/Radiance/src/common",        
 }
 


 filter "platforms:WIN*"
     defines { "WIN" }    
     links {
         third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
         third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
         libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}/*"
     }
     includedirs {
         third_party_dir.."/SketchUp/WIN/headers", -- this changes in different machines
     }     
