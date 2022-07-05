project (saturation)
cmake_minimum_required (VERSION 3.16)

set (CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard Version")

function (setup_target target)
  set (clang_options
    -Weverything
    -Wno-c99-extensions
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-c++14-extensions
    -Wno-disabled-macro-expansion
    -Wno-exit-time-destructors
    -Wno-global-constructors
    -Wno-padded
    -Wno-weak-vtables
    -Wno-nullability-extension
  )
  set (gcc_options
    -Wall
    -Wextra
    -pedantic
  )
  set (msvc_options
    -W4
    -wd4324 # 4324: structure was padded due to alignment specifier
  )

  if (WERROR)
      list (APPEND clang_options -Werror)
      list (APPEND gcc_options -Werror)
      list (APPEND msvc_options /WX)
  endif ()

  target_compile_options (${target} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:${clang_options}>
    $<$<CXX_COMPILER_ID:GNU>:${gcc_options}>
    $<$<CXX_COMPILER_ID:MSVC>:${msvc_options}>
  )
endfunction (setup_target)


add_subdirectory (klee)

# Tell gtest to link against the "Multi-threaded Debug DLL runtime library" on Windows.
set (gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
# Don't install either gtest or gmock.
set (INSTALL_GTEST Off CACHE BOOL "disable gtest install")
set (INSTALL_GMOCK Off CACHE BOOL "disable gmock install")
add_subdirectory (googletest)


add_subdirectory (unittests)

add_executable (saturation main.cpp saturation.hpp)
setup_target (saturation)
add_custom_command (TARGET saturation
                    PRE_LINK
                    COMMAND unittests
                    COMMENT Running unit tests)
