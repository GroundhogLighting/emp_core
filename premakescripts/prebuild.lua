-- prebuild.lua


-- This file does things required before creating the 
-- projects.

-- Load configurations
dofile(premakescripts_dir.."/config.lua")
-- Create VersionID in Radiance libraries
version_file = io.open("../"..rad_rt.."/VERSION","r")
version = version_file:read()
version_file:close()

file = io.open(rad_rt.."/Version.c",'w')
file:write("VersionID = \""..version.."\";")
file:close()