#!/bin/env lua

local d = {}
local files = require(scripts_dir.."/files")
local format = require(scripts_dir.."/format")


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


-- API DOCUMENTATION
-- =================

local user_doc_dir = "glare-doc"

local user_api_doc_file = "api-reference.md"
d.document_api = function()
    file = io.open(user_doc_dir.."/"..user_api_doc_file,'w')
    
    file:write("Testing 2!")

    file:close()
end

-- SCRIPTS DOCUMENTATION
-- =====================

local scripts_dir = "standardscripts"
local scripts_doc_file = "standard-scripts-reference.md"

function parse_script(script_name)
    ret = {
        name = script_name;
        text = "";
        code = "";
        inputs = {};
    }

    local file = io.open("./"..scripts_dir .. "/"..script_name,'r')

    local in_header = false
    local ln = file:read()    
    while ln do
        if string.find(ln,"--%[%[") then
            in_header = true
        elseif in_header and string.find(ln,"%]%]") then
            in_header = false
        elseif in_header and string.find(ln,"@") then            
            -- Delete preceding blank spaces and the at
            ln = string.gsub(ln," ","",string.find(ln,"@")-1)
            ln = string.gsub(ln,"@","")
            
            local end_tag = string.find(ln," ",1)
            local tag = string.sub(ln,0,end_tag-1)            
            local value = string.sub(ln,end_tag,#ln)
            if string.find(tag,"input") then
                tag = string.sub(tag,6)
                tag = string.gsub(tag,"[%]%[]","")
                ret.inputs[#(ret.inputs)+1] = { required = (tag == "required"); v = value }
            else
                ret[tag] = value
            end            
        elseif in_header then
            -- Other text in headers
            if ln ~= "" then
                ret.text = ret.text..ln.."\n"                
            end
        else
            -- code
            ret.code = ret.code..ln.."\n"
        end
        ln = file:read()        
    end

    file:close()
    return ret
end

function document_script(script,main_file)
    warn2("Documenting script "..script.name)
    
    local this_doc_file_name = string.gsub(script.name,".lua",".md")

    -- open file    
    local file = io.open(user_doc_dir.."//standardscripts/"..this_doc_file_name,'w')

    main_file:write(format.header1("Standard scripts"))

    -- Write title
    main_file:write(format.header2(script.title or script.name))
    file:write(format.header1(script.title or script.name))
    
    -- Write author, date and version
    if script.author or script.date or script.version then
        main_file:write(format.date(script.author,script.date,script.version))
        file:write(format.date(script.author,script.date,script.version))
    end

    -- Write brief
    if script.brief then
        main_file:write(format.brief(script.brief))        
        file:write(format.brief(script.brief))
    end

    -- Write link to more info
    main_file:write("[See details](/standardscripts/"..this_doc_file_name..")\n\n")
    

    -- Inputs    
    main_file:write(format.header3("Inputs:"))    
    file:write(format.header2("Inputs:"))
    if #(script.inputs) > 0 then
        for i,input in ipairs(script.inputs) do
            main_file:write(format.input(i,input.v,input.required))
            file:write(format.input(i,input.v,input.required))
        end
    else        
        main_file:write("None")    
        file:write("None")
    end
    main_file:write("\n")
    file:write("\n")
    
    -- write text
    if script.text then
        file:write(format.text(script.text))
    end


    -- Write example
    if script.example then                
        main_file:write(format.header3("Example"))
        file:write(format.header2("Example"))    
        main_file:write(format.code(script.example))
        file:write(format.code(script.example))
    end

    -- write code
    file:write(format.header2("Source code:"))
    if script.code then
        file:write(format.code(script.code))
    end


    file:close()

end

d.document_scripts = function()
    local scripts = files.dir(scripts_dir)
    local titles = {}

    -- document each script script
    local file = io.open(user_doc_dir.."/"..scripts_doc_file,'w')
    for i=1,#scripts do
        s = parse_script(scripts[i])
        titles[#titles+1] = {name = s.name; title = s.title}-- register title and name
        document_script(s,file)        
    end
    file:close()
    
    -- update summary
    local summary_filename = "SUMMARY.md"
    local summary = io.open(user_doc_dir.."/"..summary_filename,'r')
    local lines = {}
    local ln = summary:read()
    local in_scripts = false
    
    while ln do        
        if string.find(ln,string.gsub(scripts_doc_file,"-","%%-")) then
            in_scripts = true            
            lines[#lines+1]=ln
            for i=1,#titles do
                lines[#lines+1] = "  * ["..titles[i].title.."](/standardscripts".."/"..titles[i].name..")"
            end
        else
            if 1 ~= string.find(ln," ",1) then
                lines[#lines+1]=ln
            end
        end        
        ln = summary:read()
    end        
    summary:close()

    summary = io.open(user_doc_dir.."/"..summary_filename,'w')
    for i=1,#lines do
        summary:write(lines[i].."\n")
    end
    summary:close()

end

return d