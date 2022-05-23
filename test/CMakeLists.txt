
set(TEST_CC_FLAGS
   -static -O2
#   -Wl,--disable-new-dtags,-rpath,${TACHY_TOOLCHAIN_HOME}/sysroot/lib
#   -lm
)

set(tests
    array_sum array_and
    arrays_and #or xor
    arrays_add arrays_mul
)

#set(atomic4_cc_flags -std=c11 -latomic)
#set(test8_cc_flags ${CMAKE_CURRENT_SOURCE_DIR}/test8.S)

foreach(test_name IN LISTS tests)

    set(test_path ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.cpp)

    if(DEFINED ${test_name}_cc_flags)
        set(test_cc_flags "${${test_name}_cc_flags}")
    endif()

    add_executable(${test_name} ${test_path})
    target_compile_options(${test_name} PRIVATE ${TEST_CC_FLAGS} ${test_cc_flags})
    add_dependencies(${test_name} generate)

    add_test(NAME ${test_name} COMMAND ${FSIM} ${test_name})

endforeach()

