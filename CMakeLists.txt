cmake_minimum_required (VERSION 3.16)
project (saturation VERSION 1.0)

set (CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard Version")
option (WERROR "Compiler warnings are errors")
option (INTEL_DIALECT "Use Intel rather than AT&T assembler syntax (for GCC/Clang)")

# Using the INTEL_DIALECT option with clang requires version 14 or later.
string (REGEX MATCH "^[0-9]+" CXX_COMPILER_MAJOR_VERSION ${CMAKE_CXX_COMPILER_VERSION})
if (INTEL_DIALECT AND
    ("${CMAKE_CXX_COMPILER_ID}" MATCHES Clang) AND
    (${CXX_COMPILER_MAJOR_VERSION} LESS 14))
  message (SEND_ERROR "INTEL_DIALECT option requires clang version 14 or later")
endif()

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
    -Wno-used-but-marked-unused
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
  if (INTEL_DIALECT)
    list (APPEND clang_options -masm=intel)
    list (APPEND gcc_options -masm=intel)
  endif ()

  target_compile_options (${target} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:${clang_options}>
    $<$<CXX_COMPILER_ID:GNU>:${gcc_options}>
    $<$<CXX_COMPILER_ID:MSVC>:${msvc_options}>
  )
endfunction (setup_target)

# The "library" itself.

add_library (saturation INTERFACE
  include/saturation/add.hpp
  include/saturation/div.hpp
  include/saturation/mul.hpp
  include/saturation/saturation.hpp
  include/saturation/sub.hpp
  include/saturation/types.hpp
)
target_include_directories (saturation INTERFACE ./include)

# Tiny demo program.

add_executable (demo main.cpp)
setup_target (demo)
target_link_libraries (demo PUBLIC saturation)
add_custom_command (TARGET demo
  PRE_LINK
  COMMAND unittests
  COMMENT Running unit tests
)

# Test targets.

set (gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
set (INSTALL_GTEST Off CACHE BOOL "Disable gtest install")
set (INSTALL_GMOCK Off CACHE BOOL "Disable gmock install")
add_subdirectory (googletest)

add_subdirectory (unittests)
add_subdirectory (klee)
