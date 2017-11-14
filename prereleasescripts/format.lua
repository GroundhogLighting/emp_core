#!/usr/bin/env lua

format = {}

format.header1 = function(text)
    return "\n# "..text.."\n\n"
end

format.header2 = function(text)
    return "\n## "..text.."\n\n"
end

format.header3 = function(text)
    return "\n### "..text.."\n\n"
end

format.headerN = function(text,n)
    if n > 6 then
        return "\n<h7>"..text.."</h7>\n\n"
    end
    return "\n"..string.rep("#",n).." "..text.."\n\n"
end

format.text = function(text)
    return text.."\n\n"
end

format.brief = function(text)
    return text.."\n\n"
end

format.code = function(text)
    return "```lua\n"..text.."\n```\n\n"
end

format.date = function(author,date,version)
    author = author or ""
    date = date or ""
    version = version or ""
    
    return format.headerN(author.." | "..date .. " | version "..version,6)
end

format.input = function(i,value,required)
    r = "-- optional"
    if required then
        r = "-- required"
    end
    return i..". "..value.." "..r.."\n"
end

format.bold = function(text)
    return "**"..text.."**\n"
end

return format