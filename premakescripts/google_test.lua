
project "GoogleTest"
    kind "StaticLib"
    files { google_test_dir.."/src/gtest-all.cc" }
    includedirs { google_test_dir.."/include", google_test_dir }
    targetdir (libs_dir.."/%{cfg.platform}")        



