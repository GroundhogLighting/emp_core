-- Configurations for premake

-- Emp basic organizations
libs_dir = "../libs"
third_party_dir = "../3rdparty"

-- Google test directory
google_test_dir = "../googletest/googletest"

-- Radiance specific locations
rad_src = third_party_dir.."/Radiance/src"
rad_common = rad_src.."/common"
rad_rt = rad_src.."/rt"
rad_ot = rad_src.."/ot"
rad_gen = rad_src.."/gen"

-- Check if it is windows
is_windows = (os.target()=="windows")
is_linux = (os.target()=="linux")
is_macos = (os.target()=="macosx")

