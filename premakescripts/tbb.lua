premake.modules.lua = {}
local m = premake.modules.lua

local p = premake

newaction {
	trigger = "intelTBB",
	description = "Builds the Intel TBB library",

	onStart = function()
		print("About to compile, move and clean the Intel TBB library")
	end,

	execute = function()
        
        -- Compile tbb in Release and Debug format. Will create the 
        --tbb_release and tbb_debug directories in ./build
        os.execute("cd 3rdparty/IntelTBB; make compiler=clang stdlib=libc++ tbb_build_prefix='tbb' tbb")

        -- Create the lib 
        os.mkdir("libs/DEBUG")
        os.mkdir("libs/RELEASE")

        mv = "mv"
        cp = "cp"
        if is_windows then
            mv = "move"
            cp = "copy"
        end

        -- Remove the old TBB version
        os.rmdir("libs/DEBUG/tbb/tbb_debug")
        os.rmdir("libs/RELEASE/tbb/tbb_release")

        -- Move results to Lib
        os.execute(mv.." 3rdparty/IntelTBB/build/tbb_release libs/RELEASE/tbb")
        os.execute(mv.." 3rdparty/IntelTBB/build/tbb_debug libs/DEBUG/tbb") 
        
        -- Move the dynamic lib to the bin
        os.execute(cp.." libs/RELEASE/tbb/libtbb.dylib bin/RELEASE/libtbb.dylib") 
        os.execute(cp.." libs/DEBUG/tbb/libtbb_debug.dylib bin/DEBUG/libtbb_debug.dylib") 

        
        -- Clean new ones
        os.rmdir("3rdparty/IntelTBB/build/tbb_release")
        os.rmdir("3rdparty/IntelTBB/build/tbb_debug")

          
	end,

	onEnd = function()
		print("Finished compiling Intel TBB")
	end
}

return m