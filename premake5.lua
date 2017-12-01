-- premake5.lua


-- Run pre-build commands
premakescripts_dir = "./premakescripts"
dofile(premakescripts_dir.."/prebuild.lua")


workspace "Glare"
    architecture "x86_64"
    platforms { "WIN64", "MACOS", "LINUX" }
    configurations { "DEBUG", "RELEASE" }
    defines{ "GLARE" }

filter "configurations:DEBUG"
    defines { "DEBUG" }

filter "configurations:RELEASE"
    optimize "On"
    
project "GoogleTest"
    kind "StaticLib"
    files { google_test_dir.."/src/gtest-all.cc" }
    includedirs { google_test_dir.."/include", google_test_dir }
    targetdir (libs_dir.."/%{cfg.platform}")        

project "Lua"
    kind "StaticLib"
    language "C"
    targetdir (libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}")    
    files {
        lua_dir.."/lapi.c",
        lua_dir.."/lcode.c",
        lua_dir.."/lctype.c",
        lua_dir.."/ldebug.c",
        lua_dir.."/ldo.c",
        lua_dir.."/ldump.c",
        lua_dir.."/lfunc.c",
        lua_dir.."/lgc.c",
        lua_dir.."/llex.c",
        lua_dir.."/lmem.c",
        lua_dir.."/lobject.c",
        lua_dir.."/lopcodes.c",
        lua_dir.."/lparser.c",
        lua_dir.."/lstate.c",
        lua_dir.."/lstring.c",
        lua_dir.."/ltable.c",
        lua_dir.."/ltm.c",
        lua_dir.."/lundump.c",
        lua_dir.."/lvm.c",
        lua_dir.."/lzio.c",
        lua_dir.."/lauxlib.c",
        lua_dir.."/lbaselib.c",
        lua_dir.."/lbitlib.c",
        lua_dir.."/lcorolib.c",
        lua_dir.."/ldblib.c",
        lua_dir.."/liolib.c",
        lua_dir.."/lmathlib.c",
        lua_dir.."/loslib.c",
        lua_dir.."/lstrlib.c",
        lua_dir.."/ltablib.c",
        lua_dir.."/lutf8lib.c",
        lua_dir.."/loadlib.c",
        lua_dir.."/linit.c" 
    }

project "rtrad"
    kind "StaticLib"
    language "C"
    files {
        rad_common.."/addobjnotify.c", 
        rad_common.."/badarg.c",
        rad_common.."/biggerlib.c",
        rad_common.."/bmalloc.c",
        rad_common.."/bmpfile.c",
        rad_common.."/bsdf.c",
        rad_common.."/bsdf_m.c",        
        rad_common.."/bsdf_t.c",
        rad_common.."/byteswap.c",
        rad_common.."/caldefn.c",
        rad_common.."/calexpr.c",
        rad_common.."/calfunc.c",
        rad_common.."/calprnt.c",
        rad_common.."/ccolor.c",
        rad_common.."/ccyrgb.c",        
        rad_common.."/chanvalue.c",
        rad_common.."/clip.c",
        rad_common.."/color.c",
        rad_common.."/colrops.c",
        rad_common.."/cone.c",
        rad_common.."/cvtcmd.c",
        rad_common.."/dircode.c",
        rad_common.."/disk2square.c",
        rad_common.."/ealloc.c",
        rad_common.."/eputs.c",
        rad_common.."/erf.c",
        rad_common.."/error.c",
        rad_common.."/expandarg.c",
        rad_common.."/ezxml.c",
        rad_common.."/face.c",
        rad_common.."/falsecolor.c",
        rad_common.."/fdate.c",
        rad_common.."/fgetline.c",
        rad_common.."/fgetval.c",
        rad_common.."/fgetword.c",
        rad_common.."/fixargv0.c",
        rad_common.."/font.c",
        rad_common.."/fputword.c",
        --rad_common.."/free_os.c",  -- Deleted for Glare
        rad_common.."/fropen.c",
        rad_common.."/fvect.c",
        rad_common.."/gethomedir.c",
        rad_common.."/getlibpath.c",
        rad_common.."/getpath.c",
        rad_common.."/header.c",
        rad_common.."/hilbert.c",        
        rad_common.."/image.c",
        rad_common.."/instance.c",
        rad_common.."/invmat4.c",
        rad_common.."/lamps.c",
        rad_common.."/linregr.c",
        rad_common.."/loadbsdf.c",
        rad_common.."/loadvars.c",
        rad_common.."/lookup.c",        
        rad_common.."/mat4.c",
        rad_common.."/mesh.c",
        rad_common.."/modobject.c",
        rad_common.."/multisamp.c",
        rad_common.."/myhostname.c",
        rad_common.."/objset.c",
        rad_common.."/octree.c",
        rad_common.."/otypes.c",        
        rad_common.."/paths.c",
        rad_common.."/plocate.c",
        rad_common.."/portio.c",
        rad_common.."/process.c",
        rad_common.."/quit.c",
        rad_common.."/readfargs.c",
        rad_common.."/readmesh.c",
        rad_common.."/readobj.c",        
        rad_common.."/readoct.c",
        rad_common.."/resolu.c",
        rad_common.."/rexpr.c",
        rad_common.."/savqstr.c",
        rad_common.."/savestr.c",
        rad_common.."/sceneio.c",
        rad_common.."/spec_rgb.c",
        rad_common.."/tcos.c",        
        rad_common.."/tmap16bit.c",
        rad_common.."/tmapcolrs.c",
        rad_common.."/tmapluv.c",
        rad_common.."/timegm.c",
        rad_common.."/tmaptiff.c",
        rad_common.."/tmesh.c",
        rad_common.."/tonemap.c",        
        rad_common.."/triangulate.c",
        rad_common.."/urand.c",
        rad_common.."/urind.c",
        rad_common.."/words.c",
        rad_common.."/wordfile.c",
        rad_common.."/wputs.c",
        rad_common.."/xf.c",
        rad_common.."/zeroes.c"
    }
    targetdir (libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}")   
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }

