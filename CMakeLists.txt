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


# Checks that the 'klee' tool is available and that the compiler is clang.
function (klee_is_available result)
  find_program (KLEE_PATH klee)
  if ((CMAKE_CXX_COMPILER_ID MATCHES "Clang$") AND (NOT "${KLEE_PATH}" STREQUAL "KLEE_PATH-NOTFOUND"))
    set (${result} Yes PARENT_SCOPE)
  else()
    set (${result} No PARENT_SCOPE)
  endif()
endfunction (klee_is_available)


add_executable (saturation main.cpp sat.hpp)
setup_target (saturation)
klee_is_available (klee_available)
message (STATUS "KLEE available? ${klee_available}")


function (setup_klee_target name)
  setup_target (${name})
  target_compile_options (${name} PRIVATE -g -fno-threadsafe-statics -target x86_64-pc-linux-gnu)
endfunction (setup_klee_target)

function (add_klee_target)
  cmake_parse_arguments (
    klee_prefix
    "" # options
    "NAME" # one-value keywords
    "DEPENDS;SOURCES" # multi-value keywords.
    ${ARGN}
  )

  if (NOT klee_available)
    return ()
  endif ()
  
  set (target_base ${klee_prefix_NAME})  
  set (depends ${klee_prefix_DEPENDS})
  set (sources ${klee_prefix_SOURCES})

  # Target names derived from the NAME argument.
  set (bitcode_target "${target_base}-bc")
  set (exe_target "${target_base}-exe")
  set (run_target "${target_base}-run")

  # Bitcode Target
    
  add_library (${bitcode_target} OBJECT ${sources})
  setup_klee_target (${bitcode_target})
  target_compile_options (${bitcode_target} PRIVATE -emit-llvm)

  # Executable Target
   
  add_executable (${exe_target} ${sources})
  setup_klee_target (${exe_target})
  target_compile_definitions (${exe_target} PRIVATE KLEE_RUN=1)
  target_compile_options (${exe_target} PRIVATE -fsanitize=undefined)
  target_link_options (${exe_target} PRIVATE -fsanitize=undefined)
  target_link_libraries (${exe_target} PRIVATE /home/klee/klee_build/lib/libkleeRuntest.so)
  target_link_libraries (${exe_target} PRIVATE ${depends})

  # Run Target
    
  set (link_llvm_lib "") # collection of dependent libraries.
  foreach (dependent ${depends})
    list (APPEND link_llvm_lib "--link-llvm-lib=$<TARGET_FILE:${dependent}-bc>")
  endforeach (dependent)


  
  add_custom_target (
      ${run_target}
      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/kout
      COMMAND rm -r -f ${CMAKE_CURRENT_BINARY_DIR}/kout/${target_base}
    COMMAND klee
            --libc=uclibc
            --posix-runtime
            --only-output-states-covering-new
            --optimize
            --output-dir=${CMAKE_CURRENT_BINARY_DIR}/kout/${target_base}
            --watchdog
            --max-time=600
            ${link_llvm_lib}
            $<TARGET_OBJECTS:${bitcode_target}>

    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_klee_test
            $<TARGET_FILE:${exe_target}>
            ${CMAKE_CURRENT_BINARY_DIR}/kout/${target_base}/*.ktest

    DEPENDS ${bitcode_target}
            ${exe_target}
            ${link_llvm_lib}

    COMMENT "Running KLEE for '${bitcode_target}'"
  )

  add_dependencies(klee-all ${run_target})
endfunction (add_klee_target)
    
add_custom_target (klee-all)
add_klee_target (NAME klee_adds32 SOURCES klee_adds32.cpp sat.hpp)
add_klee_target (NAME klee_subs32 SOURCES klee_subs32.cpp sat.hpp)
add_klee_target (NAME klee_divs32 SOURCES klee_divs32.cpp sat.hpp)
add_klee_target (NAME klee_muls32 SOURCES klee_muls32.cpp sat.hpp)
