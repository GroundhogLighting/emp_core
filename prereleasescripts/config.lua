#!/bin/env lua

-- Configuration for pre release scripts


is_windows = package.config:sub(1,1) == "\\"

null_output = "/dev/null"
if is_windows then
    null_output = "nul 2>&1"
end

local tab = "    "

function inform(txt)
    io.stderr:write(txt.."\n")
end
    
function warn(txt)    
    io.stderr:write(tab.."... "..txt.."\n")
end

function warn2(txt)
    io.stderr:write(tab..tab.." --> "..txt.."\n")
end