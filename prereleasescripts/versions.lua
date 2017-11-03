-- versions.lua

--[[
    This file provides functions for handling versions
--]]

local v = {}

local version_file = "./src/VERSION"

local function read_version_file()    
    version = io.open(version_file,"r")
    glare_version = version:read()
    version:close()
    return glare_version:match("([^,]+).([^,]+).([^,]+)")
end

local function write_version_file(major,minor,patch)
    file = io.open(version_file,'w')
    file:write(string.format("%d.%d.%d",major,minor,patch))
    file:close()
end

v.increase_patch = function()
    major,minor,patch = read_version_file()
    write_version_file(major,minor,patch+1)
end


v.increase_minor = function()
    major,minor,patch = read_version_file()
    write_version_file(major,minor+1,0)
end


v.increase_major = function()
    major,minor,patch = read_version_file()
    write_version_file(major+1,0,0)
end

return v