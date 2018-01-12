-- prebuild.lua


-- This file does things required before creating the 
-- projects.

-- Load configurations
dofile(premakescripts_dir.."/config.lua")

-- Load Radiance version
version_file = io.open(rad_rt.."/VERSION","r")
rad_version = version_file:read()
version_file:close()

-- Load Glare version
version_file = io.open("../src/VERSION","r")
glare_version = version_file:read()
version_file:close()

-- Write Radiance version for itself
file = io.open("../"..rad_rt.."/Version.c",'w')
file:write("char * VersionID = \""..rad_version.."\";")
file:close()

-- Write Radiance and Glare versions
file = io.open("../src/versions.h",'w')
file:write("#pragma once","\n")
file:write("#define EMP_VERSION \"Emp "..glare_version.."\"","\n")
file:write("#define RADIANCE_VERSION \""..rad_version.."\"","\n")
file:close()

-- Create the bin/Debug and bin/Release directories
os.mkdir("../bin/DEBUG")
os.mkdir("../bin/RELEASE")
