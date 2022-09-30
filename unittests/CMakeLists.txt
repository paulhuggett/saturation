add_executable (unittests
    test_8.cpp
    test_16.cpp
    test_32.cpp
    test_multiply.cpp
    test_sat.cpp
)
setup_target (unittests)
target_link_libraries (unittests PRIVATE saturation gmock_main)
