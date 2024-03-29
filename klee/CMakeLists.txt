# Checks that the 'klee' tool is on the path and that the C++ compiler is clang.
function (klee_is_available result)
  find_program (KLEE_PATH klee)
  if ((CMAKE_CXX_COMPILER_ID MATCHES "Clang$") AND (NOT "${KLEE_PATH}" STREQUAL "KLEE_PATH-NOTFOUND"))
    set (${result} Yes PARENT_SCOPE)
  else()
    set (${result} No PARENT_SCOPE)
  endif()
endfunction (klee_is_available)



function (setup_klee_target target)
  setup_target (${target})
  target_include_directories (${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
  target_compile_options (${target} PRIVATE
    -g
    -fno-threadsafe-statics
    -target x86_64-pc-linux-gnu
  )
  target_compile_definitions (${target} PRIVATE HAVE_INT128)
endfunction (setup_klee_target)


# The KLEE targets are a bit complicated. There are three of them:
#
# 1. The bitcode target.
#    This is used for the KLEE run. The tool will symbolically execute the
#    bitcode and generate a series of test cases that execute the various
#    control paths through the code.
#
# 2. The executable target
#    This is an executable program that is used to exercise the test cases on
#    the target. We compile it with santizers enabled to try and shake out any
#    latent bugs.
#
# 3. The run target
#    Coordinates the running of the above processes.

function (add_klee_target)
  cmake_parse_arguments (
    klee_prefix
    "" # options
    "TARGET" # one-value keywords
    "DEPENDS;SOURCES" # multi-value keywords.
    ${ARGN}
  )

  if (NOT klee_available)
    return ()
  endif ()

  set (depends ${klee_prefix_DEPENDS})
  set (sources ${klee_prefix_SOURCES})

  # Target names derived from the TARGET argument.
  set (target_base ${klee_prefix_TARGET})
  set (bitcode_target "${target_base}-bc")
  set (exe_target "${target_base}-exe")
  set (run_target "${target_base}-run")

  # Bitcode Target
    
  add_library (${bitcode_target} OBJECT ${sources})
  setup_klee_target (${bitcode_target})
  target_compile_definitions (${bitcode_target} PRIVATE NDEBUG NO_INLINE_ASM)
  target_compile_options (${bitcode_target} PRIVATE -emit-llvm)
  target_link_libraries (${bitcode_target} PRIVATE saturation)

  # Executable Target
   
  add_executable (${exe_target} ${sources})
  setup_klee_target (${exe_target})
  target_compile_definitions (${exe_target} PRIVATE KLEE_RUN=1)
  target_compile_options (${exe_target} PRIVATE -fsanitize=undefined)
  target_link_options (${exe_target} PRIVATE -fsanitize=undefined --rtlib=compiler-rt -lgcc_s)
  target_link_libraries (${exe_target} PRIVATE /home/klee/klee_build/lib/libkleeRuntest.so)
  target_link_libraries (${exe_target} PRIVATE ${depends})
  target_link_libraries (${exe_target} PRIVATE saturation)

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
            --output-dir=${CMAKE_CURRENT_BINARY_DIR}/kout/${target_base}
            --watchdog
            --max-time=1000
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
    

klee_is_available (klee_available)
message (STATUS "KLEE available? ${klee_available}")
if (klee_available)
  add_custom_target (klee-all)

  # Unsigned
  add_klee_target (TARGET klee_addu64 SOURCES addu64.cpp)
  add_klee_target (TARGET klee_addu32 SOURCES addu32.cpp)
  add_klee_target (TARGET klee_addu16 SOURCES addu16.cpp)
  add_klee_target (TARGET klee_addu8 SOURCES addu8.cpp)

  add_klee_target (TARGET klee_subu64 SOURCES subu64.cpp)
  add_klee_target (TARGET klee_subu32 SOURCES subu32.cpp)
  add_klee_target (TARGET klee_subu16 SOURCES subu16.cpp)
  add_klee_target (TARGET klee_subu8 SOURCES subu8.cpp)

  add_klee_target (TARGET klee_mulu64 SOURCES mulu64.cpp)
  add_klee_target (TARGET klee_mulu32 SOURCES mulu32.cpp)
  add_klee_target (TARGET klee_mulu16 SOURCES mulu16.cpp)
  add_klee_target (TARGET klee_mulu8 SOURCES mulu8.cpp)

  add_klee_target (TARGET klee_divu64 SOURCES divu64.cpp)
  add_klee_target (TARGET klee_divu32 SOURCES divu32.cpp)
  add_klee_target (TARGET klee_divu16 SOURCES divu16.cpp)
  add_klee_target (TARGET klee_divu8 SOURCES divu8.cpp)

  # Signed
  add_klee_target (TARGET klee_adds64 SOURCES adds64.cpp)
  add_klee_target (TARGET klee_adds32 SOURCES adds32.cpp)
  add_klee_target (TARGET klee_adds16 SOURCES adds16.cpp)
  add_klee_target (TARGET klee_adds8 SOURCES adds8.cpp)

  add_klee_target (TARGET klee_subs64 SOURCES subs64.cpp)
  add_klee_target (TARGET klee_subs32 SOURCES subs32.cpp)
  add_klee_target (TARGET klee_subs16 SOURCES subs16.cpp)
  add_klee_target (TARGET klee_subs8 SOURCES subs8.cpp)

  add_klee_target (TARGET klee_muls64 SOURCES muls64.cpp)
  add_klee_target (TARGET klee_muls32 SOURCES muls32.cpp)
  add_klee_target (TARGET klee_muls16 SOURCES muls16.cpp)
  add_klee_target (TARGET klee_muls8 SOURCES muls8.cpp)

  add_klee_target (TARGET klee_divs64 SOURCES divs64.cpp)
  add_klee_target (TARGET klee_divs32 SOURCES divs32.cpp)
  add_klee_target (TARGET klee_divs16 SOURCES divs16.cpp)
  add_klee_target (TARGET klee_divs8 SOURCES divs8.cpp)
endif ()
