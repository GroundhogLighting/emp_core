#!/usr/bin/env lua

scripts_dir = "prereleasescripts"
dofile(scripts_dir.."/config.lua")

local versioning = require(scripts_dir.."/versions")
local document = require(scripts_dir.."/document")
local clean = require(scripts_dir.."/clean")


-- Variable with the available commands
local commands = {}


-- Adds one more number to the patch
commands["increase_patch"] = versioning.increase_patch
commands["increase_minor"] = versioning.increase_minor
commands["increase_major"] = versioning.increase_major

-- Document
commands["doxygen"] = document.doxygen
commands["document_api"] = document.document_api
commands["document_scripts"] = document.document_scripts

-- Files
commands["clean"] = clean.clean

-- MAIN

inform("\n================")
unknown_commands = {}

for i,command in ipairs(arg) do       
    if commands[command] == nil then        
        unknown_commands[#unknown_commands+1] = command
    else        
        -- Execute
        warn("executing action '"..command.."'")
        commands[command]()
    end
end

-- report errors

if #unknown_commands > 0 then
    io.stderr:write("\nCommands")
    for i = 1, #unknown_commands do
        io.stderr:write(" '"..unknown_commands[i].."' ")
    end

    io.stderr:write("do not exist... available commands are:\n\n")
    
    for i,v in pairs(commands) do
        io.stderr:write("    "..i.."\n")
    end
    
end

inform("================\n")