# CMakeLists.txt for tests directory
# Add subdirectories for each test module
add_subdirectory(someTests)


# Find valgrind
find_program(VALGRIND_PROGRAM valgrind)

if(VALGRIND_PROGRAM)
    # Add memory check tests for each module
    add_test(
        NAME someTests_memcheck
        COMMAND ${VALGRIND_PROGRAM}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --error-exitcode=1
            $<TARGET_FILE:someTests>
    )

    # Make memcheck tests depend on regular tests
    set_tests_properties(someTests_memcheck PROPERTIES DEPENDS SomeTestSuite)
endif()