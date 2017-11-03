
local f = {}


f.delete_file = function(flnm)
    os.remove(flnm)    
end

f.delete_dir = function(dirname)
    
    if is_windows then
        os.execute("rd /s /q "..dirname)
    else
        os.execute("rm -rf "..dirname)
    end
end

f.dir = function(d)
    dirs = "dirs.tmp"    
    lines = {}

    if is_windows then
        os.execute("dir  /B "..d.." > "..dirs)
    else
        os.execute("ls "..d.." > "..dirs)
    end
    file = io.open(dirs,"r")

    ln = file:read()
    while ln do
        lines[#lines+1] = ln
        ln = file:read()
    end
    file:close()
    f.delete_file(dirs)
    return lines
end


return f