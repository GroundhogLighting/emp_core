
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