project "radiance"
    kind "StaticLib"
    language "C"
    files {
        rad_rt.."/Version.c", -- this one is generated on prebuild scripts
        rad_rt.."/ambcomp.c", 
        rad_rt.."/ambient.c",
        rad_rt.."/ambio.c", 
        rad_rt.."/aniso.c", 
        rad_rt.."/ashikhmin.c",
        rad_rt.."/data.c",
        rad_rt.."/dielectric.c",
        rad_rt.."/fprism.c",
        rad_rt.."/freeobjmem.c",
        rad_rt.."/func.c", 
        rad_rt.."/glass.c",
        rad_rt.."/initotypes.c",
        rad_rt.."/m_alias.c",
        rad_rt.."/m_brdf.c",
        rad_rt.."/m_bsdf.c",
        rad_rt.."/m_clip.c",
        rad_rt.."/m_direct.c",
        rad_rt.."/m_mirror.c",
        rad_rt.."/m_mist.c",
        rad_rt.."/mx_func.c",
        rad_rt.."/mx_data.c",
        rad_rt.."/noise3.c",
        rad_rt.."/normal.c",
        rad_rt.."/o_face.c", 
        rad_rt.."/o_cone.c",
        rad_rt.."/o_instance.c",
        rad_rt.."/o_mesh.c",
        rad_rt.."/p_data.c",
        rad_rt.."/p_func.c",
        rad_rt.."/preload.c",
        rad_rt.."/raytrace.c",
        rad_rt.."/renderopts.c",
        rad_rt.."/source.c",
        rad_rt.."/sphere.c",
        rad_rt.."/srcobstr.c",
        rad_rt.."/srcsupp.c",
        rad_rt.."/srcsamp.c",
        rad_rt.."/t_data.c",
        rad_rt.."/t_func.c",
        rad_rt.."/text.c",
        rad_rt.."/pmap.c",
        rad_rt.."/pmapsrc.c",
        rad_rt.."/pmapmat.c",
        rad_rt.."/pmaprand.c",
        rad_rt.."/pmapio.c",        
        rad_rt.."/pmapbias.c",
        rad_rt.."/pmapparm.c",
        rad_rt.."/pmapcontrib.c",
        rad_rt.."/pmapamb.c",
        rad_rt.."/pmapray.c",
        rad_rt.."/pmapopt.c",        
        rad_rt.."/pmapdata.c",
        rad_rt.."/pmapdiag.c",
        rad_rt.."/pmaptype.c",
        rad_rt.."/pmutil.c",
        rad_rt.."/pmcontrib2.c",        
        rad_rt.."/../common/random.h",
        rad_rt.."/../common/platform.h",
        rad_rt.."/func.c",
        rad_rt.."/virtuals.c",
        rad_rt.."/../common/paths.c",
    }
    targetdir (libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}")   
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }

project "raycalls"
    radiance_dir = ""
    kind "StaticLib"
    language "C"
    files {
        third_party_dir.."/Radiance/src/rt/raycalls.c",
        third_party_dir.."/Radiance/src/rt/rayfifo.c", 
    }
    includedirs{
        third_party_dir.."/Radiance/src/**",
        third_party_dir
    }
    targetdir (libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}")   
    
project "glare"
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


project "glare_test"
    kind "ConsoleApp"

    filter "configurations:DEBUG"
        defines { "DEBUG" }

    includedirs {
        "./src/",
        third_party_dir,
        google_test_dir.."/include", 
        third_party_dir.."/SketchUp/WIN/headers" 
    }
    
    filter "platforms:WIN*"
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",
            libs_dir.."/%{cfg.platform}".."/%{cfg.buildcfg}/*"
        }
        includedirs{
            "./src/",
            third_party_dir,
            third_party_dir.."/intelTBB/include",
            lua_dir,
            third_party_dir.."/Radiance/src/common",        
        }  

    links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",
            libs_dir.."/%{cfg.platform}/*",           
            third_party_dir.."/intelTBB/lib/intel64/vc14/*"
    }

    

    files { 
        "main_test.cpp",
        "main_test.h",
        "src/**",
        "tests/*.h"
    }
