add_executable (unittests test_sat.cpp ../saturation.hpp)
setup_target (unittests)
target_link_libraries (unittests PRIVATE gmock_main)
