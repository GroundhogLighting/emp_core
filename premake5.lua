-- premake5.lua


-- Run pre-build commands
premakescripts_dir = "./premakescripts"
dofile(premakescripts_dir.."/prebuild.lua")


workspace "Emp"
    architecture "x86_64"
    --platforms { "WIN64", "MACOS", "LINUX" }
    configurations { "DEBUG", "RELEASE" } 
    defines { "EMP" }   

filter "configurations:DEBUG"
    symbols "On"
    defines { 
        "_DEBUG", 
        "TBB_DO_ASSERT=1", 
        "TBB_DO_THREADING_TOOLS=1" 
    }

filter "configurations:RELEASE"
    --optimize "On"
    defines { 
        "TBB_DO_ASSERT=0", 
        "TBB_DO_THREADING_TOOLS=0" 
    }
    

dofile(premakescripts_dir.."/lua.lua")  
dofile(premakescripts_dir.."/rtrad.lua")
dofile(premakescripts_dir.."/radiance.lua")
dofile(premakescripts_dir.."/raycalls.lua")
dofile(premakescripts_dir.."/emp.lua")
dofile(premakescripts_dir.."/emp_tests.lua")
dofile(premakescripts_dir.."/google_test.lua")
  
