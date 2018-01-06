#!/bin/env lua


local c = {}

local files = require(scripts_dir.."/files")

c.clean = function()    
    files.delete_file("./premakescripts/*.vcxproj")
    files.delete_file("*.sln*")
    
    files.delete_dir("./premakescripts/*.xcodeproj")
    files.delete_dir("*.xcworkspace")
    
    files.delete_dir("obj")
    files.delete_dir("libs")
    files.delete_dir("bin")
end

return c