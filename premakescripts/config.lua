-- Configurations for premake

-- Emp basic organizations
libs_dir = "../libs"
third_party_dir = "../3rdparty"

-- Google test directory
google_test_dir = "../googletest/googletest"

-- Radiance specific locations
rad_common = third_party_dir.."/Radiance/src/common"
rad_rt = third_party_dir.."/Radiance/src/rt"

-- Check if it is windows
is_windows = (os.target()=="windows")
is_linux = (os.target()=="linux")
is_macos = (os.target()=="macosx")


