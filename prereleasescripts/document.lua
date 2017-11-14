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
    if not (first_line == nil or first_line:gsub("[ \n]","") == "") then        
        warn2("Doxygen documentation seem to be incomplete")
    end
end


-- API DOCUMENTATION
-- =================

local user_doc_dir = "glare-doc"

local user_api_doc_file = "api-reference.md"
d.document_api = function()
    -- Create container
    local funcs = {}
    
    -- Open the source file
    source = io.open("./src/api/api.cpp",'r')
    ln = source:read()
    local current_group = false
    local current_function = false   
    local in_function = false
    while ln do        
        if ln:find("@APIgroup") then
            current_group = ln:gsub("[*/]","")
            _,e = current_group:find("APIgroup")
            current_group = current_group:sub(e+2):lower()
            current_group = current_group:sub(1,1):upper()..current_group:sub(2)            
            funcs[current_group] = {}
            current_function = {
                params = {};
                returns = {};
                text = {};
            }
        elseif ln:find("@APIfunction") then
            in_function = true
        elseif ln:find("lua_register") then
            local first_comma = ln:find(",",1)
            local second_comma = ln:find(",",first_comma+1)
            local name = ln:sub(first_comma+1,second_comma-1):gsub("[\" ]","") 
            funcs[current_group][name] = current_function
            current_function = {
                params = {};
                returns = {};
                text = {};
            }
            in_function  = false
        elseif in_function and ln:find("@") then
            local at = ln:find("@")
            local blank = ln:find(" ",at+1)
            local tag = ln:sub(at+1,blank)
            local v = ln:sub(blank+1)
            if tag:find("param") then
                local r = false
                if tag:find("required") then
                    r = true
                end
                local second_blank = v:find(" ")
                local param_name = v:sub(0,second_blank-1)
                local param_description = v:sub(second_blank)
                current_function.params[param_name] = {required = r; value = param_description}
            elseif tag:find("return") then
                local second_blank = v:find(" ")
                local ret_name = v:sub(0,second_blank-1)
                local ret_description = v:sub(second_blank)                
                current_function.returns[ret_name] = ret_description            
            else
                current_function[tag] = v
            end
        elseif in_function and not ln:find("[*/]") then             
            current_function.text[#current_function.text+1] = ln
        end
        ln = source:read()
    end

    source:close()

    -- Write the resulting file
    result_file = io.open(user_doc_dir.."/"..user_api_doc_file,'w')

    -- Write title
    result_file:write(format.header1("Glare's API reference manual"))
    result_file:write("All the functions in Glare's API")

    result_file:write(format.header2("Index"))
    -- Write index
    for group_name,group in pairs(funcs) do
        result_file:write(format.header3(group_name))
        for f,_ in pairs(group) do
            result_file:write("- "..f.."\n")
        end
        result_file:write("\n")
    end
    
    result_file:write("\n---\n")

    -- Write functions    
    result_file:write(format.header2("Functions"))

    for group_name,group in pairs(funcs) do
        result_file:write(format.header3(group_name))
        for f,data in pairs(group) do
            -- Write name of function
            p = {}
            for param,v in pairs(data.params) do
                p[#p+1] = param
            end            
            result_file:write(format.headerN(f.."("..table.concat(p,",")..")",4))
            
            -- Write text
            tag = "text"
            value = data[tag]
            for i=1,#value do
                s = value[i]:find("%w")
                if s then
                    result_file:write(value[i]:sub(s).."\n")                
                else
                    result_file:write(value[i])
                end
            end

            -- Write params
            tag = "params"
            value = data[tag]
            result_file:write("\n"..format.headerN("Arguments",7).."\n")
            local n = 0
            for ret,v in pairs(value) do
                local req = ""
                if v.required then
                    req = " | required"
                end
                result_file:write("- **"..ret.."**:"..v.value..req.."\n")
                n = n+1
            end              
            if n == 0 then
                result_file:write("- none\n")
            end
        

            -- Write returns
            tag = "returns"
            value = data[tag]
            result_file:write("\n"..format.headerN("Return",7).."\n")
            local n = 0
            for ret,v in pairs(value) do
                result_file:write("- **"..ret.."**: "..v.."\n")
                n = n+1
            end
            if n == 0 then
                result_file:write("- none\n")
            end
        
        end
    end

    result_file:close()
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
        if ln:find("--%[%[") then
            in_header = true
        elseif in_header and ln:find("%]%]") then
            in_header = false
        elseif in_header and ln:find("@") then            
            -- Delete preceding blank spaces and the at
            ln = ln:gsub(" ","",ln:find("@")-1)
            ln = ln:gsub("@","")
            
            local end_tag = ln:find(" ",1)
            local tag = ln:sub(0,end_tag-1)            
            local value = ln:sub(end_tag,#ln)
            if tag:find("input") then
                tag = tag:sub(6)
                tag = tag:gsub("[%]%[]","")
                ret.inputs[#(ret.inputs)+1] = { required = (tag == "required"); v = value }
            else
                ret[tag] = value
            end            
        elseif in_header then
            -- Other text in headers
            ret.text = ret.text..ln.."\n"                            
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
    
    local this_doc_file_name = script.name:gsub(".lua",".md")

    -- open file    
    local file = io.open(user_doc_dir.."/"..scripts_dir.."/"..this_doc_file_name,'w')
    
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
    main_file:write("[See details](/"..scripts_dir.."/"..this_doc_file_name..")\n\n")
    
    -- write text
    if script.text then
        file:write(format.text(script.text))
    end

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
    file:write(format.header1("Standard scripts"))
    
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
        if ln:find(scripts_doc_file:gsub("-","%%-")) then
            in_scripts = true            
            lines[#lines+1]=ln
            for i=1,#titles do
                lines[#lines+1] = "  * ["..titles[i].title.."](/"..scripts_dir.."/"..titles[i].name:gsub(".lua",".md")..")"
            end
        else
            if 1 ~= ln:find(" ",1) then
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