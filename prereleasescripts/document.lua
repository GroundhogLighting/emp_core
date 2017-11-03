#!/bin/env lua

local d = {}

local doxygen_log = "./developer-doc/doc.log"

d.doxygen = function()   
    os.execute("doxygen >"..null_output)
    log = io.open(doxygen_log,"r")
    first_line = log:read()     
    log:close()    
    if not (first_line == nil or string.gsub(first_line,"[ \n]","") == "") then        
        warn2("Doxygen documentation seem to be incomplete")
    end
end

local user_doc_dir = "glare-doc"
local user_api_doc_file = "api-doc.md"
d.document_api = function()
    file = io.open(user_doc_dir.."/"..user_api_doc_file,'w')
    
    file:write("Testing!")

    file:close()
end

return d