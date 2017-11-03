#!/bin/env lua


local f = {}

function delete_file(flnm)
    if is_windows then
        os.execute("del "..flnm)
    else -- assume is unix
        os.execute("rm "..flnm)
    end
end

function delete_dir(dirname)
    if is_windows then
        os.execute("rd /s /q "..dirname)
    else
        os.execute("rm -rf "..dirname)
    end
end

f.clean = function()    
    delete_file("*.vcxproj*")
    delete_file("*.sln*")
    delete_dir("obj")
    delete_dir("libs")
    delete_dir("bin")
end

return f