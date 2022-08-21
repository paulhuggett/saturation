add_executable (unittests test_sat.cpp)
setup_target (unittests)
target_link_libraries (unittests PRIVATE saturation gmock_main)
