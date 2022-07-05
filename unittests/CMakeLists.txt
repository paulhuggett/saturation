add_executable (unittests test_sat.cpp ../saturation.hpp)
target_link_libraries (unittests PRIVATE gmock_main)
